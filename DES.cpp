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
string* DES::permutate_key(string key){
    string* c0_d0 = new string[2];

    // Go through row and column of permutation table get that index in the key
    // and append to the new vector. Account for position 0 bias
    string permutedKey = "";
    for (const auto& row : Permutations::PC1) {
        for (int pos : row) {
            permutedKey += key[pos - 1];  
        }
    }


    c0_d0[0] = permutedKey.substr(0,28);
    c0_d0[1] = permutedKey.substr(28,55);



    return c0_d0;
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
    cout << permutated_result << endl;
    return permutated_result;



}

void DES::left_shift(string c0_d0[], int count){

   string key1 = c0_d0[0];
    char ind = key1.at(0);
    cout << "Before Shift: " << key1 << endl;
    for(int i = 0; i < key1.length()-1; i++){
        key1[i] = key1[i+1];
    }
    key1[-1] = ind;
    cout << "After Shift: " << key1 << endl;

    c0_d0[0] = key1;



}