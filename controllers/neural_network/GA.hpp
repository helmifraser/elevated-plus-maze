
#include "pugixml.hpp"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <thread>
#include <sstream>


#define TIME_STEP 32
#define PS_THRESHOLD 150

using Individual = std::vector<std::vector<std::vector<float>>>;

struct Genotype {
  Individual individual;
  float fitness;
};

using Population = std::vector<Genotype>;

class GA {
private:
  int previousPosition;
  int allPositions[15];
  std::vector<float> createRow(int parameters[3]);
  void mutate(Individual &individual, float severity);

public:
  Population populate(int popsize);
  std::map<int, int> points();
  Individual createIndividual();

  void mutateGen(Population &population, float mutateRate, float severity);

  void updatePosition(int (&position)[15], int currentPosition);
  int position(std::vector<float> coordinates);
  float fitnessEval(int currentPosition, int position[15], float sumPoints,
                    std::vector<float> sensorValues);
  Population createNewGen(Population &population, int tournamentSize,
                          float elitism);
  void sortByFitness(Population &population);
  Individual child(Individual parentA, Individual &parentB);
  void printAll(Individual individual);
  Individual returnBestWeights(Individual individual);

  void printPopToFile(Population population);
  std::vector<std::string> parseFile(std::string filename, int popsize);
  Individual returnFileWeights(std::vector<std::string> fileWeights);
};
