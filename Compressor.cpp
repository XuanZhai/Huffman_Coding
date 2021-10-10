#include "Compressor.h"

using namespace std;


bool cmpp(const Node* a, const Node* b){
    if(a->value == b->value){
        return a->name < b->name;
    }
    return a->value < b->value;                         // Compare and Sort the Nodes from small to large
}


Compressor::Compressor() {          // Initialize all the value
    filesize = 0;
    listsize  = 0;
    chartsize = 0;
    Inputdata = nullptr;
    CharList = nullptr;
    Transchart = nullptr;
    TreeHead = nullptr;
}


Compressor::~Compressor() {
    delete[] Inputdata;
    for(int i = 0; i < listsize; i++){
        delete CharList[i];
    }
    delete[] CharList;
    delete[] Transchart;
}


void Compressor::ReadFile(char* filename) {

    struct stat results;
    if (stat(filename, &results) != 0) {
        printf("Error in loading file\n");
        exit(1);
    }
    filesize = results.st_size;                     // Preload the size and allocate memory based on the size.
    Inputdata = new char[filesize];
    CharList = new Node*[filesize*2];        // The num of inter node should not be higher than the terminal nodes.


    ifstream inputfile(filename, ios::in | ios::binary);    // Load the file in binary mode.


     inputfile.read(Inputdata,filesize);            // Read the text into a char array.

     for(int i = 0; i < filesize; i++){
         bitset<8> trans(Inputdata[i]);         // Convert each char into an 8 bits bitset.
         string temp = trans.to_string();           // Convert it to string.
         InsertList(temp);                      // Add it to the Charlist.
     }

    sort(CharList, CharList+listsize,cmpp);      // Sort the list based on the frequency.

    inputfile.close();
}


void Compressor::InsertList(std::string& newchar) {     // pass in an string refers to an 8bits bitset
    if(listsize == 0) {                                 // If currently the list is empty
        CharList[0] = new Node(1, newchar);             // Put it to the first play.
        listsize++;
        return;
    }

    for(int i = 0; i < listsize; i++){                  // Check whether there is one already existed.
        if(CharList[i]->name == newchar) {
            CharList[i]->value++;                       // Update the frequency.
            return;
        }
    }
    Node* newNode = new Node(1,newchar);          // If it is a new one, create a node and add it to the list.
    CharList[listsize] = newNode;
    listsize++;
}


void Compressor::InsertTree() {                 // Build a huffman tree
    int current = 0;                         // Loop through all the nodes
    while(current != listsize-1){
        Node* newNode = new Node(CharList[current]->value+CharList[current+1]->value);  // Create a new node based on the two smallest
        newNode->LChild = CharList[current];
        newNode->RChild = CharList[current+1];      // Set children and parent
        CharList[current]->Parent = newNode;
        CharList[current+1]->Parent = newNode;
        current = current + 2;

        int loc = 0;                                // We need to insert that node into this sorted list, so find the loc first.
        for(loc = 0; loc < listsize; loc++){
            if(CharList[loc]->value >= newNode->value)
                break;
        }

        for(int i = listsize-1; i>=loc; i--){       // Push the items after that loc 1 index
            CharList[i+1] = CharList[i];
        }
        CharList[loc] = newNode;
        listsize++;
    }

    TreeHead = CharList[listsize-1];            // Set the largest one as the tree head
}


void Compressor::CreatePath() {                 // Create paths for each terminal nodes.
    Transchart = new Node*[filesize];

    TreeHead->visited = true;                   // Use BFS to generate the path
    Node** TreeStack = new Node* [listsize];
    TreeStack[0] = TreeHead;
    int currentsize = 1;

    while (currentsize!=0){
        Node* temp = TreeStack[currentsize-1];

        if(!temp->visited){
            if(temp == temp->Parent->LChild){
                temp->visited = true;
                temp->path = temp->Parent->path;
                temp->path.push_back(false);            // The path is a list of true/false which refers to 0/1
            }
            else if(temp == temp->Parent->RChild){
                temp->visited = true;
                temp->path = temp->Parent->path;
                temp->path.push_back(true);
            }
        }

        if(temp->name != "NULL"){                   // If the BFS reaches a leaf, store it to the chart for later interpretation
            Transchart[chartsize] = temp;
            chartsize++;
        }
        currentsize--;
        if (temp->RChild != nullptr) {
            TreeStack[currentsize] = temp->RChild;
            currentsize++;
        }
        if (temp->LChild != nullptr) {
            TreeStack[currentsize] = temp->LChild;
            currentsize++;
        }
    }
    delete[] TreeStack;
}



int Compressor::Compression() {                     // Compression and generate the compressed file.
    ofstream outputfile("compressed.txt", ios::out | ios::binary);
    if (!outputfile) {
        cout << "Error on loading output.txt" << endl;
        exit(1);
    }

    char *resulttext = new char[filesize];          // Store the result.
    int textcounter = 0;                            // Count the number of char in the result.
    bool bytebuffer[8];                             // An 8 bits buffer for converting from bool array to 1 byte char.
    int buffercounter = 0;                          // Count the current number of item in the buffer.

    for (int i = 0; i < filesize; i++) {
        for (int j = 0; j <
                        chartsize; j++) {         // For each word in the original text, look through the chart to find a match.
            bitset<8> trans(Inputdata[i]);
            string temp = trans.to_string();

            if (temp == Transchart[j]->name) {       // If there's one
                for (int k = 0;k < Transchart[j]->path.size(); k++) {        // Loop through its path and add them to the buffer.
                    bytebuffer[buffercounter] = Transchart[j]->path.at(k);
                    buffercounter++;
                    if (buffercounter ==
                        8) {                     // If the buffer is full, convert it to a one byte char.
                        resulttext[textcounter] = VbooltoByte(bytebuffer);
                        textcounter++;
                        buffercounter = 0;
                    }
                }
                break;
            }
        }
    }

    outputfile.write(resulttext, textcounter);              // Write the text to the file.
    if (buffercounter != 0) {                                 // If there are less than 1 byte in the buffer right now.
        for (int i = 0; i < buffercounter; i++) {
            char vOut[1];
            if (bytebuffer[i])
                vOut[0] = '1';
            else
                vOut[0] = '0';                              // Store each bit as a byte since you cannot store a bit into the file.
            outputfile.write(vOut, 1);
        }
    }

    outputfile.close();                         // Return the size of the compressed text,
    delete[] resulttext;
    return textcounter;                         // So that we can identify the uncompressed part at the bottom.
}


void Compressor::SaveKey() {                // Save the chart into another file.
    ofstream outputfile("key.txt");

    if(!outputfile){
        cout << "Error on loading output.txt" << endl;
        exit(1);
    }

    for(int i = 0; i < chartsize; i++){
        outputfile << "\n" << bitset<8>(Transchart[i]->name) << " " << Transchart[i]->value;
    }
    outputfile.close();
    outputfile.close();
}


// Reference:https://stackoverflow.com/questions/29123959/convert-a-char-array-of-0-and-1-to-bytes-in-c
char Compressor::VbooltoByte(bool* sequence){       // Convert a bool array to a one byte char
    unsigned char byte = 0;
    for( int i = 0; i < 8; ++i ) {
        if (sequence[i]) byte |= 1 << (7 - i);
    }
    return byte;
}