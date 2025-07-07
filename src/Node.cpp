#include "Node.h"
#include "Link.h"
#include "Packet.h"
#include "Simulator.h"

#include <memory>
#include <random>

Node::Node(int addr, std::shared_ptr<Simulator> sim, int capacity)
    : address(addr), simulator(sim), max_incoming_capacity(capacity),
      random_engine(std::random_device()()) {}

int Node::getRandomAddressInt(std::shared_ptr<Simulator> sim) {
  int range = sim->getAllNodeAddresses().size();
  std::uniform_int_distribution<> distribution(0, range - 1);
  int random_address;
  do {
    int random_index = distribution(random_engine);
    random_address = sim->getAllNodeAddresses().at(random_index);
  } while (random_address == address);
  return random_address;
}

std::shared_ptr<Packet>
Node::makePacketToRandom(std::shared_ptr<Simulator> sim) {
  int random_address_int = getRandomAddressInt(sim);
  std::shared_ptr<Packet> packet = std::make_shared<Packet>(
      address, random_address_int, sim->getCurrentTime(), 12000);
  return packet;
}

void Node::generatePacket(double interval) {
  if (auto sim = simulator.lock()) {

    std::shared_ptr<Packet> new_packet = makePacketToRandom(sim);
    incoming.push(new_packet);
    processQueuedPacket();
    sim->addPacketGenerated(shared_from_this());
    double next_event_time = sim->getCurrentTime() + interval;
    Event next_event(Event::EventType::PACKET_GENERATION, next_event_time,
                     shared_from_this(), nullptr, nullptr);
    sim->scheduleEvent(next_event);
  } else {
    throw std::runtime_error(
        "error: Node::generatePacket(): simulator does not exist!");
  }
}

void Node::receivePacket(std::shared_ptr<Packet> packet) {
  assert(packet != nullptr && "error: Node::receivePacket(): packet is null!");
  if (auto sim = simulator.lock()) {
    if (packet->getDestinationAddress() == address) {
      sim->addPacketReceived(shared_from_this());
    } else if (incoming.size() >= max_incoming_capacity) {
      sim->addPacketDropped(shared_from_this());
    } else {
      incoming.push(packet);
      processQueuedPacket();
    }
  } else {
    throw std::runtime_error("error: Node::receivePacket(): no simulator!");
  }
}

void Node::notifyLinkFree(std::shared_ptr<Link> free_link) {
  assert(free_link != nullptr &&
         "error: Node::notifyLinkFree(): link is null!");
  processQueuedPacket();
}

void Node::processQueuedPacket() {
  if (!incoming.empty()) {
    auto packet_to_send = incoming.front();
    assert(packet_to_send != nullptr &&
           "error: Node::processQueuedPacket(): packet is null!");
    auto it = routes.find(packet_to_send->getDestinationAddress());
    assert(it != routes.end() &&
           "error: Node::processQueuedPacket(): no route found!");
    std::shared_ptr<Link> outgoing_link = it->second;
    if (outgoing_link->isBusy()) {
      incoming.push(packet_to_send);
    } else {
      incoming.pop();
      outgoing_link->startTransmission(packet_to_send);
    }
  }
}

void Node::setRoute(int node, std::shared_ptr<Link> link) {
  routes.insert({node, link});
}

void Node::addOutgoingLink(std::shared_ptr<Link> link) {
  outgoing_links.insert({address, link});
}

double Node::getInterval() const { return INTERVAL; }

int Node::getAddress() const { return address; }

std::map<int, std::shared_ptr<Link>> Node::getRoutes() const { return routes; }
