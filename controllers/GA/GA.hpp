// #include <webots/DistanceSensor.hpp>
// #include <webots/GPS.hpp>

#include "configLoader.hpp"
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
  std::map<int, int> punishment;

public:
  // std::vector<float> getDistanceValues();
  // std::vector<float> getGPSValues();
  void updatePosition(int positions[15], int currentPosition);
  int position(std::vector<float> coordinates);
  float fitnessEval(int currentPosition, float sumPoints,
                    std::vector<float> sensorValues);
  Population createNewGen(Population &population, int tournamentSize,
                          float elitism);
  void sortByFitness(Population &population);
  Population populate(int popsize);
  void mutate(Individual &individual, float severity);
  Individual createIndividual();
  Individual child(Individual parentA, Individual &parentB);
  std::vector<float> createRow(int parameters[3]);
  void printIndividual(Individual individual);
  void printPopToFile(Population population);
  std::vector<std::string> parseFile(std::string filename, int popsize);
  Individual returnFileWeights(std::vector<std::string> fileWeights);
};
