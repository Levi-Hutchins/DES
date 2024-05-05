#include "DES.h"
#include "SBoxes.h"
#include "Permutations.h"
#include <string>
#include <vector>
#include <cstring> 
#include <iostream>
#include <bitset>
using namespace std;

DES::DES(){
}
/**
 * @param key: Plaintext key from file
 * @return c0_d0: 28 bit split key c0_d0[0] and [1]
 * @desc: 
 */vector<string> DES::permutate_key(string key){

    // Go through row and column of permutation table get that index in the key
    // and append to the new vector. Account for position 0 bias
    string permutedKey = "";
    for (const auto& row : Permutations::PC1) {
        for (int pos : row) {
            permutedKey += key[pos - 1];  
        }
    }
    // Ensure it is empty before pushing back (decryption)
    this->c0_d0.clear();
    // Position 0 = c0
    this->c0_d0.push_back(permutedKey.substr(0,28));
    // Position 1 = d0
    this->c0_d0.push_back(permutedKey.substr(28,28));

    return this->c0_d0;
}

/**
 * @param plaintext: string that is retrieved from the input file 
 * @return permutated_result: string of plaintext after permutation
 * @desc: 
 */
string DES::permutate_plaintext(string plaintext){
    
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
string DES::final_permutation(const string& data) {
    string permuted(64, '0');
    for(int i = 0; i < 64; i++) {
        permuted[i] = data[Permutations::INVERSE_IP[i/8][i%8] - 1];
    }
    return permuted;
}

/**
 * @param c0_d0: string array representing the two 28 bit keys
 * @param count: int that determines the number of shifts (determined from shift schedule)
 * @desc:  Performs left shift count amount of times 
 */
void DES::left_shift(int count){
    for (int k = 0; k < 2; k++) {
        string &half = c0_d0[k];  
        string temp = half;      
        
        for (size_t i = 0; i < half.size(); i++) {
            half[i] = temp[(i + count) % half.size()];
        }
    
    }
}

/**
 * @param combined The combined key halves.
 * @return The resulting round key after permutation.
 * @description This method applies the PC2 permutation table to the combined 
 *               key halves, which have undergone left shifts. 
 */
string DES::applyPC2(const string& combined) {
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
void DES::generate_subkeys() {

    string combinedKey;

    for (int i = 0; i < 16; i++) {
        left_shift(Permutations::shiftSchedule[i]);

        combinedKey = c0_d0[0] + c0_d0[1];
        string roundKey = applyPC2(combinedKey);
        this->roundKeys.push_back(roundKey);
    }
}

/** 
 * @param a The first binary string.
 * @param b The second binary string.
 * @return The result of the XOR operation between the input strings.
 * @description This function takes two binary strings as input and performs the 
 *               bitwise XOR operation between them. 
 */
string DES::xor_strings(const string& a, const string& b) {
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
string DES::feistel_function(const string& right, const string& roundKey) {
    string expandedRight(48, '0');
    for (int i = 0; i < 8; i++) {  
        for (int j = 0; j < 6; j++) {
            int idx = i * 6 + j;
            expandedRight[idx] = right[Permutations::EXPANSION_PERMUTATION[i][j] - 1];
        }
    }

    string result = xor_strings(expandedRight, roundKey);

    string sBoxOutput = sBox_substitution(result);

    string output(32, '0');
    for (int i = 0; i < 4; i++) { 
        for (int j = 0; j < 8; j++) {
            int idx = i * 8 + j;
            output[idx] = sBoxOutput[Permutations::PERMUTATION_FUNCTION[i][j] - 1];
        }
    }

    return output;
}

/**
 * @param input The input string to be substituted using S-boxes.
 * @return The output string after S-box substitution.
 * @description This method performs S-box substitution on the input string using 
 *               the predefined S-boxes 1 through 8. It divides the input string 
 *               into 6-bit segments and computes the corresponding row and column 
 *               indices for each segment. 
 */
string DES::sBox_substitution(const string &input) {
    string output;   
    int s_box_value = 0;

    
    for (int i = 0; i < 8; i++) {
        int index = i * 6;
        string six_bit_segment = input.substr(index, 6);

        int row = 2 * (six_bit_segment[0] - '0') + (six_bit_segment[5] - '0');
        int col = 8 * (six_bit_segment[1] - '0') + 4 * (six_bit_segment[2] - '0') +
                  2 * (six_bit_segment[3] - '0') + (six_bit_segment[4] - '0');

        if(i == 0){ s_box_value = SBoxes::S1[row][col];}
        if(i == 1){ s_box_value = SBoxes::S2[row][col];}
        if(i == 2){ s_box_value = SBoxes::S3[row][col];}
        if(i == 3){ s_box_value = SBoxes::S4[row][col];}
        if(i == 4){ s_box_value = SBoxes::S5[row][col];}
        if(i == 5){ s_box_value = SBoxes::S6[row][col];}
        if(i == 6){ s_box_value = SBoxes::S7[row][col];}
        if(i == 7){ s_box_value = SBoxes::S8[row][col];}



        string four_bit_output = bitset<4>(s_box_value).to_string();
        output.append(four_bit_output);
    }
    return output;
}

void DES::count_bit_changes(const string& new_bits) {
    static string last_bits;  // Keep track of the bits from the last round
    if (last_bits.empty()) {
        last_bits = new_bits;  // Initialize with the first set of bits
        return;
    }

    int count = 0;
    for (size_t i = 0; i < new_bits.size() && i < last_bits.size(); ++i) {
        if (new_bits[i] != last_bits[i]) count++;
    }

    cout << "Bit changes from last round: " << count << endl;
    last_bits = new_bits;  // Update last_bits for the next round comparison
}



/** 
 * @param plaintext The plaintext string to be encrypted.
 * @param key The encryption key string.
 * @return The ciphertext string resulting from the encryption process.
 * @description This method encrypts the plaintext using the Data Encryption Standard (DES) 
 *               algorithm with the provided key. It begins by permutating the key and 
 *               generating all 16 subkeys required for the encryption rounds.
 */
string DES::encrypt(const string& plaintext, const string& key) {
    this->original_plaintext = plaintext;
    this->original_key = key;
    permutate_key(key);
    generate_subkeys();

    string initialPermutation = permutate_plaintext(plaintext);
    string left = initialPermutation.substr(0, 32);
    string right = initialPermutation.substr(32, 32);

    for (int i = 0; i < 16; i++) {
        string nextRight = left;
        left = right;
        right = xor_strings(nextRight, feistel_function(right, this->roundKeys[i]));

        count_bit_changes(right + left);
    }

    string finalPermutation = final_permutation(right + left); 

    return finalPermutation;
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
string DES::decrypt(const string& ciphertext, const string& key) {

    permutate_key(key);
    generate_subkeys();

   
    string initialPermutation = permutate_plaintext(ciphertext);
    string left = initialPermutation.substr(0, 32);
    string right = initialPermutation.substr(32, 32);

    
    for (int i = 15; i >= 0; i--) {
        string nextRight = left;
        left = right;
        right = xor_strings(nextRight, feistel_function(right, roundKeys[i]));
    }

    
    string finalPermutation = final_permutation(right + left);

    return finalPermutation;
}