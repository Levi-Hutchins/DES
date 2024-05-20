#include <string>
#include <vector>
#ifndef DES0_H
#define DES0_H
using namespace std;

class DES0{

    public:

        DES0();


        vector<string> permutate_key(string key);

        string permutate_plaintext(string plaintext);

        string applyPC2(const string& combined);

        void left_shift(int count);

        void generate_subkeys();

        string feistel_function(const string& right, const string& round_keys);

        string sBox_substitution(const string &input);

        string xor_strings(const string &a, const string &b);

        string encrypt(const string &plaintext, const string& plaintext_PRIME, const string& key);

        string decrypt(const string& ciphertext, const string& key);

        string final_permutation(const string& data);
        string encryptv2(const string& pt, const string& key, const string& key_prime);       
        void count_bit_changes(const string& pt, const string& pt_prime);

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