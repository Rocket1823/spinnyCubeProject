#include <iostream>
#include <unistd.h>
#include <cmath>
#include <math.h>
#include <vector>

#include "Cube.h"

using namespace std;

void printGraphics(vector<uint64_t> cords);

int main(){
    /**
    cout << "hello" << '\n';
    cout << "yeet";
    fflush(stdout);
    cout << "\r\x1b[A";
    cout << "hi" << endl;
    sleep(5);
    */

    /**
    cube* cube1 = new cube(5);
    cube1->printVertices();
    cout << endl;
    cube1->printEdges();
    cout << endl;
    cube1->printEdgeLengths();
    return 0;
    */
    
    vector<uint64_t> cords = {0x8000F000, 0x800D0000,0x80030050,0xF0000000};
    printGraphics(cords);
}

void printGraphics(vector<uint64_t> cords){
    for(auto row:cords){
        for(uint64_t i = 0x80000000; i > 0; i = i>>1){
            cout << (i & row ? "1" : "0");
        }
        cout << endl;
    }
}
