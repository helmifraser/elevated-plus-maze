#include "super.hpp"

simulationControl::simulationControl() {
  timeStep = 32;
  timeCount = 100; // 9375
  name = "E-puck";
  robot = getFromDef(name.c_str());

  receiver = getReceiver("receiver");
  receiver->enable(timeStep);

  translation[0] = 0.0196607;
  translation[1] = 0.52;
  translation[2] = -0.121699;
  rotation[0] = 3.53093e-05;
  rotation[1] = 1;
  rotation[2] = 0.000133708;
  rotation[3] = 0.0541781;

  count = 0;

  if (!robot)
    exit(1);

  translationField = robot->getField("translation");
  rotationField = robot->getField("rotation");
  keyboard = getKeyboard();
  keyboard->enable(timeStep);
}

void simulationControl::run() {
  // std::vector<float> data;
  // data.clear();
  std::string hello = "0.999 0.899 9 81";

  while (step(timeStep) != -1) {
    int keyboardInput = keyboard->getKey();
    // std::cout << "Get key" << std::endl;
    switch (keyboardInput) {
    case 49:
      std::cout << "Demo" << std::endl;
      getReceiverData(0);
      getReceiverData(1);
      std::cout << "data " << myString[0] << std::endl;
      std::cout << "gps " << myString[1] << std::endl;
      processReceiverData(myString[0], 0);
      processReceiverData(myString[1], 1);
      std::cout << "receiver data ";
      for (size_t i = 0; i < sensorData.size(); i++) {
        std::cout << sensorData[i] << " ";
      }
      std::cout << std::endl;

      std::cout << "gps data ";
      for (size_t i = 0; i < gpsData.size(); i++) {
        std::cout << gpsData[i] << " ";
      }
      std::cout << std::endl;

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
    int check = count % timeCount;
    std::cout << "count " << count << std::endl;
    if (check == 0 && count != 0) {
      robot->resetPhysics();
      translationField->setSFVec3f(translation);
      rotationField->setSFRotation(rotation);
      std::cout << "reset" << std::endl;
      resetCount++;
    }
    count++;
    std::cout << "resetCount " << resetCount << std::endl;
  }
}

void simulationControl::processReceiverData(std::string data, int i) {
  try {
    std::regex re("[*0-9*.*0-9*]+|[-][*0-9*.*0-9*]+");
      std::sregex_iterator next(data.begin(), data.end(), re);
      std::sregex_iterator end;
      int index = 0;
      if (i == 0) {
        sensorData.clear();
      } else {
        gpsData.clear();
      }
      while (next != end) {
        std::smatch match = *next;
        std::string match1 = match.str();
        if (i == 0) {
          sensorData.push_back(atof(match1.c_str()));
        } else if (i == 1) {
          gpsData.push_back(atof(match1.c_str()));
        }
        next++;
        index++;
    }

  } catch (std::regex_error &e) {
    std::cout << "Error in regex" << std::endl;
  }
}

void simulationControl::getReceiverData(int arrayIndex) {
  const size_t spartans = 400;
  Receiver *copy = (Receiver *)malloc(spartans);
  for (int i = 0; i < receiver->getQueueLength(); i++) {
    data = (char *)receiver->getData();
    memcpy(copy, data, spartans);
    myString[arrayIndex] = (char *)copy;
    receiver->nextPacket();
  }
}

int main() {
  std::cout << "Run" << std::endl;
  simulationControl *controller = new simulationControl();
  controller->run();
  delete controller;
  return 0;
}
