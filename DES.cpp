#include "DES.h"
#include "Permutations.h"
#include <string>
#include <vector>
#include <cstring> 
#include <iostream>
using namespace std;

DES::DES(){
}
// TODO: function documentation
vector<string> DES::permutate_key(string key){
    //string* c0_d0 = new string[2];

    // Go through row and column of permutation table get that index in the key
    // and append to the new vector. Account for position 0 bias
    string permutedKey = "";
    for (const auto& row : Permutations::PC1) {
        for (int pos : row) {
            permutedKey += key[pos - 1];  
        }
    }

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

    //string result(permutated_plaintext.begin(), permutated_plaintext.end());
    // cout << "Original:   "<< plaintext << endl;
    // cout << "Permutated: " << result << endl;
    string permutated_result(permutated_plaintext.begin(), permutated_plaintext.end());
    return permutated_result;



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

string DES::applyPC2(const string& combined) {
    string roundKey(48, '0');
    for (int i = 0; i < 48; i++) {
        roundKey[i] = combined[Permutations::PC2[i] - 1];
    }
    return roundKey;
}


void DES::generate_subkeys() {
    permutate_key((this->c0_d0.at(0)+this->c0_d0.at(1)));  

    vector<string> roundKeys;
    string combinedKey;

    for (int i = 0; i < 16; i++) {
        // Perform left shift according to the schedule
        left_shift(Permutations::shiftSchedule[i]);

        // Combine and permute to create the round key
        combinedKey = c0_d0[0] + c0_d0[1];
        string roundKey = applyPC2(combinedKey);
        roundKeys.push_back(roundKey);
    }
}
