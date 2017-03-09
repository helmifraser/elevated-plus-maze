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

float GA::fitnessEval(int currentPosition, float sumPoints, std::vector<float> sensorValues) {

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
