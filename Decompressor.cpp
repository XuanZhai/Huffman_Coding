#include "Decompressor.h"

using namespace std;


bool cmp(const Node* a, const Node* b){
    if(a->value == b->value){
        return a->name < b->name;
    }
    return a->value < b->value;                         // Compare and Sort the Nodes from small the large
}


Decompressor::Decompressor(){
    struct stat results;
    struct stat results2;
    if (stat("compressed.txt", &results) != 0) {    // Load the size of two files.
        printf("Error in loading file\n");
        exit(1);
    }

    if (stat("key.txt", &results2) != 0) {
        printf("Error in loading file\n");
        exit(1);
    }

    filesize = results.st_size;
    Inputdata = new char[filesize*8];
    Outputdata = new char[filesize*8];
    CharList = new Node*[results2.st_size*2];
    Inputcounter = 0;
    Outputcounter = 0;
    listsize = 0;
    TreeHead = nullptr;
}


Decompressor::~Decompressor(){
    delete[] Outputdata;
    delete[] Inputdata;
    for(int i = 0; i < listsize; i++){
        delete CharList[i];
    }
    delete[] CharList;
}


void Decompressor::ReadFile(int newsize) {
     ifstream inputfile("compressed.txt", ios::in | ios::binary);
     if(!inputfile){
         cout << "Error on loading resource.txt" << endl;
         exit(1);
     }

     char* buffer = new char[newsize];
     inputfile.read(buffer,newsize);            // Read the compressed text to a buffer.

     for(int i = 0; i < newsize; i++){
        bitset<8> trans(buffer[i]);
        string temp = trans.to_string();        // Convert each byte to an 8bit bitset and store the sequence to an array.
        for (char & it : temp) {
            Inputdata[Inputcounter] = it;
            Inputcounter++;
        }
    }

    char* extrabuffer = new char[filesize-newsize];     // Use the extra buffer to store the uncompressed part
    inputfile.read(extrabuffer,(filesize-newsize));
    for(int i = 0; i < filesize-newsize; i++){
        Inputdata[Inputcounter] = extrabuffer[i];
        Inputcounter++;
    }
    inputfile.close();

    ifstream inputkey("key.txt");                   // Load the key
    if(!inputkey){
        cout << "Error on loading output.txt" << endl;
        exit(1);
    }

    int test = 0;
    string name; int frequency;
    getline(inputkey,name);
    while (!inputkey.eof()){
        inputkey >> name;
        inputkey >> frequency;
        Node* newNode = new Node(frequency,name);           // Read name and frequency, make a node for each pair
        CharList[listsize] = newNode;
        listsize++;
        test ++;
    }
    inputkey.close();
    sort(CharList, CharList+listsize,cmp);              // Sort the list
    delete[] buffer;
    delete[] extrabuffer;
}


void Decompressor::InsertTree() {             // Build the huffman tree, same concept as the one in compressor.
    int current = 0;                         // Loop through all the nodes
    while (current != listsize - 1) {
        Node *newNode = new Node(CharList[current]->value + CharList[current + 1]->value);
        newNode->LChild = CharList[current];
        newNode->RChild = CharList[current + 1];
        CharList[current]->Parent = newNode;
        CharList[current + 1]->Parent = newNode;
        current = current + 2;

        int loc = 0;
        for (loc = 0; loc < listsize; loc++) {
            if (CharList[loc]->value >= newNode->value)
                break;
        }

        for (int i = listsize - 1; i >= loc; i--) {
            CharList[i + 1] = CharList[i];
        }
        CharList[loc] = newNode;
        listsize++;
    }

    TreeHead = CharList[listsize - 1];
}


void Decompressor::Decompression() {        // Decompress the text based on the tree
    Node* temp = TreeHead;
    for(int i = 0; i < Inputcounter; i++){
        if(Inputdata[i] == '0'){        // Walk through the tree.
            temp = temp->LChild;
        }
        else{
            temp = temp->RChild;
        }


        if(temp->name != "NULL"){           // If reaches a leaf.
             string newtemp = temp->name;
             char c = '\0';
             for(int j = 0; j < 8; j++){           // Change the name from a binary sequence to a one byte char
                 if(newtemp.at(j) == '1'){
                     c = (c << 1)|1;
                 }
                 else{
                     c = c << 1;
                 }
             }
             Outputdata[Outputcounter] = c;     // Add that char to the output array.
             Outputcounter++;
             temp = TreeHead;
        }
    }


    ofstream Outputfile("Decompressed.txt");
    if(!Outputfile){
        cout << "Unable to load Decompressed.txt" << endl;
    }

    Outputfile << Outputdata;
    Outputfile.close();
}


void Decompressor::Comparison(char* filename) {            // Compare the old one with the compression/decompression one
    struct stat results;
    if (stat(filename, &results) != 0) {
        printf("Error in loading file\n");
        exit(1);
    }

    char* testinput = new char[results.st_size];
    ifstream inputfile(filename, ios::in | ios::binary);    // Load the file in binary mode.

    inputfile.read(testinput,results.st_size);            // Read the text into a char array.

    int num = 0;
    int denom = 0;
    int ncompare = 0;
    if(results.st_size > Outputcounter) {
        denom = results.st_size;                        // The larger number should be the denominator
        ncompare = Outputcounter;                       // The smaller one should be the number of comparison to avoid out_of_range issue.
    }
    else {
        denom = Outputcounter;
        ncompare = results.st_size;
    }

    for(int i = 0; i < ncompare; i++){
        if(testinput[i] == Outputdata[i]){
            num++;
        }
    }

    cout << "Accuracy is "<< ((double)num/(double)denom*100) << "%" << endl;
    delete[] testinput;
}