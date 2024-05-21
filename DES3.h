/*
Author: Levi Hutchins C3386116
Course: COMP3260
Desc:
*/
#include <string>
#include <vector>
#ifndef DES3_H
#define DES3_H
using namespace std;
class DES3{

    public:

        DES3();

        string permutate_plaintext(string plaintext);
        string applyPC2(const string& combined);
        string feistel_function(const string& right, const string& round_keys);
        string sBox_substitution(const string &input);
        string xor_strings(const string &a, const string &b);
        string decrypt(const string& ciphertext, const string& key);
        string final_permutation(const string& data);
        string encrypt(const string &plaintext, const string& plaintext_PRIME, const string& key);
        
        vector<string> permutate_key(string key);
        vector<string> encrypt_with_two_keys(const string& pt, const string& key, const string& key_prime);
        vector<int> get_bit_difference();

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
