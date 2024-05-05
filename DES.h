#include <string>
#include <vector>
#ifndef DES_H
#define DES_H
using namespace std;

class DES{

    public:

        DES();

        vector<string> permutate_key(string key);

        string permutate_plaintext(string plaintext);

        string applyPC2(const string& combined);


        void left_shift(int count);

        void generate_subkeys();

        string feistel_function(const string& right, const string& round_keys);

        string sBox_substitution(const string &input);

        string xor_strings(const string &a, const string &b);

        string encrypt(const string &plaintext, const string& key);

        string decrypt(const string& ciphertext, const string& key);

        string final_permutation(const string& data);

        void count_bit_changes(const string& plaintext_after_round);




    private:
        vector<int> bit_differences;
        vector<string> c0_d0;
        vector<string> roundKeys;
        string original_plaintext;
        string original_key;




};



#endif