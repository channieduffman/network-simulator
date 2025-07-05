#pragma once
#include "Node.h"
#include "Packet.h"

#include <memory>

struct Event {
  enum class EventType {
    PACKET_GENERATION,
    PACKET_ARRIVAL,
    PACKET_DEPARTURE,
  };

  double event_time;
  EventType event_type;

  std::shared_ptr<Node> target_node;
  std::shared_ptr<Packet> associated_packet;

  Event(double time, EventType type) : event_time(time), event_type(type) {};
};

struct EventComparator {
  bool operator()(const Event &a, const Event &b) const {
    return a.event_time > b.event_time;
  }
};
