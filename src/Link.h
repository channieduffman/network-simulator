#pragma once

#include "Node.h"
#include "Packet.h"

#include <memory>

class Simulator;

class Link {
private:
  std::shared_ptr<Node> source_node;
  std::shared_ptr<Node> destination_node;

  double latency_in_ms;
  double bandwidth_in_Mbps;

  std::shared_ptr<Simulator> simulator;

  bool is_busy;

public:
  Link(std::shared_ptr<Node> source, std::shared_ptr<Node> destination,
       double latency, double bandwidth, std::shared_ptr<Simulator> sim);

  double calculateTransmissionDelay(std::shared_ptr<Packet> packet);

  void handlePacketDeparture();
};
