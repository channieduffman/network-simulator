#pragma once

class Node {
private:
  double address;

public:
  Node();
  Node(int address);

  double getAddress() const;
};
