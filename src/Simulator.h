#pragma once

#include "Event.h"
#include "Node.h"

#include <map>
#include <memory>
#include <queue>

class Link;

class Simulator : public std::enable_shared_from_this<Simulator> {
private:
  static const int DEFAULT_SIMULATION_END = 120;

  double current_time;
  double simulation_end_time;

  std::priority_queue<Event, std::vector<Event>, EventComparator> event_queue;
  std::map<int, std::shared_ptr<Node>> all_nodes;
  std::vector<int> all_node_addresses;
  std::vector<std::shared_ptr<Link>> all_links;

  std::map<int, int> packets_generated_by_node;
  std::map<int, int> packets_received_by_node;
  std::map<int, int> packets_dropped_by_node;

  void processEvent(const Event &event);
  void handlePacketGeneration(const Event &event);
  void handlePacketArrival(const Event &event);
  void handleTransmissionComplete(const Event &event);

public:
  Simulator();
  Simulator(double end_time);

  void addNode(std::shared_ptr<Node> node);
  void addLink(std::shared_ptr<Link> link);
  void addPacketGenerated(std::shared_ptr<Node> node);
  void addPacketReceived(std::shared_ptr<Node> node);
  void addPacketDropped(std::shared_ptr<Node> node);

  double getCurrentTime() const;
  std::shared_ptr<Node> getNodeAtAddress(int address);
  const std::vector<int> &getAllNodeAddresses() const;

  void scheduleEvent(const Event &event);
  void setup();
  void run();

  void printReport() const;
};
