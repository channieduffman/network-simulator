#include "Simulator.h"

#include <iostream>
#include <memory>

Simulator::Simulator() {
  current_time = 0.0;
  simulation_end_time = DEFAULT_SIMULATION_END;
  while (!event_queue.empty()) {
    event_queue.pop();
  }
}

Simulator::Simulator(double end_time) {
  current_time = 0.0;
  simulation_end_time = end_time;
  while (!event_queue.empty()) {
    event_queue.pop();
  }
}

void Simulator::processEvent(const Event &event) {
  switch (event.event_type) {
  case Event::EventType::PACKET_GENERATION:
    break;
  case Event::EventType::PACKET_ARRIVAL:
    break;
  case Event::EventType::TRANSMISSION_COMPLETE:
    break;
  }
}

void Simulator::addNode(std::shared_ptr<Node> node) {
  assert(node != nullptr && "error: Simulator::addNode(): node is null!");
  int address = node->getAddress();
  auto [it, inserted] = all_nodes.insert({address, node});
  if (!inserted) {
    throw std::runtime_error("error: Simulator::addNode(): duplicate key `" +
                             std::to_string(address) + "`!");
  }
}

void Simulator::addLink(std::shared_ptr<Link> link) {
  assert(link != nullptr && "error: Simulator::addLink(): link is null!");
  all_links.push_back(link);
}

void Simulator::setup() {
  try {
    /* TODO
     * Make setup configurable
     * Allow for variable topologies
     */
    std::shared_ptr<Node> node0 = std::make_shared<Node>(0, shared_from_this());
    std::shared_ptr<Node> node1 = std::make_shared<Node>(1, shared_from_this());
    std::shared_ptr<Node> node2 = std::make_shared<Node>(2, shared_from_this());
    std::shared_ptr<Node> node3 = std::make_shared<Node>(3, shared_from_this());

    addNode(node0);
    addNode(node0);
    addNode(node0);
    addNode(node0);

    std::shared_ptr<Link> link01 = std::make_shared<Link>(node0, node1, 1, 100);
    std::shared_ptr<Link> link12 = std::make_shared<Link>(node1, node2, 1, 500);
    std::shared_ptr<Link> link23 = std::make_shared<Link>(node2, node3, 1, 900);

    addLink(link01);
    addLink(link12);
    addLink(link23);
  } catch (const std::exception &e) {
    std::cout << "error: " << e.what() << std::endl;
  }
}

void Simulator::run() {}

void Simulator::scheduleEvent(const Event &event) { event_queue.push(event); }

double Simulator::getCurrentTime() const { return current_time; }

std::shared_ptr<Node> Simulator::getNodeAtAddress(int address) {
  std::shared_ptr<Node> node = nullptr;
  if (auto it = all_nodes.find(address); it != all_nodes.end()) {
    node = it->second;
  }
  return node;
}

const std::vector<int> &Simulator::getAllNodeAddresses() const {
  return all_node_addresses;
}
