#include "Event.h"

#include <string>

Event::Event(EventType type, double time, std::shared_ptr<Node> node,
             std::shared_ptr<Link> link, std::shared_ptr<Packet> packet) {
  event_type = type;
  event_time = time;
  target_node = node;
  target_link = link;
  associated_packet = packet;
}

std::string Event::getTypeString() {
  std::string repr;
  switch (event_type) {
  case Event::EventType::PACKET_GENERATION:
    repr = "PACKET_GENERATION";
    break;
  case Event::EventType::PACKET_ARRIVAL:
    repr = "PACKET_ARRIVAL";
    break;
  case Event::EventType::TRANSMISSION_COMPLETE:
    repr = "TRANSMISSION_COMPLETE";
    break;
  }
  return repr;
}
