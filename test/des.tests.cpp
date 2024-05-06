#include <iostream>
#include "../DES0.h"
const char* GREEN_COLOR_CODE = "\033[1;32m";
const char* RED_COLOR_CODE = "\033[1;31m";
const char* RESET_COLOR_CODE = "\033[0m";

bool compare_strings(const std::string& expected, const std::string& actual) {
    return expected == actual;
}

#define IS_TRUE(x) { \
    if (!(x)) \
        std::cout << RED_COLOR_CODE << " __ TEST FAILED __ " << RESET_COLOR_CODE << " Line :"<<  __LINE__ << std::endl; \
    else \
        std::cout << GREEN_COLOR_CODE << "-- TEST PASSED -- " << RESET_COLOR_CODE << std::endl; \
}

void des0_encrypt_decrypt() {
    DES0 des = DES0();

    // Plaintext
    std::string plaintext1 = "1101011000110101001001111001010110100010110001101110010100011101";
    std::string key1 = "0110111100011010011110001000100001110001011100100111010000100010";

    std::string plaintext2 = "1101011000110101001001111001010110100010110001101110010100011100";
    std::string key2 = "0110111100011010011110001000100001110001011100100111010000100011";

    std::string ciphertext1 = des.encrypt(plaintext1, key1);
    std::string decrypted_text1 = des.decrypt(ciphertext1, key1);

    std::string ciphertext2 = des.encrypt(plaintext2, key2);
    std::string decrypted_text2 = des.decrypt(ciphertext2, key2);

    IS_TRUE(compare_strings(plaintext1, decrypted_text1));
    IS_TRUE(compare_strings(plaintext2, decrypted_text2));

}

int main() {
    des0_encrypt_decrypt();
    
    return 0;
}