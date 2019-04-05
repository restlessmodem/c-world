#include <string>
#include <iostream>
using namespace std;

int main(int argc, const char * argv[]) {
    
    int zahl = 80;
    
    string sand = ".";
    
    for (int x = 0; x < zahl; x++){
        sand.append(".");
    }
    cout << sand << endl << endl;
}
