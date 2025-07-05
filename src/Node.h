#pragma once

#include "Packet.h"

#include <map>
#include <memory>
#include <queue>
#include <vector>

class Simulator;
class Link;

class Node {
private:
  double address;

  std::queue<Packet> incoming;
  std::queue<Packet> outgoing;

  std::map<int, std::shared_ptr<Link>> routes;

  std::weak_ptr<Simulator> simulator;

public:
  Node();
  Node(int address);

  void notifyLinkFree(std::shared_ptr<Link> free_link);
  double getAddress() const;
};
