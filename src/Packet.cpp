#include "Packet.h"

Packet::Packet(int source, int destination, double created, double size) {
  source_address = source;
  destination_address = destination;
  time_created_in_seconds = created;
  size_in_bits = size;
}

int Packet::getSourceAddress() const { return source_address; }

int Packet::getDestinationAddress() const { return destination_address; }

double Packet::getSizeInBits() const { return size_in_bits; }

double Packet::getTimeCreatedInSeconds() const {
  return time_created_in_seconds;
}
