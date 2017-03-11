#include "GA.hpp"
// #include <webots/DistanceSensor.hpp>

void printIndividual(Individual individual) {
  std::cout << "Weights " << std::endl;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 8; j++) {
      std::cout << individual[0][j][i] << " ";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl;
}

int main(int argc, char const *argv[]) {

  GA *test = new GA;
  configLoader *fileLoader = new configLoader();

  srand(time(NULL));
  int popsize = 10;
  std::vector<Genotype> population;
  population = test->populate(popsize);

  test->printPopToFile(population);
  test->parseFile("save_file_output.xml", popsize);
  // printIndividual(population[0].individual);
  //
  // std::string filename;
  // std::cin >> filename;
  // std::map<std::string, std::string> helloooo = fileLoader->LoadConfig(filename);
  //
  // std::cout << helloooo["Weights0"];
  // std::cout << "Done" << std::endl;

  return 0;
}
