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
  std::cout << "-----------------------------------------------------------"
            << std::endl;
}

void printAll(Individual individual) {
  for (int z = 0; z < individual.size(); z++) {
    for (int x = 0; x < individual[z].size(); x++) {
      /* code */
      for (int c = 0; c < individual[z][x].size(); c++) {
        /* code */
        std::cout << " " << individual[z][x][c] << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
}

int main(int argc, char const *argv[]) {

  GA *test = new GA;

  srand(time(NULL));
  int popsize = 10;
  float muteRate = 1;
  std::vector<Genotype> population;
  population = test->populate(popsize);

  test->printPopToFile(population);

  test->mutateGen(population, muteRate, 1);

  // std::vector<std::string> fileOut = test->parseFile("weights.xml", popsize);
  // Individual testOut = test->returnFileWeights(fileOut);
  // printAll(testOut);
  // std::cout << "testOut " << testOut.size() << std::endl;
  // printAll(test->returnBestWeights(testOut));

  // printIndividual(population[0].individual);
  //
  // std::string filename;
  // std::cin >> filename;
  // std::map<std::string, std::string> helloooo =
  // fileLoader->LoadConfig(filename);
  //
  // std::cout << helloooo["Weights0"];
  // std::cout << "Done" << std::endl;

  return 0;
}
