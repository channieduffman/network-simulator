#include "Node.h"
#include "Link.h"
#include "Packet.h"
#include "Simulator.h"

#include <iostream>
#include <memory>
#include <random>

Node::Node(int addr, std::shared_ptr<Simulator> sim, int capacity) {
  address = addr;
  simulator = sim;
  max_incoming_capacity = capacity;
}

void Node::notifyLinkFree(std::shared_ptr<Link> free_link) {
  assert(free_link != nullptr &&
         "error: Node::notifyLinkFree(): link is null!");
  processQueuedPacket();
}

void Node::setRoute(int node, std::shared_ptr<Link> link) {
  routes.insert({node, link});
}

void Node::addOutgoingLink(std::shared_ptr<Link> link) {
  outgoing_links.insert({address, link});
}

void Node::processQueuedPacket() {
  if (!incoming.empty()) {
    auto packet_to_send = incoming.front();
    assert(packet_to_send != nullptr &&
           "error: Node::processQueuedPacket(): packet is null!");

    auto it = routes.find(packet_to_send->getDestinationAddress());

    if (it == routes.end()) {
      throw std::runtime_error(
          "error: Node::processQueuedPacket(): failed to route packet!");
    }

    assert(it->second != nullptr &&
           "error: processQueuedPacket(): missing route!");

    std::shared_ptr<Link> outgoing_link = it->second;
    int to_node = it->first;

    assert(outgoing_link != nullptr &&
           "error: Node::processQueuedPacket(): outgoing link is null!");

    if (!outgoing_link->isBusy()) {
      std::cout << "ref: " << outgoing_link.use_count()
                << ", to: " << std::to_string(to_node) << std::endl;
      packet_to_send->printPacket();
      outgoing_link->startTransmission(packet_to_send);
      incoming.pop();
    }
  }
}
void Node::generatePacket(int target, double interval) {
  if (auto sim = simulator.lock()) {
    int destination;
    if (target == -1) {
      destination = getRandomIndex(sim->getAllNodeAddresses().size());
    } else {
      destination = target;
    }
    std::shared_ptr<Packet> new_packet = std::make_shared<Packet>(
        address, destination, sim->getCurrentTime(), 12000);
    incoming.push(new_packet);
    processQueuedPacket();
    Event next_event(Event::EventType::PACKET_GENERATION,
                     sim->getCurrentTime() + interval, shared_from_this(),
                     nullptr, nullptr);
    sim->scheduleEvent(next_event);
  } else {
    throw std::runtime_error(
        "error: Node::generatePacket(): simulator does not exist!");
  }
}

int Node::getRandomIndex(int len) {
  assert(len != 0 && "error: Node::getRandomIndex(): len is 0!");
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, len - 1);
  int candidate;
  do {
    candidate = distrib(gen);
  } while (candidate == address);
  return candidate;
}

void Node::receivePacket(std::shared_ptr<Packet> packet) {
  assert(packet != nullptr && "error: Node::receivePacket(): packet is null!");
  if (packet->getDestinationAddress() == address) {
    packets_received++;
  } else {
    if (incoming.size() >= max_incoming_capacity) {
      std::cout << "Buffer full; dropping packet" << std::endl;
      packets_dropped++;
    } else {
      incoming.push(packet);
      processQueuedPacket();
    }
  }
}

std::map<int, std::shared_ptr<Link>> Node::getRoutes() const { return routes; }

int Node::getAddress() const { return address; }

int Node::getPacketsReceived() const { return packets_received; }

int Node::getPacketsDropped() const { return packets_dropped; }
