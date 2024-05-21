/*
Author: Levi Hutchins C3386116
Course: COMP3260
Desc:
*/
#include "DES2.h"
#include "Permutations.h"
#include <string>
#include <vector>
#include <cstring> 
#include <iostream>
#include <bitset>
using namespace std;

DES2::DES2() : is_prime_key(false) {}

/**
 * @param key: Plaintext key from file
 * CASE1: @return c0_d0: 28 bit split key c0_d0[0] and [1]
 * CASE2: @return c0_d0_prime: 28 bit split key c0_d0_prime[0] and [1]
 * @desc: Given a key perform the permutation of PC1 and split into two halves.
 *        Return the correct key depending it is the prime key given. 
 */
vector<string> DES2::permutate_key(string key) {
    // Go through row and column of permutation table get that index in the key
    // and append to the new vector. Account for position 0 bias
    string permutedKey = "";
    for (const auto& row : Permutations::PC1) {
        for (int pos : row) {
            permutedKey += key[pos - 1];
        }
    }

    if (!this->is_prime_key) {
        this->c0_d0.clear();
        this->c0_d0.push_back(permutedKey.substr(0, 28));
        this->c0_d0.push_back(permutedKey.substr(28, 28));
    } else {
        this->c0_d0_prime.clear();
        this->c0_d0_prime.push_back(permutedKey.substr(0, 28));
        this->c0_d0_prime.push_back(permutedKey.substr(28, 28));
    }
    return !this->is_prime_key ? this->c0_d0 : this->c0_d0_prime;
}

/**
 * @param plaintext: string that is retrieved from the input file and to be permutated
 * @return permutated_result: string of plaintext after permutation
 * @desc: Perform the inital permutation on the plaintext with the IP values.
 */
string DES2::permutate_plaintext(string plaintext){
    
    vector<char> permutated_plaintext(64);

    // Go through row and column of permutation table get that index in the plaintext
    // and append to the new vector. Account for position 0 bias
    for(int i = 0; i < Permutations::IP.size(); i++){
        for(int j = 0; j < Permutations::IP.size(); j++){
            permutated_plaintext[i * 8 + j] = plaintext[Permutations::IP[i][j]-1];
        }
    }
    // Join the permutated characters into new permutated plaintext
    string permutated_result(permutated_plaintext.begin(), permutated_plaintext.end());
    return permutated_result;
}

/** 
 * @param data The data to be permuted.
 * @return The final permuted data string.
 * @description This method performs the final permutation step as specified 
 *               by the DES algorithm. It takes the input data and applies the 
 *               inverse initial permutation (IP^-1) to reorder the bits. 
 */
string DES2::final_permutation(const string& data) {
    string permuted(64, '0');
    for(int i = 0; i < 64; i++) {
        permuted[i] = data[Permutations::INVERSE_IP[i/8][i%8] - 1];
    }
    return permuted;
}

/**
 * @param count: int that determines the number of shifts (determined from shift schedule)
 * @desc:  Performs left shift count amount of times on the respective
 */
void DES2::left_shift(int count){
    // Perform left shift on the original key halves
    if (!this->is_prime_key) {
        for (int k = 0; k < 2; k++) {
            string &half = this->c0_d0[k];
            string temp = half;
            for (size_t i = 0; i < half.size(); i++) {
                half[i] = temp[(i + count) % half.size()];
            }
        }
    } else {
        // Perform left shift on the prime key halves
        for (int k = 0; k < 2; k++) {
            string &half = this->c0_d0_prime[k];
            string temp = half;
            for (size_t i = 0; i < half.size(); i++) {
                half[i] = temp[(i + count) % half.size()];
            }
        }
    }
}

/**
 * @param combined The combined key halves.
 * @return The resulting round key after permutation.
 * @description This method applies the PC2 permutation table to the combined 
 *               key halves, which have undergone left shifts. 
 */
string DES2::applyPC2(const string& combined) {
    string roundKey(48, '0');
    for (int i = 0; i < 48; i++) {
        roundKey[i] = combined[Permutations::PC2[i] - 1];
    }
    return roundKey;
}

/**
 * @return None.
 * @description This method generates all 16 subkeys required for the DES rounds. 
 *               It iterates through the key schedule, performing left shifts on 
 *               the key halves according to the shift schedule specified by DES. 
 */
void DES2::generate_subkeys() {
    string combinedKey = "";
    if (!this->is_prime_key) {
        this->roundKeys.clear();
        for (int i = 0; i < 16; i++) {
            // Shift depending on schedule and combine before PC2 permutation
            left_shift(Permutations::shiftSchedule[i]);
            combinedKey = c0_d0[0] + c0_d0[1];
            string roundKey = applyPC2(combinedKey);
            this->roundKeys.push_back(roundKey);
        }
    } else {
        this->roundKeys_prime.clear();
        for (int i = 0; i < 16; i++) {
            // Shift depending on schedule and combine before PC2 permutation
            left_shift(Permutations::shiftSchedule[i]);
            combinedKey = c0_d0_prime[0] + c0_d0_prime[1];
            string roundKey = applyPC2(combinedKey);
            this->roundKeys_prime.push_back(roundKey);

        }
    }
}

/** 
 * @param a The first binary string.
 * @param b The second binary string.
 * @return The result of the XOR operation between the input strings.
 * @description This function takes two binary strings as input and performs the 
 *               bitwise XOR operation between them. 
 */
string DES2::xor_strings(const string& a, const string& b) {
    string result(a.size(), '0');
    for (size_t i = 0; i < a.size(); i++) {
        result[i] = (a[i] == b[i] ? '0' : '1');
    }
    return result;
}

/**
 * @param right The right half of the plaintext or ciphertext.
 * @param roundKey The round key used for encryption or decryption.
 * @return The result of the Feistel function, which includes expansion permutation,
 *         XOR operation with the round key, S-box substitution, and permutation.
 * @description The Feistel function is a crucial component of the DES algorithm. 
 *               It operates on the right half of the plaintext during encryption 
 *               or ciphertext during decryption. 
 */
string DES2::feistel_function(const string& right, const string& roundKey) {
     // Expand the right half from 32 bits to 48 bits using the expansion permutation table
    string expandedRight(48, '0');
    for (int i = 0; i < 8; i++) {  
        for (int j = 0; j < 6; j++) {
            int idx = i * 6 + j;
            expandedRight[idx] = right[Permutations::EXPANSION_PERMUTATION[i][j] - 1];
        }
    }
    string result = xor_strings(expandedRight, roundKey);
    // Apply inverse of expansion permutation to contract from 48 to 32 bits
    string output(32, '0');
    for (int i = 0; i < 8; i++) { 
        for (int j = 0; j < 4; j++) {  
            int idx = i * 4 + j;
            output[idx] = result[Permutations::INVERSE_IP[i][j] - 1];
        }
    }
    return output;
}


/**
 * @param pt, pt_prime
 * @return None
 * @description: Given two strings return the number of different bits
 */
void DES2::count_bit_changes(const string& pt, const string& pt_prime) {
        if (pt.size() != pt_prime.size()) {
            std::cerr << "Error: Plaintext sizes do not match." << std::endl;
            return;
        }
        int count = 0;
        for (size_t i = 0; i < pt.size(); ++i) {
            if (pt[i] != pt_prime[i]) count++;
        }

        this->bit_differences.push_back(count);
}

/** 
 * @param plaintext The plaintext string to be encrypted.
 * @param key The encryption key string.
 * @return The ciphertext string resulting from the encryption process.
 * @description: Performs the DES encryption on both plaintexts which differ by 1 bit
 *              DES2: SBoxes missing from F function
 */
string DES2::encrypt(const string& pt, const string& pt_prime, const string& key) {
     // Clear any previously stored round keys and bit differences
    this->roundKeys.clear();
    this->roundKeys_prime.clear();
    this->bit_differences.clear();

    this->is_prime_key = false;

    permutate_key(key);
    generate_subkeys();

    // Perform the initial permutation on the plaintext
    string pt_inital_perm = permutate_plaintext(pt);
    string pt_left = pt_inital_perm.substr(0, 32);
    string pt_right = pt_inital_perm.substr(32, 32);

    // Perform the initial permutation on the prime plaintext
    string pt_prime_initial_perm = permutate_plaintext(pt_prime);
    string pt_prime_left = pt_prime_initial_perm.substr(0, 32);
    string pt_prime_right = pt_prime_initial_perm.substr(32, 32);

    for (int i = 0; i < 16; i++) {
        // Save the current left half to use as the next right half
        string nextRight = pt_left;
        pt_left = pt_right;
        // Apply Feistel function and XOR with the saved left half to get the new right half
        pt_right = xor_strings(nextRight, feistel_function(pt_right, this->roundKeys[i]));

        // Repeat the same process for the prime plaintext
        string nextRight_PRIME = pt_prime_left;
        pt_prime_left = pt_prime_right;
        pt_prime_right = xor_strings(nextRight_PRIME, feistel_function(pt_prime_right, this->roundKeys[i]));
        count_bit_changes((pt_right + pt_left), (pt_prime_right + pt_prime_left));
    }
    // Perform the final permutation and return the resulting ciphertext
    string finalPermutation = final_permutation(pt_right + pt_left); 
    return finalPermutation;
}


/** 
 * @param pt, key, key_prime key and key prime differ by 1 bit
 * @return vector of two strings which is plaintext encrypted with both keys.
 * @description: Performs the DES encryption on with keys which differ by 1 bit. Performs 
 *              the same process as encrypt() however make use of the two keys on the plaintext
 *              DES2: SBoxes missing from F function
 */
vector<string> DES2::encrypt_with_two_keys(const string& pt, const string& key, const string& key_prime) {
    vector<string> ciphers;
    // Clear any previously stored round keys and bit differences
    this->roundKeys.clear();
    this->roundKeys_prime.clear();
    this->bit_differences.clear();
    this->is_prime_key = false;

    permutate_key(key);
    generate_subkeys();

    // Generate round keys for the prime key
    this->is_prime_key = true;
    permutate_key(key_prime);
    generate_subkeys();

    // Perform the initial permutation on the plaintext
    string pt_initial_perm = permutate_plaintext(pt);
    string pt_left = pt_initial_perm.substr(0, 32);
    string pt_right = pt_initial_perm.substr(32, 32);

    // Perform the initial permutation on the prime plaintext
    string pt_prime_left = pt_left;
    string pt_prime_right = pt_right;

    for (int i = 0; i < 16; i++) {
                // Save the current left half to use as the next right half
        string nextRight = pt_left;
        pt_left = pt_right;
        // Apply Feistel function and XOR with the saved left half to get the new right half
        pt_right = xor_strings(nextRight, feistel_function(pt_right, this->roundKeys[i]));

        // Repeat the same process for the prime plaintext
        string nextRight_PRIME = pt_prime_left;
        pt_prime_left = pt_prime_right;
        
        pt_prime_right = xor_strings(nextRight_PRIME, feistel_function(pt_prime_right, this->roundKeys_prime[i]));
        count_bit_changes(pt_right + pt_left, pt_prime_right + pt_prime_left);


    }
    // Perform the final permutation and add the resulting ciphertexts to the ciphers vector
    ciphers.push_back(final_permutation(pt_right+pt_left));
    ciphers.push_back(final_permutation(pt_prime_right+pt_prime_left));

    return ciphers;
}

/**
 * @param ciphertext The ciphertext string to be decrypted.
 * @param key The decryption key string.
 * @return The plaintext string resulting from the decryption process.
 * @description This method decrypts the ciphertext using the Data Encryption Standard (DES) 
 *               algorithm with the provided key. It begins by permutating the key and 
 *               generating all 16 subkeys required for the decryption rounds. The ciphertext 
 *               undergoes initial permutation and is split into left and right halves. 
 */
string DES2::decrypt(const string& ciphertext, const string& key) {
    permutate_key(key);
    generate_subkeys();

    // Perform the initial permutation on the ciphertext
    string initialPermutation = permutate_plaintext(ciphertext);
    string left = initialPermutation.substr(0, 32);
    string right = initialPermutation.substr(32, 32);

    
    for (int i = 15; i >= 0; i--) {
        // Save the current left half to use as the next right half
        string nextRight = left;
        left = right;
        // Apply Feistel function and XOR with the saved left half to get the new right half
        right = xor_strings(nextRight, feistel_function(right, roundKeys[i]));
    }

    // Perform the final permutation and return the resulting plaintext
    string finalPermutation = final_permutation(right + left);
    return finalPermutation;
}

// Retuns the vector of bit differences at every round
vector<int> DES2::get_bit_difference(){
    return this->bit_differences;
}