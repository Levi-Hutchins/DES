#include <iostream>
#include "../DES0.h"
#include "../DES1.h"
#include "../DES2.h"
#include "../DES3.h"


const char* GREEN_COLOR_CODE = "\033[1;32m";
const char* RED_COLOR_CODE = "\033[1;31m";
const char* RESET_COLOR_CODE = "\033[0m";

bool compare_strings(const std::string& expected, const std::string& actual) {
    return expected == actual;
}

#define IS_TRUE(x) { \
    if (!(x)) \
        std::cout << RED_COLOR_CODE << "__ TEST FAILED __ " << RESET_COLOR_CODE << " Line :"<<  __LINE__ << std::endl; \
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

    std::string ciphertext1 = des.encrypt(plaintext1, plaintext2,  key1);
    std::string decrypted_text1 = des.decrypt(ciphertext1, key1);

    // std::string ciphertext2 = des.encrypt(plaintext2, key2);
    // std::string decrypted_text2 = des.decrypt(ciphertext2, key2);

    IS_TRUE(compare_strings(plaintext1, decrypted_text1));
    //IS_TRUE(compare_strings(plaintext2, decrypted_text2));

}
void test_encrypt_decrypt() {
    DES0 des0 = DES0();
    DES1 des1 = DES1();
    DES2 des2 = DES2();
    DES3 des3 = DES3();

    // Plaintext
    std::string plaintext1 = "1101011000110101001001111001010110100010110001101110010100011101";
    std::string key1 = "0110111100011010011110001000100001110001011100100111010000100010";
    std::string plaintext2 = "1101011000110101001001111001010110100010110001101110010100011100";
    std::string key2 = "0110111100011010011110001000100001110001011100100111010000100011";

    std::string ciphertext0 = des0.encrypt(plaintext1, plaintext2,  key1);
    std::string decrypted_text0 = des0.decrypt(ciphertext0, key1);
    IS_TRUE(compare_strings(plaintext1, decrypted_text0));


    std::string ciphertext1 = des1.encrypt(plaintext1, plaintext2,  key1);
    std::string decrypted_text1 = des1.decrypt(ciphertext1, key1);
    IS_TRUE(compare_strings(plaintext1, decrypted_text1));


    std::string ciphertext2 = des2.encrypt(plaintext1, plaintext2,  key1);
    std::string decrypted_text2 = des2.decrypt(ciphertext2, key1);
    IS_TRUE(compare_strings(plaintext1, decrypted_text2));
    
    std::string ciphertext3 = des3.encrypt(plaintext1, plaintext2,  key1);
    std::string decrypted_text3 = des3.decrypt(ciphertext3, key1);
    IS_TRUE(compare_strings(plaintext1, decrypted_text3));
}


int main() {
    test_encrypt_decrypt();
    
    return 0;
}