/*
Author: Levi Hutchins C3386116
Course: COMP3260
Desc: Header file for DES1. Defines everything required for the DES1 variation.
*/
#include <string>
#include <vector>
#ifndef DES1_H
#define DES1_H
using namespace std;
class DES1{

    public:

        DES1();

        string permutate_plaintext(string plaintext);
        string applyPC2(const string& combined);
        string feistel_function(const string& right);
        string sBox_substitution(const string &input);
        string encrypt(const string &plaintext, const string& plaintext_PRIME, const string& key);
        string decrypt(const string& ciphertext, const string& key);
        string final_permutation(const string& data);
        string xor_strings(const string& a, const string& b);

        vector<int> get_bit_difference();
        vector<string> permutate_key(string key);
        vector<string> encrypt_with_two_keys(const string& pt, const string& key, const string& key_prime);

        
        void count_bit_changes(const string& pt, const string& pt_prime);

        void left_shift(int count);

        void generate_subkeys();

    private:
        vector<int> bit_differences;
        vector<string> c0_d0;
        vector<string> c0_d0_prime;
        vector<string> roundKeys;
        vector<string> roundKeys_prime;
        bool is_prime_key = false;
};
#endif