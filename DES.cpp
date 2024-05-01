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
 * @param : 
 * @return :
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
 * @param count: int that determines the number of shifts  
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
 * @param : 
 * @return :
 */
string DES::applyPC2(const string& combined) {
    string roundKey(48, '0');
    for (int i = 0; i < 48; i++) {
        roundKey[i] = combined[Permutations::PC2[i] - 1];
    }
    return roundKey;
}

/**
 * @param : 
 * @return :
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
 * @param : 
 * @return :
 */
string DES::xor_strings(const string& a, const string& b) {
    string result(a.size(), '0');
    for (size_t i = 0; i < a.size(); i++) {
        result[i] = (a[i] == b[i] ? '0' : '1');
    }
    return result;
}

/**
 * @param : 
 * @return :
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
 * @param : 
 * @return :
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

/**
 * @param : 
 * @return :
 */
string DES::encrypt(const string& plaintext, const string& key) {
    permutate_key(key);
    generate_subkeys();

    string initialPermutation = permutate_plaintext(plaintext);
    string left = initialPermutation.substr(0, 32);
    string right = initialPermutation.substr(32, 32);

    for (int i = 0; i < 16; i++) {
        string nextRight = left;
        left = right;
        right = xor_strings(nextRight, feistel_function(right, this->roundKeys[i]));
    }

    string finalPermutation = final_permutation(right + left); 

    return finalPermutation;
}
/**
 * @param : 
 * @return :
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