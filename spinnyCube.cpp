#include <iostream>
#include <unistd.h>
#include <cmath>
#include <math.h>
#include <vector>
#include <chrono>
#include <thread>

#include "Cube.h"

using namespace std;

void printGraphics(vector<uint64_t> cords);

int main(){
    /**
    fflush(stdout);
    cout << "\r\x1b[A";
    cout << "hi" << endl;
    sleep(5);
    */

    cube* cube2 = new cube(10);
    while(true){
        printGraphics(cube2->genereateGraphics(64));
        cube2->rotate(0, 1.0, 0);
        for(int i = 0; i<64; i++)
            cout << "\r\x1b[A";
        this_thread::sleep_for(chrono::milliseconds(25));
    }
}

void printGraphics(vector<uint64_t> cords){
    for(auto row = cords.rbegin(); row!=cords.rend(); ++row){
        for(uint64_t i = 0x8000000000000000; i > 0; i = i>>1){
            cout << (i & *row ? "0" : " ");
        }
        cout << endl;
    }
    fflush(stdout);
}
