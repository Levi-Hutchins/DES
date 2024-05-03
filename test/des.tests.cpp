#include <iostream>
#include "../DES.h"
const char* GREEN_COLOR_CODE = "\033[1;32m";
const char* RESET_COLOR_CODE = "\033[0m";

bool compare_strings(const std::string& expected, const std::string& actual) {
    return expected == actual;
}

#define IS_TRUE(x) { \
    if (!(x)) \
        std::cout << GREEN_COLOR_CODE << " failed on line " << RESET_COLOR_CODE << __LINE__ << std::endl; \
    else \
        std::cout << GREEN_COLOR_CODE << "-- TEST PASSED -- " << RESET_COLOR_CODE << std::endl; \
}

void test_encrypt_decrypt() {
    DES des = DES();

    std::string plaintext = "1101011000110101001001111001010110100010110001101110010100011101";

    std::string key = "0110111100011010011110001000100001110001011100100111010000100010";


    std::string ciphertext = des.encrypt(plaintext, key);
    std::string decrypted_text = des.decrypt(ciphertext, key);

    IS_TRUE(compare_strings(plaintext, decrypted_text));
}

int main() {
    test_encrypt_decrypt();
    return 0;
}
