#include "DES0.h"
#include "DES1.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
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

void handle_outfile(const string* data){
  
    using chrono::high_resolution_clock;
    using chrono::duration_cast;
    using chrono::duration;
    using chrono::milliseconds;

    auto t1 = high_resolution_clock::now();
    DES0 des0;
    DES1 des1;
    // DES2 des2;
    // DES3 des3;
    auto t2 = high_resolution_clock::now();
    duration<double, milli> ms_double = t2 - t1;
    string time = to_string(ms_double.count());

    string cipher = des0.encrypt(data[0], data[1], data[2]);
    vector<int> des0_bits = des0.get_bit_difference();
    string cipher_prime = des0.encrypt(data[1], data[0], data[2]);

    string cipher1 = des1.encrypt(data[0], data[1], data[2]);
    vector<int> des1_bits = des1.get_bit_difference();
    string cipher_prime1 = des1.encrypt(data[1], data[0], data[2]);



    ofstream outfile("outfile.txt");
    outfile << "Avalanche Demonstration" << endl;
    outfile << "Plaintext P: " << data[0] << endl;;
    outfile << "Plaintext P': " << data[1] << endl;
    outfile << "Key K: " << data[2] << endl;
    outfile << "Key K': " << data[3] << endl;
    outfile << "Total Running Time: " + time + "ms" <<endl;
    outfile << endl;

    outfile << "P and P' under K"<< endl;
    outfile << "Ciphertext C:  " << cipher << endl;
    outfile << "Ciphertext C': " << cipher_prime << endl;


    outfile << std::left;  // Align text to the left
    outfile << std::setw(15) << "Round"
              << std::setw(15) << "DES0"
              << std::setw(15) << "DES1"
              << std::setw(15) << "DES2"
              << std::setw(15) << "DES3" << std::endl;

    outfile << std::string(15 * 5, '-') << std::endl;

    for(int i = 0; i < 16; i++){
        outfile << std::setw(15) << i
                << std::setw(15) << des0_bits.at(i)
                << std::setw(15) << des1_bits.at(i)
                << std::setw(15) << "" << endl;

    }


 



    outfile.close();
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
    DES1 alg = DES1();


    // PASSED
    //alg.permutate_plaintext(data[0]);
    cout << "Plaintext: " <<  data[0] << endl;
    string cipher = alg.encrypt(data[0], data[1], data[2]);
    cout << "Cipher:    "<<  cipher << endl;
    cout << "Decrypt:   "<<alg.decrypt(cipher, data[1]) << endl;



    handle_outfile(data);
    //cout <<alg.encrypt(data[0], data[2]) << endl;
    //PASSED
    //vector<string> keys = alg.permutate_key(data[2]);
    //cout << keys[0] << keys[1] << endl;
    // PASSED
    //alg.left_shift(keys, 2);


   
    return 0;
}
