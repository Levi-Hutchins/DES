#include "DES0.h"
#include "DES1.h"
#include "DES2.h"
#include "DES3.h"
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
 * @param is_encrypt: determine which file reading functionality is required
 * @return values: string array with the following:
 *   CASE1         [0] P - Plaintext
 *                 [1] P': Plaintext with 1 bit flipped
 *                 [2] K: Key for encryption
 *                 [3] K': Key with 1 bit flipped
 * @return values: string array with the following:
 *   CASE2         [0] P - Plaintext
 *                 [1] K: Key for encryption
 */
string* process_input(string file_location, bool is_encrypt) {

    if(is_encrypt){
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
    else{
        string* values = new string[2];
        ifstream file(file_location);

        if (!file.is_open()) {
            delete[] values; 
            throw runtime_error("Unable to open file: " + file_location);
        }

        for (int i = 0; i < 2 && getline(file, values[i]); ++i);
        file.close();
        return values;
    }
    
}

void handle_outfile(const string* data){
  
    using chrono::high_resolution_clock;
    using chrono::duration_cast;
    using chrono::duration;
    using chrono::milliseconds;

    auto t1 = high_resolution_clock::now();
    DES0 des0;
    DES1 des1;
    DES2 des2;
    DES3 des3;
    // DES3 des3;
    auto t2 = high_resolution_clock::now();
    duration<double, milli> ms_double = t2 - t1;
    string time = to_string(ms_double.count());

    string cipher = des0.encrypt(data[0], data[1], data[2]);
    string cipher_prime = des0.encrypt(data[1], data[0], data[2]);

    des1.encrypt(data[0], data[1], data[2]);
    des2.encrypt(data[0], data[1], data[2]);
    des3.encrypt(data[0], data[1], data[2]);



    ofstream outfile("analysis_outfile.txt");
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
                << std::setw(15) << des0.get_bit_difference().at(i)
                << std::setw(15) << des1.get_bit_difference().at(i)
                << std::setw(15) << des2.get_bit_difference().at(i)
                << std::setw(15) << des3.get_bit_difference().at(i)<< endl;

    }

    cipher = des0.encrypt(data[0], data[0], data[2]);
    cipher_prime = des0.encrypt(data[0], data[0], data[3]);

    des1.encrypt(data[0], data[1], data[2]);
    des2.encrypt(data[0], data[1], data[2]);
    des3.encrypt(data[0], data[1], data[2]);

    outfile << "P under K and K`"<< endl;
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
                << std::setw(15) << des0.get_bit_difference().at(i)
                << std::setw(15) << des1.get_bit_difference().at(i)
                << std::setw(15) << des2.get_bit_difference().at(i)
                << std::setw(15) << des3.get_bit_difference().at(i)<< endl;

    }

    outfile.close();
}

void handle_decryption_file(const string* data){
    ofstream outfile("decryption_outfile.txt");
    outfile << "DECRYPTION" << endl;
    outfile << "Ciphertext C: " << data[0] << endl;
    outfile << "Key K: " << data[1] << endl; 
    DES0 des = DES0();
    outfile << "Plaintext P: "<<des.decrypt(data[0], data[1]) << endl;;


}

bool validate_args(int argc, const char * argv[]){
    if(argc != 3){
        std::cerr << "Usage: " << argv[0] << " -e <file_name>" << std::endl;
        return false;
    }
    string first_arg = argv[1];
    if((first_arg == "-e" || first_arg == "-d") && argv[2] != nullptr){
        return true;
    }

    return false;


}


int main(int argc, const char * argv[]) {
    const char* GREEN_COLOR_CODE = "\033[1;32m";
    const char* RESET_COLOR_CODE = "\033[0m";
    
    // string* data = nullptr;
    // if(validate_args(argc, argv)){
    //     string flag = argv[1];

    //     if(flag == "-e"){
    //          try {
    //                     data = process_input(argv[2], true);
    //             } catch (const std::exception& e) {
    //                     cerr << "Error: " << e.what() << endl;
    //             }
    //         cout << " " << endl;
    //         handle_outfile(data);
    //         cout << GREEN_COLOR_CODE << "SUCCESS: " << RESET_COLOR_CODE << "Please see analysis_outfile.txt for results" << endl; 
    //         cout << " " << endl;

    //     }
    //     else if( flag == "-d"){
    //         try {
    //                     data = process_input(argv[2], false);
    //             } catch (const std::exception& e) {
    //                     cerr << "Error: " << e.what() << endl;
    //             }
    //         handle_decryption_file(data);
    //         cout << " " << endl;
    //         cout << GREEN_COLOR_CODE << "SUCCESS: " << RESET_COLOR_CODE << "Please see decryption_outfile.txt for results" << endl; 
    //         cout << " " << endl;

    //     }
    //     else{
    //         cerr << "An Error Occured " << endl;

    //     }
    // }
    // else{
    //     std::cerr << "Encryption Usage: " << argv[0] << " -e <file_name>" << std::endl;
    //     std::cerr << "Decryption Usage: " << argv[0] << " -d <file_name>" << std::endl;

    //     return 1;
    // }


    DES0 des0;
    string cipher  = des0.encryptv2("1101011000110101001001111001010110100010110001101110010100011101",
    "0110111100011010011110001000100001110001011100100111010000100010","1110111100011010011110001000100001110001011100100111010000100010");
    for (int i = 0; i < des0.get_bit_difference().size(); i++){
        cout << des0.get_bit_difference().at(i) << endl;
    }


    return 0;

  
}
