#include "HCTree.hpp"
#include "Helper.hpp"
#include <vector>
const int freqSz = 256;

int main(int argc, char* argv[] ){

    //check if enough arguments were provided
    if( argc < 3){
        error("Incorrect Parameters");
    }

    //open input file
    FancyInputStream in(argv[1]);
    //open output file
    FancyOutputStream out(argv[2]);

    //check if a file exists
    if( !in.good() ){
        error("Failed to open file");
    }

    //store size of compressed input file
    int inFileSz = in.filesize();

    //Base Case:if file is empty, uncompress to empty file
    if(inFileSz == 0){
       return 0;
    }

    //store size of original uncompressed file (first byte holds size of original file)
    int UCfileSz = in.read_byte();

    //vector will store frequency of each symbol
    vector<int> freqs(freqSz, 0);

    //Read the file header at the beginning of the input file:
    //read in freq for 256 ints
    for(int i=0; i < freqSz; i++){
        int currInt = in.read_int();
        freqs[i] = currInt;
    }

    //use freq count to reconstruct the Huffman coding tree
    HCTree tree = HCTree();
    tree.build(freqs);

    //tracks how many bits of the original file are left to decode
    int count = UCfileSz;

    //Using the Huffman coding tree, decode the bits from the input file
    while( count > 0){
        unsigned char currSymb = tree.decode(in);
        //write symbol to output file
        out.write_byte(currSymb);
        //decrease to indicate we have decoded 1 bit
        count--;
    }

    //input and output files are closed

    //indicate success
    return 0;
}

