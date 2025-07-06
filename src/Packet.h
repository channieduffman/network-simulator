#pragma once

class Packet {
private:
  static constexpr double DEFAULT_SIZE_IN_BITS = 12000;

  int source_address;
  int destination_address;
  double time_created_in_seconds;
  double size_in_bits;

public:
  Packet(int source, int destination, double created,
         double size = DEFAULT_SIZE_IN_BITS);

  int getSourceAddress() const;
  int getDestinationAddress() const;
  double getSizeInBits() const;
  double getTimeCreatedInSeconds() const;

  void printPacket() const;
};
