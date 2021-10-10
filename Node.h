#ifndef HUFFMAN_CODING_NODE_H
#define HUFFMAN_CODING_NODE_H

#include <iostream>
#include <string>
#include <vector>

class Node{
    private:

    public:
        int value;                                         // Here I put all the elements into public, it's convenient
        std::string name;                                  // but not optimal. I can change that as further improvements
        bool visited;
        Node* Parent;
        Node* LChild;
        Node* RChild;
        std::vector<bool> path;

        Node();
        Node(const Node&);
        Node(int,std::string);                             // Load name and value as a leaves
        Node(int);                                         // Load just value as branches
        Node operator= (const Node&);
        bool operator<(const Node& rhs) const;             // overloaded less than operator
        bool operator>(const Node& rhs) const;             // overloaded greater than operator
        bool operator==(const Node& rhs) const;            // overloaded equal to operator


};


#endif //HUFFMAN_CODING_NODE_H
