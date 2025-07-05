#pragma once

class Packet {
private:
  double size_in_bytes;

public:
  Packet();

  double getSizeInBytes() const;
};
