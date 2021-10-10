#ifndef HUFFMAN_CODING_DECOMPRESSOR_H
#define HUFFMAN_CODING_DECOMPRESSOR_H

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <sys/stat.h>
#include <cmath>
#include "Node.h"

class Decompressor{
    private:
        char* Outputdata;
        char* Inputdata;
        Node** CharList;
        int filesize;
        int Inputcounter;
        int Outputcounter;
        int listsize;
        Node* TreeHead;
    public:
        Decompressor();
        ~Decompressor();
        void ReadFile(int);
        void InsertTree();
        void Decompression();
        void Comparison(char*);
};

#endif //HUFFMAN_CODING_DECOMPRESSOR_H
