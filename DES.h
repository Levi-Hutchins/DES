#include <string>
#ifndef DES_H
#define DES_H
using namespace std;

class DES{

    public:

        DES();

        string* permutate_key(string key);

        string permutate_plaintext(string plaintext);

};



#endif