#include "GA.hpp"
// #include <webots/DistanceSensor.hpp>

void printIndividual(std::vector<std::vector<std::vector<float>>> individual){
  std::cout << "Weights " << std::endl;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 8; j++) {
      std::cout << individual[0][j][i] << " ";
    }
    std::cout  << std::endl;
  }

  std::cout << std::endl;
}

int main(int argc, char const *argv[]) {

  GA *test = new GA;
  srand(time(NULL));
  std::vector<std::vector<std::vector<float>>> parentA = test->createIndividual();
  std::vector<std::vector<std::vector<float>>> parentB = test->createIndividual();
  std::cout << "here 2" << std::endl;
  std::vector<std::vector<std::vector<float>>> child = test->child(parentA, parentB);
  std::cout << "Created child" << std::endl;

  printIndividual(parentA);
  printIndividual(parentB);
  printIndividual(child);


  return 0;
}
