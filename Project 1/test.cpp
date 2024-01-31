#include "HCTree.hpp"
#include "HCTree.cpp"
#include <vector>

int main(int agrc, char* argv[]){
    vector<int> freqs(256);

    //A has freq 6
    freqs[65] = 6;
    //B
    freqs[66]=4;
    //C
    freqs[67]=4;
    //G
    freqs[71]=1;
    //H
    freqs[72]=2;

    HCTree tree = HCTree();

    tree.build(freqs);
}
