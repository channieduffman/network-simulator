#pragma once

#include "Link.h"
#include "Node.h"
#include "Packet.h"

#include <memory>

struct Event {
  enum class EventType {
    PACKET_GENERATION,
    PACKET_ARRIVAL,
    TRANSMISSION_COMPLETE,
  };

  EventType event_type;
  double event_time;

  std::shared_ptr<Node> target_node;
  std::shared_ptr<Link> target_link;
  std::shared_ptr<Packet> associated_packet;

  std::string getTypeString();

  Event(EventType type, double time, std::shared_ptr<Node> node = nullptr,
        std::shared_ptr<Link> link = nullptr,
        std::shared_ptr<Packet> packet = nullptr);
};

struct EventComparator {
  bool operator()(const Event &a, const Event &b) const {
    return a.event_time > b.event_time;
  }
};
