// webots
#include <webots/DifferentialWheels.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/Emitter.hpp>
#include <webots/GPS.hpp>
#include <webots/LED.hpp>
#include <webots/Robot.hpp>

// own
#include <array>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

#define TIME_STEP 32

using namespace webots;
using Individual = std::vector<std::vector<std::vector<float>>>;


class neural_network : public DifferentialWheels {
private:
  // Instantiating things we need
  DifferentialWheels *diffWheels;
  DistanceSensor *distanceSensors[8];
  Emitter *radio;
  GPS *gps;
  LED *leds[8];

  std::vector<float> activationFunc(std::vector<float> input);
  std::vector<float> layerCalc(std::vector<float> nodeOutputs,
                               std::vector<std::vector<float>> weights);
  void sendPacket(std::vector<float> sensorValues);
  std::vector<float> getDistanceSensorValues();
  std::vector<float> getGPSValue();

  public:
    neural_network();
    std::vector<float> getOutputs(std::vector<float> inputs,
                                  Individual weights);
  };
