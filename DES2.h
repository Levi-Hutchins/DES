#include <string>
#include <vector>
#ifndef DES2_H
#define DES2_H
using namespace std;

class DES2{

    public:

        DES2();


        vector<string> permutate_key(string key);

        string permutate_plaintext(string plaintext);

        string applyPC2(const string& combined);

        void left_shift(int count);

        void generate_subkeys();

        string feistel_function(const string& right, const string& round_keys);

        string sBox_substitution(const string &input);


        string encrypt(const string &plaintext, const string& plaintext_PRIME, const string& key);
        vector<string> encrypt_with_two_keys(const string& pt, const string& key, const string& key_prime);

        string decrypt(const string& ciphertext, const string& key);

        string final_permutation(const string& data);

        void count_bit_changes(const string& pt, const string& pt_prime);

        string xor_strings(const string& a, const string& b);

        vector<int> get_bit_difference();

        




    private:
        vector<int> bit_differences;
        vector<string> c0_d0;
        vector<string> c0_d0_prime;
        vector<string> roundKeys;
        vector<string> roundKeys_prime;
        string original_key;
        bool is_prime_key = false;




};



#endif