#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

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

    string* input_values = process_input("./sample_input/sample1.txt");
    for (int i = 0; i < 4; i++){
        cout << input_values[i] << endl;
    }
    
    
    return 0;
}
