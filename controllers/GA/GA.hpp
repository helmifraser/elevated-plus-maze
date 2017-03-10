// #include <webots/DistanceSensor.hpp>
// #include <webots/GPS.hpp>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <functional>

#define TIME_STEP 32
#define PS_THRESHOLD 150

// using namespace webots;

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
    float fitnessEval(int currentPosition, float sumPoints, std::vector<float> sensorValues);
    std::vector<std::vector<std::vector<std::vector<float>>>> populate(int popsize);
    std::vector<std::vector<std::vector<float>>> createIndividual();
    std::vector<std::vector<std::vector<float>>> child(std::vector<std::vector<std::vector<float>>> parentA, std::vector<std::vector<std::vector<float>>> &parentB);
    std::vector<float> createRow(int parameters[3]);

};
