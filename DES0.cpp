#include "DES0.h"
#include "SBoxes.h"
#include "Permutations.h"
#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <bitset>
using namespace std;

DES0::DES0() : is_prime_key(false) {}

vector<string> DES0::permutate_key(string key) {
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

string DES0::permutate_plaintext(string plaintext){
    vector<char> permutated_plaintext(64);
    for(int i = 0; i < Permutations::IP.size(); i++){
        for(int j = 0; j < Permutations::IP.size(); j++){
            permutated_plaintext[i * 8 + j] = plaintext[Permutations::IP[i][j]-1];
        }
    }
    return string(permutated_plaintext.begin(), permutated_plaintext.end());
}

string DES0::final_permutation(const string& data) {
    string permuted(64, '0');
    for(int i = 0; i < 64; i++) {
        permuted[i] = data[Permutations::INVERSE_IP[i/8][i%8] - 1];
    }
    return permuted;
}

void DES0::left_shift(int count){
    if (!this->is_prime_key) {
        for (int k = 0; k < 2; k++) {
            string &half = this->c0_d0[k];
            string temp = half;
            for (size_t i = 0; i < half.size(); i++) {
                half[i] = temp[(i + count) % half.size()];
            }
        }
    } else {
        for (int k = 0; k < 2; k++) {
            string &half = this->c0_d0_prime[k];
            string temp = half;
            for (size_t i = 0; i < half.size(); i++) {
                half[i] = temp[(i + count) % half.size()];
            }
        }
    }
}

string DES0::applyPC2(const string& combined) {
    string roundKey(48, '0');
    for (int i = 0; i < 48; i++) {
        roundKey[i] = combined[Permutations::PC2[i] - 1];
    }
    return roundKey;
}

void DES0::generate_subkeys() {
    string combinedKey = "";
    if (!this->is_prime_key) {

        this->roundKeys.clear();
        for (int i = 0; i < 16; i++) {
            left_shift(Permutations::shiftSchedule[i]);
            combinedKey = c0_d0[0] + c0_d0[1];
            string roundKey = applyPC2(combinedKey);
            this->roundKeys.push_back(roundKey);
        }
    } else {
        this->roundKeys_prime.clear();
        for (int i = 0; i < 16; i++) {
            left_shift(Permutations::shiftSchedule[i]);
            combinedKey = c0_d0_prime[0] + c0_d0_prime[1];
            string roundKey = applyPC2(combinedKey);
            this->roundKeys_prime.push_back(roundKey);

        }
    }
}

string DES0::xor_strings(const string& a, const string& b) {
    string result(a.size(), '0');
    for (size_t i = 0; i < a.size(); i++) {
        result[i] = (a[i] == b[i] ? '0' : '1');
    }
    return result;
}

string DES0::feistel_function(const string& right, const string& roundKey) {
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

string DES0::sBox_substitution(const string &input) {
    string output;
    int s_box_value = 0;
    for (int i = 0; i < 8; i++) {
        int index = i * 6;
        string six_bit_segment = input.substr(index, 6);
        int row = 2 * (six_bit_segment[0] - '0') + (six_bit_segment[5] - '0');
        int col = 8 * (six_bit_segment[1] - '0') + 4 * (six_bit_segment[2] - '0') +
                  2 * (six_bit_segment[3] - '0') + (six_bit_segment[4] - '0');
        switch(i) {
            case 0: s_box_value = SBoxes::S1[row][col]; break;
            case 1: s_box_value = SBoxes::S2[row][col]; break;
            case 2: s_box_value = SBoxes::S3[row][col]; break;
            case 3: s_box_value = SBoxes::S4[row][col]; break;
            case 4: s_box_value = SBoxes::S5[row][col]; break;
            case 5: s_box_value = SBoxes::S6[row][col]; break;
            case 6: s_box_value = SBoxes::S7[row][col]; break;
            case 7: s_box_value = SBoxes::S8[row][col]; break;
        }
        string four_bit_output = bitset<4>(s_box_value).to_string();
        output.append(four_bit_output);
    }
    return output;
}

void DES0::count_bit_changes(const string& pt, const string& pt_prime) {
    if (pt.size() != pt_prime.size()) {
        std::cerr << "Error: Sizes do not match." << std::endl;
        return;
    }
    int count = 0;
    for (size_t i = 0; i < pt.size(); ++i) {
        if (pt[i] != pt_prime[i]) count++;
    }
    this->bit_differences.push_back(count);
}

string DES0::encrypt(const string& pt, const string& pt_prime, const string& key) {
    this->roundKeys.clear();
    this->roundKeys_prime.clear();
    this->bit_differences.clear();

    this->is_prime_key = false;
    permutate_key(key);
    generate_subkeys();

    string pt_inital_perm = permutate_plaintext(pt);
    string pt_left = pt_inital_perm.substr(0, 32);
    string pt_right = pt_inital_perm.substr(32, 32);

    string pt_prime_initial_perm = permutate_plaintext(pt_prime);
    string pt_prime_left = pt_prime_initial_perm.substr(0, 32);
    string pt_prime_right = pt_prime_initial_perm.substr(32, 32);

    for (int i = 0; i < 16; i++) {
        string nextRight = pt_left;
        pt_left = pt_right;
        pt_right = xor_strings(nextRight, feistel_function(pt_right, this->roundKeys[i]));

        string nextRight_PRIME = pt_prime_left;
        pt_prime_left = pt_prime_right;
        pt_prime_right = xor_strings(nextRight_PRIME, feistel_function(pt_prime_right, this->roundKeys[i]));

        count_bit_changes((pt_right + pt_left), (pt_prime_right + pt_prime_left));
    }
    
    return final_permutation(pt_right + pt_left);
}

vector<string> DES0::encryptv2(const string& pt, const string& key, const string& key_prime) {
    vector<string> ciphers;
    this->roundKeys.clear();
    this->roundKeys_prime.clear();
    this->bit_differences.clear();

    this->is_prime_key = false;
    permutate_key(key);
    generate_subkeys();

    this->is_prime_key = true;
    permutate_key(key_prime);
    generate_subkeys();

    string pt_initial_perm = permutate_plaintext(pt);
    string pt_left = pt_initial_perm.substr(0, 32);
    string pt_right = pt_initial_perm.substr(32, 32);

    string pt_prime_left = pt_left;
    string pt_prime_right = pt_right;

    for (int i = 0; i < 16; i++) {
        count_bit_changes(pt_right + pt_left, pt_prime_right + pt_prime_left);

        string nextRight = pt_left;
        pt_left = pt_right;
        pt_right = xor_strings(nextRight, feistel_function(pt_right, this->roundKeys[i]));

        string nextRight_PRIME = pt_prime_left;
        pt_prime_left = pt_prime_right;
        pt_prime_right = xor_strings(nextRight_PRIME, feistel_function(pt_prime_right, this->roundKeys_prime[i]));
    }
    ciphers.push_back(final_permutation(pt_right+pt_left));
    ciphers.push_back(final_permutation(pt_prime_right+pt_prime_left));

    return ciphers;
}

string DES0::decrypt(const string& ciphertext, const string& key) {
    this->roundKeys.clear();
    this->roundKeys_prime.clear();

    this->is_prime_key = false;
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

    return final_permutation(right + left);
}

vector<int> DES0::get_bit_difference() {
    return this->bit_differences;
}
