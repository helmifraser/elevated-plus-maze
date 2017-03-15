#include "super.hpp"
#include "GA.hpp"

int main(int argc, char const *argv[]) {
  std::cout << "Run" << std::endl;
  simulationControl *controller = new simulationControl();
  GA *testGA = new GA();
  Individual clarossa = testGA->createIndividual();
  std::cout << "done" << std::endl;
  controller->run();
  delete controller;
  return 0;
}
