#include "Link.h"

Link::Link(std::shared_ptr<Node> source, std::shared_ptr<Node> destination,
           double latency, double bandwidth, std::shared_ptr<Simulator> sim) {
  source_node = source;
  destination_node = destination;
  latency_in_ms = latency;
  bandwidth_in_Mbps = bandwidth;
  simulator = sim;
  is_busy = false;
}
