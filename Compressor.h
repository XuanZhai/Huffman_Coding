#ifndef HUFFMAN_CODING_COMPRESSOR_H
#define HUFFMAN_CODING_COMPRESSOR_H

#include "Node.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <sys/stat.h>


class Compressor{
    private:
        char* Inputdata;
        Node** CharList;
        Node** Transchart;
        Node* TreeHead;
        int filesize;
        int listsize;
        int chartsize;


    public:
        Compressor();
        ~Compressor();
        void ReadFile(char*);
        void InsertList(std::string&);
        void InsertTree();
        void CreatePath();
        int Compression();
        void SaveKey();
        char VbooltoByte(bool*);
};


#endif //HUFFMAN_CODING_COMPRESSOR_H
