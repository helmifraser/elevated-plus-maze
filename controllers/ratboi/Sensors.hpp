#include <webots/DistanceSensor.hpp>
#include <webots/GPS.hpp>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define TIME_STEP 32
#define PS_THRESHOLD 150

using namespace webots;

// class Sensors : public DifferentialWheels {
// private:
//   // DistanceSensor *distanceSensors[8];
//   // GPS *gps;
//
// public:
//   DistanceSensor *distanceSensors[8];
//   GPS *gps;
//
//   Sensors();
//   std::vector<float> getDistanceValues();
//   std::vector<float> getGPSValues();
// };

std::vector<float> getDistanceValues();
std::vector<float> getGPSValues();
