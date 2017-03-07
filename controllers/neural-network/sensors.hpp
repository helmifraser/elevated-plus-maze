#include <webots/DistanceSensor.hpp>
#include <webots/GPS.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define TIME_STEP 8
#define PS_THRESHOLD 150

using namespace webots;

class sensors {
private:
  DistanceSensor *distanceSensors[8];
  GPS *gps;

public:
  std::vector<float> getDistanceValues();
  std::vector<float> getGPSValues();
};
