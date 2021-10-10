#include <iostream>
#include "Compressor.h"
#include "Decompressor.h"


int main(int argc, char** argv) {
    if(argc != 2){
        printf("Invalid command line arguments!\n"
               "The one argument should be the input file name.\n");
        exit(1);
    }

    Compressor newCompressor;
    newCompressor.ReadFile(argv[1]);
    newCompressor.InsertTree();
    newCompressor.CreatePath();
    int size = newCompressor.Compression();
    newCompressor.SaveKey();

    Decompressor newDecompressor;
    newDecompressor.ReadFile(size);
    newDecompressor.InsertTree();
    newDecompressor.Decompression();
    newDecompressor.Comparison(argv[1]);
    return 0;
}

