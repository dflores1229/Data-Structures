#include "HCTree.hpp"
#include "Helper.hpp"
#include <vector>
#include <iostream>
const int freqSz = 256;
const bool debug = false;

/*
 * argc provides the number of command line arguments that were provided
 * argv variable is an array containing the command line arguments
 *
 * The compress program will take as input an arbitrary file (original_file)
 *  and will use Huffman Compression to create a compressed version of the
 *  file (compressed_file):
 *  Construct a Huffman code for the contents of original_file
 *  Use the constructed Huffman code to encode original_file
 *  Write the results (with a header to allow for uncompression) to compressed_file
 */
int main( int argc, char* argv[] ){

    //check if enough arguments were provided
    if( argc < 3){
        error("Incorrect Parameters");
    }

    if(debug){
      cerr << "Opening input file and output file\n";  
    }
    //open input file
    FancyInputStream in(argv[1]);
    //open output file
    FancyOutputStream out(argv[2]);

    
    if(debug){
      cerr <<"Checking if file exists\n"; 
    }
    //check if a file exists
    if( !in.good() ){
        error("Failed to open file");
    }

    //store file size
    int fileSz = in.filesize();
    
    if(debug){
        cerr << "Checking if file is empty\n";
    }
    //Base Case:if file is empty, compress to empty file
    if(fileSz == 0){
       return 0;
    }

    //vector will store frequency of each symbol
    vector<int> freqs(freqSz, 0);

    if(debug){
        cerr << "Reading first byte from input file\n";
    }
    //read next byte from input file
    int currByte = in.read_byte();

    //while haven't reached end of file
    while( currByte != -1 ){
        //increase number of occurrences of current byte value
        freqs[currByte] += 1;

        if(debug){
            cerr << "Reading next byte from input file\n";
        }  
        //read next byte from input file
        currByte = in.read_byte();
    }

    if(debug){
        cerr << "Creating Huffman Coding Tree\n";
    }
    //use byte count to create a Huffman coding tree
    HCTree tree = HCTree();
    tree.build(freqs);

    //write header to compressed file for decoding:
    //for the **naive** header encoding you write out 256 integers
    //where each of these integers represent the frequency
    //that they appeared in the original file

    if(debug){
        cerr << "Writing header to compressed file\n";
    }
    
    //write size of original file
    out.write_byte(fileSz);

    //iterate over freq vector and write every frequency
    for(int i=0; i< freqSz; i++){
        out.write_int(freqs[i]);
    }

    if(debug){
        cerr << "Going back to top of file\n";
    }
    //go back to top of file
    in.reset();

    //read next byte from input file
    currByte = in.read_byte();

    //while haven't reached end of file
    while( currByte != -1 ){
        if(debug){
            cerr << "Encoding next byte from file\n";
        }
        //encode and write current byte using tree
        tree.encode(currByte, out);
    }

    //input and output files are closed

    if(debug){
        cerr << "Successfully compressed\n";
    }
    //indicate success
    return 0;
}

