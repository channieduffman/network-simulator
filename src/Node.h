#pragma once

#include "Packet.h"

#include <map>
#include <memory>
#include <queue>
#include <random>
#include <vector>

class Simulator;
class Link;

class Node : public std::enable_shared_from_this<Node> {
private:
  int max_incoming_capacity;
  int address;

  int packets_received;
  int packets_dropped;

  std::queue<std::shared_ptr<Packet>> incoming;
  std::map<int, std::shared_ptr<Link>> outgoing_links;

  std::map<int, std::shared_ptr<Link>> routes;

  std::mt19937 random_engine;

  std::weak_ptr<Simulator> simulator;

  int getRandomAddress(int source_address);

public:
  Node(int address, std::shared_ptr<Simulator> sim, int capacity = 3);

  void generatePacket(int target = -1, double interval = 0.02);
  void receivePacket(std::shared_ptr<Packet> packet);
  void notifyLinkFree(std::shared_ptr<Link> free_link);
  void processQueuedPacket();

  void setRoute(int node, std::shared_ptr<Link> link);
  void addOutgoingLink(std::shared_ptr<Link> link);

  int getAddress() const;
  int getPacketsReceived() const;
  int getPacketsDropped() const;
  std::map<int, std::shared_ptr<Link>> getRoutes() const;
};
