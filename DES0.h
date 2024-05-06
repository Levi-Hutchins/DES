#include <string>
#include <vector>
#ifndef DES0_H
#define DES0_H
using namespace std;

class DES0{

    public:

        DES0();

        virtual vector<string> permutate_key(string key);

        virtual string permutate_plaintext(string plaintext);

        virtual string applyPC2(const string& combined);


        virtual void left_shift(int count);

        virtual void generate_subkeys();

        virtual string feistel_function(const string& right, const string& round_keys);

        virtual string sBox_substitution(const string &input);

        virtual string xor_strings(const string &a, const string &b);

        virtual string encrypt(const string &plaintext, const string& key);

        virtual string decrypt(const string& ciphertext, const string& key);

        virtual string final_permutation(const string& data);

        virtual void count_bit_changes(const string& plaintext_after_round);




    private:
        vector<int> bit_differences;
        vector<string> c0_d0;
        vector<string> roundKeys;
        string original_plaintext;
        string original_key;




};



#endif