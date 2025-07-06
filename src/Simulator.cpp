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
    handlePacketGeneration(event);
    break;
  case Event::EventType::PACKET_ARRIVAL:
    handlePacketArrival(event);
    break;
  case Event::EventType::TRANSMISSION_COMPLETE:
    handleTransmissionComplete(event);
    break;
  }
}

void Simulator::handlePacketGeneration(const Event &event) {
  std::shared_ptr<Node> node = event.target_node;
  node->generatePacket();
}

void Simulator::handlePacketArrival(const Event &event) {
  std::shared_ptr<Node> node = event.target_node;
  node->receivePacket(event.associated_packet);
}

void Simulator::handleTransmissionComplete(const Event &event) {
  std::shared_ptr<Link> link = event.target_link;
  link->completeTransmission(event.associated_packet);
}

void Simulator::addNode(std::shared_ptr<Node> node) {
  assert(node != nullptr && "error: Simulator::addNode(): node is null!");
  int address = node->getAddress();
  auto [it, inserted] = all_nodes.insert({address, node});
  if (!inserted) {
    throw std::runtime_error("error: Simulator::addNode(): duplicate key `" +
                             std::to_string(address) + "`!");
  }
  all_node_addresses.push_back(address);
}

void Simulator::addLink(std::shared_ptr<Link> link) {
  assert(link != nullptr && "error: Simulator::addLink(): link is null!");
  std::cout << "adding link: " << std::to_string(link->getSourceNode()) << "->"
            << std::to_string(link->getDestinationNode()) << std::endl;
  all_links.push_back(link);
}

void Simulator::setup() {
  try {
    /* TODO
     * Make setup configurable
     * Allow for variable topologies
     * Implement a simple protocol
     */
    std::shared_ptr<Node> node0 = std::make_shared<Node>(0, shared_from_this());
    std::shared_ptr<Node> node1 = std::make_shared<Node>(1, shared_from_this());
    std::shared_ptr<Node> node2 = std::make_shared<Node>(2, shared_from_this());
    std::shared_ptr<Node> node3 = std::make_shared<Node>(3, shared_from_this());

    std::shared_ptr<Link> link01 =
        std::make_shared<Link>(node0, node1, 1, 100, shared_from_this());
    std::shared_ptr<Link> link10 =
        std::make_shared<Link>(node1, node0, 1, 100, shared_from_this());
    std::shared_ptr<Link> link12 =
        std::make_shared<Link>(node1, node2, 1, 500, shared_from_this());
    std::shared_ptr<Link> link21 =
        std::make_shared<Link>(node2, node1, 1, 500, shared_from_this());
    std::shared_ptr<Link> link23 =
        std::make_shared<Link>(node2, node3, 1, 900, shared_from_this());
    std::shared_ptr<Link> link32 =
        std::make_shared<Link>(node3, node2, 1, 900, shared_from_this());

    node0->addOutgoingLink(link01);

    node1->addOutgoingLink(link10);
    node1->addOutgoingLink(link12);

    node2->addOutgoingLink(link21);
    node2->addOutgoingLink(link23);

    node3->addOutgoingLink(link32);

    node0->setRoute(node1->getAddress(), link01);
    node0->setRoute(node2->getAddress(), link01);
    node0->setRoute(node3->getAddress(), link01);

    node1->setRoute(node0->getAddress(), link10);
    node1->setRoute(node2->getAddress(), link12);
    node1->setRoute(node3->getAddress(), link12);

    node2->setRoute(node0->getAddress(), link21);
    node2->setRoute(node1->getAddress(), link21);
    node2->setRoute(node3->getAddress(), link23);

    node3->setRoute(node0->getAddress(), link32);
    node3->setRoute(node1->getAddress(), link32);
    node3->setRoute(node2->getAddress(), link32);

    addNode(node0);
    addNode(node1);
    addNode(node2);
    addNode(node3);

    addLink(link01);
    addLink(link10);
    addLink(link12);
    addLink(link21);
    addLink(link23);
    addLink(link32);

    for (auto link : all_links) {
      link->printLink();
    }

    for (const auto &pair : node0->getRoutes()) {
      std::cout << pair.first << ", ";
      pair.second->printLink();
    }

    node0->generatePacket(node3->getAddress(), 0.5);
    node1->generatePacket(node2->getAddress(), 0.2);

  } catch (const std::exception &e) {
    std::cout << "error: " << e.what() << std::endl;
  }
}

void Simulator::run() {
  std::cout << "Simulation started: " << current_time << " --- " << std::endl;
  while (!event_queue.empty() && current_time < simulation_end_time) {
    Event next_event = event_queue.top();
    event_queue.pop();
    current_time = next_event.event_time;
    std::cout << "Processing " << next_event.getTypeString() << " at "
              << current_time << std::endl;
    processEvent(next_event);
  }
  std::cout << "Simulation ended: " << current_time << " --- " << std::endl;
}

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

void Simulator::printReport() const {
  int total_packets_received = 0;
  int total_packets_dropped = 0;
  for (auto node : all_nodes) {
    total_packets_received += node.second->getPacketsReceived();
    total_packets_dropped += node.second->getPacketsDropped();
  }

  std::cout << "Packets Delivered:\t" << total_packets_received << std::endl;
  std::cout << "Packets Dropped:\t" << total_packets_dropped << std::endl;
  std::cout << "==================================" << std::endl;
  std::cout << "In " << current_time << " seconds." << std::endl;
}
