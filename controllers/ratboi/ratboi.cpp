#include "Sensors.hpp"
#include <webots/Robot.hpp>
#include <webots/DifferentialWheels.hpp>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#define TIME_STEP 32

class ratboi : public DifferentialWheels {

private:
    DistanceSensor *distanceSensors[8];
    GPS *gps;

public:
  ratboi() {
    gps = getGPS("gps");
    gps->enable(TIME_STEP);

    std::string ps = "ps";

    for (unsigned int i = 0; i < 8; i++) {
      distanceSensors[i] = getDistanceSensor(ps.replace(2, 1, std::to_string(i)));
      distanceSensors[i]->enable(TIME_STEP);
    }

    // distanceSensors[0] = getDistanceSensor("ps0");
    // distanceSensors[0]->enable(TIME_STEP);
    // distanceSensors[1] = getDistanceSensor("ps1");
    // distanceSensors[1]->enable(TIME_STEP);
    // distanceSensors[2] = getDistanceSensor("ps2");
    // distanceSensors[2]->enable(TIME_STEP);
    // distanceSensors[3] = getDistanceSensor("ps3");
    // distanceSensors[3]->enable(TIME_STEP);
    // distanceSensors[4] = getDistanceSensor("ps4");
    // distanceSensors[4]->enable(TIME_STEP);
    // distanceSensors[5] = getDistanceSensor("ps5");
    // distanceSensors[5]->enable(TIME_STEP);
    // distanceSensors[6] = getDistanceSensor("ps6");
    // distanceSensors[6]->enable(TIME_STEP);
    // distanceSensors[7] = getDistanceSensor("ps7");
    // distanceSensors[7]->enable(TIME_STEP);

  }

  void run() {

    while (step(TIME_STEP) != -1) {

      std::cout << "Distance ";
      for (int i = 0; i < 8; i++) {
        std::cout << distanceSensors[i]->getValue() << " ";
      }
      std::cout << std::endl;

      std::vector<float> ps_vals = getDistanceValues2();
      std::vector<float> gps_vals = getGPSValues(gps);

      std::cout << "Distance ";
      for (int i = 0; i < 8; i++) {
        std::cout << ps_vals[i] << " ";
      }
      std::cout << std::endl;

      std::cout << "GPS ";
      for (int i = 0; i < 3; i++) {
        std::cout << gps_vals[i] << " ";
      }
      std::cout << std::endl;

      std::cout << "test" << std::endl;
    }
  }

  float scaleVal2(float parameters[4], float value) {
    float out = (((parameters[3] - parameters[2]) * (value - parameters[0])) /
                 (parameters[1] - parameters[0])) +
                parameters[2];

    if (out > 2.5) {
      out = 2.5;
    } else if (out < -2.5) {
      out = -2.5;
    }

    return out;
  }

  std::vector<float> getDistanceValues2() {
    std::vector<float> current;
    float scaleParam[4] = {50, 1000, -2.5, 2.5};
    float value = 0;

    for (int i = 0; i < 8; i++) {
      value = distanceSensors[i]->getValue();
      std::cout << "value " << value << std::endl;
      float scaledVal = scaleVal2(scaleParam, value);
      std::cout << "scaledVal " << scaledVal << std::endl;
      current.push_back(scaledVal);
      value = 0;
    }

    return current;
  }
};

int main(int argc, char const *argv[]) {

  ratboi *raat = new ratboi;
  raat->run();
  delete raat;
  return 0;
}
