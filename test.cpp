
#include <iostream>
#include "SBoxes.h"
int main() {

    // SBoxes::S1; - How to get static prop
    for(const auto& row: SBoxes::S1){
        for(int col: row){
            cout << col << " ";
        }
        cout << endl;
    }
    return 0;
}
