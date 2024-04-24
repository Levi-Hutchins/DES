#include "DES.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
using namespace std;

/**
 * @param file_location: string that represents file location
 * @return values: string array with the following:
 *                 [0] P - Plaintext
 *                 [1] P': Plaintext with 1 bit flipped
 *                 [2] K: Key for encryption
 *                 [3] K': Key with 1 bit flipped
 */
string* process_input(string file_location) {
     string* values = new string[4];
    ifstream file(file_location);

    if (!file.is_open()) {
        delete[] values; 
        throw runtime_error("Unable to open file: " + file_location);
    }

    for (int i = 0; i < 4 && getline(file, values[i]); ++i);
    file.close();

    return values;
}

int main(int argc, const char * argv[]) {

    string* data = nullptr;
    try {
        data = process_input(argv[1]);
        // for (int i = 0; i < 4; i++) {
        //     cout << data[i] << endl;
        // }
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
      
    }
    DES alg = DES();
    // PASSED
    //alg.permutate_plaintext(data[0]);

    //PASSED
    //string* keys = alg.permutate_key(data[2]);
    // PASSED
    //alg.left_shift(keys, 2);

    
    return 0;
}
