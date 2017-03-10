#include "GA.hpp"

std::map<int, int> points = {{0, -10}, {1, 2},   {2, 2},   {3, 1},
                             {4, 2},   {5, 2},   {6, -1},  {7, -2},
                             {8, -3},  {9, -4},  {10, -5}, {11, -1},
                             {12, -2}, {13, -3}, {14, -4}, {15, -5}};

std::map<std::string, float> sensorPoints = {{"reward", 0.5},
                                             {"punishment", -0.25}};

// std::vector<float> getGPSValues(){
//     std::vector<float> position;
//     const double *dataGPS = gps->getValues();
//
//     for (int i = 0; i < 3; i++) {
//       position.push_back( (float) dataGPS[i]);
//     }
//
//     return position;
// }

std::vector<std::vector<std::vector<std::vector<float>>>>
GA::populate(int popsize) {
  std::vector<std::vector<std::vector<std::vector<float>>>> population;
  population.clear();

  for (int i = 0; i < popsize; i++) {
    population.push_back(createIndividual());
  }

  return population;
}

std::vector<std::vector<std::vector<float>>> GA::createIndividual() {
  std::vector<std::vector<std::vector<float>>> individual;
  std::vector<std::vector<float>> layerOne, layerTwo;
  layerOne.clear();
  layerTwo.clear();
  individual.clear();

  int parameters[3] = {-1, 1, 4};
  int parameters2[3] = {-1, 1, 2};

  for (int j = 0; j < 8; j++) {
    layerOne.push_back(createRow(parameters));
  }

  for (int j = 0; j < 4; j++) {
    layerTwo.push_back(createRow(parameters2));
  }

  individual.push_back(layerOne);
  individual.push_back(layerTwo);

  return individual;
}

// 0 - min of range, 1 - max of range, 2 size of vector
std::vector<float> GA::createRow(int parameters[3]) {
  // First create an instance of an engine.
  std::random_device rnd_device;
  // Specify the engine and distribution.
  std::mt19937 mersenne_engine(rnd_device());
  std::uniform_real_distribution<float> dist(parameters[0], parameters[1]);

  auto gen = std::bind(dist, mersenne_engine);
  std::vector<float> vec(parameters[2]);
  generate(begin(vec), end(vec), gen);

  return vec;
}

std::vector<std::vector<std::vector<float>>>
GA::child(std::vector<std::vector<std::vector<float>>> parentA,
      std::vector<std::vector<std::vector<float>>> &parentB) {

  std::cout << "1" << std::endl;
  std::cout << "2" << std::endl;
  for (int i = 0; i < parentA.size(); i++) {
    std::cout << "first loop" << std::endl;
    for (int j = 0; j < parentA[i].size(); j++) {
      std::cout << "second loop" << std::endl;
      for (int k = 0; k < parentA[i][0].size(); k++) {
        std::cout << "third loop" << std::endl;
        double gen = (rand() % 101);
        std::cout << "gen " << gen << std::endl;
        if (gen < 50) {
          std::cout << "tails" << std::endl;
          parentA[i][j][k] = parentB[i][j][k];
          std::cout << "/* message */" << std::endl;
        }
      }
    }
  }

  return parentA;
}

float GA::fitnessEval(int currentPosition, float sumPoints,
                      std::vector<float> sensorValues) {

  sumPoints += points[currentPosition];

  for (int i = 0; i < 8; i++) {
    if (sensorValues[i] >= 1.71) {
      sumPoints += sensorPoints["punishment"];
    } else if (sensorValues[i] < 1.71) {
      sumPoints += sensorPoints["reward"];
    }
  }

  return sumPoints;
}

void GA::updatePosition(int positions[15], int currentPosition) {
  positions[currentPosition - 1]++;
}

int GA::position(std::vector<float> coordinates) {
  int current = 0;
  float x = coordinates[0];
  float z = coordinates[2];

  float temp = 0;

  int count = 5;

  if ((x < 0.097) & (x > -0.003) & (z >= -1.09) & (z < 0)) {
    for (int i = 0; i < count; i++) {
      if ((z <= temp) & (z > temp - 0.23)) {
        current = i + 1;
        i = 5;
      }
      temp = temp - 0.23;
    }
  } else if ((z >= -0.689) & (z < -0.46)) {

    if ((x > 0)) {
      temp = 0.097;
      for (int i = 0; i < count; i++) {
        if ((x >= temp) & (x < temp + 0.1)) {
          current = i + 6;
          i = 5;
        }

        temp = temp + 0.1;
      }
    } else if (x < 0) {
      temp = -0.003;
      for (int i = 0; i < count; i++) {
        if ((x <= temp) & (x > temp - 0.1)) {
          current = i + 11;
          i = 5;
        }
        temp = temp - 0.1;
      }
    }
  }

  return current;
}
