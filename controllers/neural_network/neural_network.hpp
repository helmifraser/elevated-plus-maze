// webots
#include <webots/DifferentialWheels.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/Emitter.hpp>
#include <webots/Receiver.hpp>
#include <webots/GPS.hpp>
#include <webots/LED.hpp>
#include <webots/Robot.hpp>
#include <webots/Keyboard.hpp>

// own
#include <array>
#include <cmath>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <regex>

#include "GA.hpp"

#define TIME_STEP 32

using namespace webots;
using Individual = std::vector<std::vector<std::vector<float>>>;


class neural_network : public DifferentialWheels {
private:
  // Instantiating things we need in the controller
  DifferentialWheels *diffWheels;
  DistanceSensor *distanceSensors[8];
  Emitter *radio;
  Receiver *receiver;
  GPS *gps;
  LED *leds[8];
  Individual weights;
  GA *myGA;
  Keyboard *keyboard;

  int popsize,timeCount, count, generations, tournamentSize;

  float elitism, muteRate, severity;

  const char *data;
  std::string receivedWeights;
  bool messageReceived;

  float scaleVal(float parameters[4], float value);
  std::vector<float> activationFunc(std::vector<float> input);
  std::vector<float> layerCalc(std::vector<float> nodeOutputs,
                               std::vector<std::vector<float>> weights);
  void sendPacket(std::vector<float> sensorValues);
  std::vector<float> getDistanceSensorValues();
  std::vector<float> getGPSValues();
  void getReceiverData();
  void processReceiverData(std::string data);
  void printAll(Individual individual);



  public:
    neural_network();
    void run();
    void demo();
    void keyboardSelect();
    std::vector<float> getOutputs(std::vector<float> inputs,
                                  Individual weights);
  };
