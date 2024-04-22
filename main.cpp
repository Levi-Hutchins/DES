#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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

    if (file.is_open()) {
        for (int i = 0; i < 4 && getline(file, values[i]); ++i);
        file.close();
    } else {
        cout << "Unable to open file" << endl;
    }
    return values;
}

int main(int argc, const char * argv[]) {

    string* input_values = process_input(argv[1]);
    for (int i = 0; i < 4; i++){
        cout << input_values[i] << endl;
    }
    
    
    return 0;
}
