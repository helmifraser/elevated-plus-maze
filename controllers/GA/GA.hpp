// #include <webots/DistanceSensor.hpp>
// #include <webots/GPS.hpp>

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
  std::map<int, int> punishment;
  std::vector<float> createRow(int parameters[3]);
  void mutate(Individual &individual, float severity);

public:
  // std::vector<float> getDistanceValues();
  // std::vector<float> getGPSValues();
  Population populate(int popsize);
  Individual createIndividual();

  void mutateGen(Population &population, float mutateRate, float severity);

  void updatePosition(int positions[15], int currentPosition);
  int position(std::vector<float> coordinates);
  float fitnessEval(int currentPosition, float sumPoints,
                    std::vector<float> sensorValues);
  Population createNewGen(Population &population, int tournamentSize,
                          float elitism);
  void sortByFitness(Population &population);
  Individual child(Individual parentA, Individual &parentB);
  void printIndividual(Individual individual);
  void printPopToFile(Population population);
  std::vector<std::string> parseFile(std::string filename, int popsize);
  Individual returnFileWeights(std::vector<std::string> fileWeights);
  Individual returnBestWeights(Individual individual);

};
