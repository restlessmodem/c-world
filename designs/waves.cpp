//
//  main.cpp
//  ASCII Fish
//
//  Created by Anna-Lena Graevingholt on 14.03.19.
//  Copyright © 2019 Anna-Lena Graevingholt. All rights reserved.
//

#include <iostream>
#include <list>
#include <string>

using namespace std;

int main(int argc, const char * argv[]) {
   
    int zahl = 80;
    
    string wave = "_.-.";
    
    for (int x = 0; x < zahl; x++){
        if (x%2==0){
            wave.append("_.");
        }
        else{
            wave.append("-.");
        }
    }
    
    cout << wave << endl << endl;
    
    string wave2 = "-._.";
    
    
    for (int x = 0; x < zahl; x++){
        if (x%2==0){
            wave2.append("-.");
        }
        else{
            wave2.append("_.");
        }
    }
    
    cout << wave2 << endl << endl;
    return 0;
}
