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
  static constexpr double INTERVAL = 0.1;

  int max_incoming_capacity;
  int address;

  std::queue<std::shared_ptr<Packet>> incoming;
  std::map<int, std::shared_ptr<Link>> outgoing_links;
  std::map<int, std::shared_ptr<Link>> routes;

  std::mt19937 random_engine;

  int getRandomAddressInt(std::shared_ptr<Simulator> sim);
  std::shared_ptr<Packet> makePacketToRandom(std::shared_ptr<Simulator> sim);

  std::weak_ptr<Simulator> simulator;

public:
  Node(int address, std::shared_ptr<Simulator> sim, int capacity);

  void generatePacket(double interval);
  void receivePacket(std::shared_ptr<Packet> packet);
  void notifyLinkFree(std::shared_ptr<Link> free_link);
  void processQueuedPacket();

  void setRoute(int node, std::shared_ptr<Link> link);
  void addOutgoingLink(std::shared_ptr<Link> link);

  double getInterval() const;
  int getAddress() const;
  std::map<int, std::shared_ptr<Link>> getRoutes() const;
};
