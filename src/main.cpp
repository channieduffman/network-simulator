#include "Simulator.h"

#include <iostream>

int main() {
  std::shared_ptr<Simulator> sim;
  try {
    sim = std::make_shared<Simulator>(120);
    sim->setup();
    sim->run();
    sim->printReport();
  } catch (const std::exception &ex) {
    std::cerr << "error: " << ex.what() << std::endl;
  }
  return 0;
}
