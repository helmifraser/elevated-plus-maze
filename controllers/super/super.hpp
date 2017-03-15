#include <webots/Field.hpp>
#include <webots/Keyboard.hpp>
#include <webots/Node.hpp>
#include <webots/Supervisor.hpp>
#include <webots/Receiver.hpp>
#include <webots/Emitter.hpp>

#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <regex>

// #include "GA.hpp"

using namespace std;
using namespace webots;

using Individual = std::vector<std::vector<std::vector<float>>>;

class simulationControl : public Supervisor {
// Instantiating things we need in the supervisor
private:
  std::string name;
  int timeStep;
  Node *robot;
  Field *translationField;
  Field *rotationField;
  Keyboard *keyboard;
  double translation[3];
  double rotation[4];
  int count, timeCount;
  Receiver *super_receiver;
  Emitter *super_radio;

  // GA *algo;

  // const std::vector<float> *data;
  const char *data;
  std::array<std::string, 2> myString;

  std::vector<float> sensorData;
  std::vector<float> gpsData;

public:
  simulationControl();
  void run();
  void live();
  void getReceiverData(int arrayIndex);
  void sendPacket(Individual individual);
  void processReceiverData(std::string data, int i);

};
