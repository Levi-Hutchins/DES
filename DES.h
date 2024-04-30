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


    private:
        vector<string> c0_d0;



};



#endif