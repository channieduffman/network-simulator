#include "Event.h"

Event::Event(EventType type, double time, std::shared_ptr<Node> node,
             std::shared_ptr<Link> link, std::shared_ptr<Packet> packet) {
  event_type = type;
  event_time = time;
  target_node = node;
  target_link = link;
  associated_packet = packet;
}
