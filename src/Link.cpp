#include "Link.h"
#include "Event.h"
#include "Simulator.h"

#include <cassert>
#include <memory>

Link::Link(std::shared_ptr<Node> source, std::shared_ptr<Node> destination,
           double latency, double bandwidth, std::shared_ptr<Simulator> sim) {
  source_node = source;
  destination_node = destination;
  latency_in_ms = latency;
  bandwidth_in_Mbps = bandwidth;
  simulator = sim;
  is_busy = false;
}

void Link::completeTransmission(std::shared_ptr<Packet> packet) {
  assert(packet != nullptr &&
         "Link::completeTransmission called with null packet");

  is_busy = false;

  if (source_node) {
    source_node->notifyLinkFree(shared_from_this());
  } else {
    throw std::runtime_error(
        "error: Link::completeTransmission: this link has no source node");
  }

  double propagation_delay = getPropagationDelayInSeconds();

  if (auto sim = simulator.lock()) {
    Event next_event(Event::EventType::PACKET_ARRIVAL,
                     sim->getCurrentTime() + propagation_delay,
                     destination_node, nullptr, packet);
    sim->scheduleEvent(next_event);
  } else {
    throw std::runtime_error(
        "error: Link::completeTransmission: the simulator does not exist");
  }
}

double
Link::getTransmissionDelayInSeconds(std::shared_ptr<Packet> packet) const {
  assert(packet != nullptr &&
         "Link::getTransmissionDelayInSeconds called with null packet");
  double bandwidth_in_bps = bandwidth_in_Mbps * 1000000;
  return packet->getSizeInBits() / bandwidth_in_bps;
}

double Link::getPropagationDelayInSeconds() const {
  return latency_in_ms / 1000;
}
