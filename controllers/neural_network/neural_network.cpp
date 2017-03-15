#include "neural_network.hpp"

neural_network::neural_network() {
  gps = getGPS("gps");
  gps->enable(TIME_STEP);

  radio = getEmitter("radio");

  receiver = getReceiver("receiver");
  receiver->enable(TIME_STEP);

  keyboard = getKeyboard();
  keyboard->enable(TIME_STEP);

  std::string ps = "ps";
  std::string led = "led";

  for (int i = 0; i < 8; i++) {
    distanceSensors[i] = getDistanceSensor(ps.replace(2, 1, std::to_string(i)));
    distanceSensors[i]->enable(TIME_STEP);
    leds[i] = getLED(led.replace(3, 1, std::to_string(i)));
  }

  myGA = new GA();
  popsize = 50;
  generations = 20;
  tournamentSize = 5;
  elitism = 0.2;
  muteRate = 0.02;
  severity = 0.4;

  timeCount = 938;
  count = 0;
}

void neural_network::sendPacket(std::vector<float> sensorValues) {
  std::vector<float> packet = sensorValues;
  std::string temp, othertemp;
  for (size_t i = 0; i < packet.size(); i++) {
    othertemp = std::to_string(packet[i]) + " ";
    temp.append(othertemp);
  }
  const char *message = temp.c_str();
  radio->send(message, strlen(message) + 1);
}

void neural_network::getReceiverData() {
  const size_t spartans = 400;
  Receiver *copy = (Receiver *)malloc(spartans);
  for (int i = 0; i < receiver->getQueueLength(); i++) {
    data = (char *)receiver->getData();
    memcpy(copy, data, spartans);
    receivedWeights = (char *)copy;
    receiver->nextPacket();
  }
}

void neural_network::processReceiverData(std::string data) {
  messageReceived = false;
  try {
    std::regex re("[*0-9*.*0-9*]+|[-][*0-9*.*0-9*]+");
    std::sregex_iterator next(data.begin(), data.end(), re);
    std::sregex_iterator end;
    std::vector<float> row;
    std::vector<std::vector<float>> layer1;
    std::vector<std::vector<float>> layer2;

    int index = 0;

    while (next != end) {
      std::smatch match = *next;
      std::string match1 = match.str();

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
          row.push_back(atof(match1.c_str()));
        }
        layer1.push_back(row);
        row.clear();
      }
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
          row.push_back(atof(match1.c_str()));
        }
        layer2.push_back(row);
        row.clear();
      }
      weights.push_back(layer1);
      weights.push_back(layer2);
      next++;
      index++;
      if (next == end) {
        messageReceived = true;
      }
    }

  } catch (std::regex_error &e) {
    std::cout << "Error in regex" << std::endl;
  }
}

std::vector<float> neural_network::getDistanceSensorValues() {
  std::vector<float> distanceSensorsValues;
  for (int i = 0; i < 8; i++) {
    distanceSensorsValues.push_back(distanceSensors[i]->getValue());
  }
  return distanceSensorsValues;
}

std::vector<float> neural_network::getGPSValues() {
  const double *dataGPS = gps->getValues();
  std::vector<float> GPSout;
  for (int i = 0; i < 3; i++) {
    GPSout.push_back(dataGPS[i]);
  }

  return GPSout;
}

std::vector<float> neural_network::activationFunc(std::vector<float> input) {
  std::vector<float> out;

  for (size_t i = 0; i < input.size(); i++) {
    out.push_back(tanh(input[i]));
  }

  return out;
}

std::vector<float>
neural_network::layerCalc(std::vector<float> nodeOutputs,
                          std::vector<std::vector<float>> weightLayer) {

  std::vector<float> layerOutput;
  float sum = 0;

  for (size_t i = 0; i < weightLayer.size(); i++) {
    sum = 0;
    for (size_t j = 0; j < nodeOutputs.size(); j++) {
      sum += nodeOutputs[j] * weightLayer[i][j];
    }
    layerOutput.push_back(sum);
  }
  return activationFunc(layerOutput);
}

std::vector<float> neural_network::getOutputs(std::vector<float> inputs,
                                              Individual weights) {

  std::vector<float> firstLayer = activationFunc(inputs);

  std::vector<float> hiddenLayer = layerCalc(firstLayer, weights[0]);

  std::vector<float> outputLayer = layerCalc(hiddenLayer, weights[1]);
  std::transform(outputLayer.begin(), outputLayer.end(), outputLayer.begin(),
                 std::bind1st(std::multiplies<float>(), 1000));

  return outputLayer;
}

float neural_network::scaleVal(float parameters[4], float value) {
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

void neural_network::printAll(Individual individual) {
  for (size_t z = 0; z < individual.size(); z++) {
    for (size_t x = 0; x < individual[z].size(); x++) {
      for (size_t c = 0; c < individual[z][x].size(); c++) {
        std::cout << " " << individual[z][x][c] << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
}

void neural_network::run() {
  std::vector<float> vals, dist, wheelSpeeds;

  int currentPosition = 0;
  int positionMap[15];
  bool boolean = false;

  float sumPoints = 0;
  float scaleParam[4] = {50, 1000, -2.5, 2.5};
  std::vector<float> v;

  Population population = myGA->populate(popsize);
  Population newPop;

  int check = count % timeCount;

  for (int j = 0; j < popsize; j++) {
    sumPoints = 0;
    while (step(TIME_STEP) != -1 && boolean == false) {
      dist = getDistanceSensorValues();
      vals = getGPSValues();
      for (size_t i = 0; i < 8; i++) {
        v.push_back(scaleVal(scaleParam, dist[i]));
      }

      wheelSpeeds.clear();
      wheelSpeeds = getOutputs(v, population[j].individual);
      setSpeed(wheelSpeeds[0], wheelSpeeds[1]);

      currentPosition = myGA->position(vals);
      myGA->updatePosition(positionMap, currentPosition);

      sumPoints = myGA->fitnessEval(currentPosition, positionMap, sumPoints, v);
      v.clear();

      check = count % timeCount;
      if (check == 0 && count > 0) {
        boolean = true;
      }
      count++;
    }
    population[j].fitness = sumPoints;
    std::cout << "sumPoints"
              << " " << 0 << " " << sumPoints << std::endl;
    boolean = false;
  }
  for (int i = 0; i < generations - 1; i++) {
    newPop = myGA->createNewGen(population, tournamentSize, elitism);
    myGA->mutateGen(newPop, muteRate, severity);
    for (int j = 0; j < popsize; j++) {
      sumPoints = 0;
      while ((step(TIME_STEP) != -1) && boolean == false) {

        dist = getDistanceSensorValues();
        vals = getGPSValues();
        for (size_t i = 0; i < 8; i++) {
          v.push_back(scaleVal(scaleParam, dist[i]));
        }

        wheelSpeeds.clear();
        wheelSpeeds = getOutputs(v, newPop[j].individual);
        setSpeed(wheelSpeeds[0], wheelSpeeds[1]);

        currentPosition = myGA->position(vals);
        myGA->updatePosition(positionMap, currentPosition);
        sumPoints =
            myGA->fitnessEval(currentPosition, positionMap, sumPoints, v);
        v.clear();
        check = count % timeCount;
        if (check == 0 && count > 0) {
          boolean = true;
        }
        count++;
      }
      newPop[j].fitness = sumPoints;
      std::cout << "sumPoints " << i + 1 << " " << sumPoints << std::endl;
      boolean = false;
    }
  }

  myGA->sortByFitness(newPop);

  std::cout << "Done" << std::endl;
  myGA->printPopToFile(newPop);
}

void neural_network::keyboardSelect() {
  std::cout << "Select. 1 for demo, 2 for evolution" << std::endl;
  while (step(TIME_STEP) != -1) {
    int keyboardInput = keyboard->getKey();
    switch (keyboardInput) {
      case 49:
      std::cout << "Demo" << std::endl;
      demo();
      break;
      case 50:
      std::cout << "Evolve" << std::endl;
      run();
      break;
    }
  }
}

void neural_network::demo() {
  std::cout << "Parsing file" << std::endl;
  std::string filename = "weights.xml";

  Individual bestWeight = myGA->returnBestWeights(myGA->returnFileWeights(myGA->parseFile(filename, popsize)));
  std::vector<float> v, wheelSpeeds, dist;
  float scaleParam[4] = {50, 1000, -2.5, 2.5};

  while (step(TIME_STEP) != -1) {

    dist = getDistanceSensorValues();
    for (size_t i = 0; i < 8; i++) {
      v.push_back(scaleVal(scaleParam, dist[i]));
    }
    wheelSpeeds = getOutputs(v, bestWeight);
    setSpeed(wheelSpeeds[0], wheelSpeeds[1]);
    v.clear();
  }

}

int main(int argc, char const *argv[]) {
  neural_network *net = new neural_network();
  net->keyboardSelect();
  delete net;
  return 0;
}
