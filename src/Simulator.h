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

  void processEvent(const Event &event);

public:
  Simulator();
  Simulator(double end_time);

  void addNode(std::shared_ptr<Node> node);
  void addLink(std::shared_ptr<Link> link);
  void setup();

  void run();

  void scheduleEvent(const Event &event);

  double getCurrentTime() const;
  std::shared_ptr<Node> getNodeAtAddress(int address);
  const std::vector<int> &getAllNodeAddresses() const;
};
