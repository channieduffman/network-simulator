#include "Node.h"

Node::Node() { this->address = 0; }

Node::Node(int address) { this->address = address; }

double Node::getAddress() const { return this->address; }
