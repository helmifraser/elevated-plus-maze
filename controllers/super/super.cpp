#include "super.hpp"

simulationControl::simulationControl() {
  timeStep = 32;
  name = "E-puck";
  robot = getFromDef(name.c_str());

  translation[0] = 0.0196607;
  translation[1] = 0.52;
  translation[2] = -0.121699;
  rotation[0] = 3.53093e-05;
  rotation[1] = 1;
  rotation[2] = 0.000133708;
  rotation[3] = 0.0541781;

  fuckingCount = 0;

  if (!robot) exit(1);

  translationField = robot->getField("translation");
  rotationField = robot->getField("rotation");
  keyboard = getKeyboard();
  keyboard->enable(timeStep);
}

void simulationControl::run() {

  while (step(timeStep) != -1) {
    int keyboardInput = keyboard->getKey();
    std::cout << "Get key" << std::endl;
    switch (keyboardInput) {
    case 49:
      std::cout << "Demo" << std::endl;
      break;
    case 50:
      std::cout << "Live" << std::endl;
      live();
      break;
    }

  }
}

void simulationControl::live() {
  int resetCount = 0;
  while (step(timeStep) != -1) {
    int check = fuckingCount % 9375;
    std::cout << "fuckingCount " << fuckingCount << std::endl;
    std::cout << "Fatricia " << check << std::endl;
    if (check == 0 && fuckingCount != 0) {
      robot->resetPhysics();
      translationField->setSFVec3f(translation);
      rotationField->setSFRotation(rotation);
      std::cout << "reset" << std::endl;
      resetCount++;
    }
    fuckingCount++;
    std::cout << "resetCount " << resetCount << std::endl;
  }
}

int main() {
  std::cout << "Run" << std::endl;
  simulationControl *controller = new simulationControl();
  controller->run();
  delete controller;
  return 0;
}
