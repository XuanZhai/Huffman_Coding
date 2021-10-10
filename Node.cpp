#include "Node.h"

using namespace std;

Node::Node(){
    value = 0;
    name = "NULL";
    visited = false;
    Parent = nullptr;
    LChild = nullptr;
    RChild = nullptr;
}

Node::Node(const Node& newNode){
    value = newNode.value;
    name = newNode.name;
    path = newNode.path;
    Parent = newNode.Parent;
    LChild = newNode.LChild;
    RChild = newNode.RChild;
    visited = newNode.visited;
}

Node::Node(int newValue, std::string newName){
    value = newValue;
    name = newName;
    visited = false;
    Parent = nullptr;
    LChild = nullptr;
    RChild = nullptr;
}

Node::Node(int newValue){
    value = newValue;
    name = "NULL";
    visited = false;
    Parent = nullptr;
    LChild = nullptr;
    RChild = nullptr;
}

Node Node::operator= (const Node& newNode){
    value = newNode.value;
    name = newNode.name;
    path = newNode.path;
    Parent = newNode.Parent;
    LChild = newNode.LChild;
    RChild = newNode.RChild;
    visited = newNode.visited;
    return *this;
}

bool Node::operator<(const Node& rhs) const{
    if(value < rhs.value){
        return true;
    }
    else if(value == rhs.value && name < rhs.name){
        return true;
    }
    else{
        return false;
    }
}

bool Node::operator>(const Node& rhs) const{
    if(value > rhs.value){
        return true;
    }
    else if(value == rhs.value && name > rhs.name){
        return true;
    }
    else{
        return false;
    }
}

bool Node::operator==(const Node& rhs) const{
    if(value == rhs.value && name == rhs.name){
        return true;
    }
    else{
        return false;
    }
}