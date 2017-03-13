#include <webots/Field.hpp>
#include <webots/Keyboard.hpp>
#include <webots/Node.hpp>
#include <webots/Supervisor.hpp>

#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;
using namespace webots;

class simulationControl : public Supervisor {
public:
  simulationControl();
  void run();
  void live();

private:
  std::string name;
  int timeStep;
  Node *robot;
  Field *translationField;
  Field *rotationField;
  Keyboard *keyboard;
  double translation[3];
  double rotation[4];
  int fuckingCount;

};
