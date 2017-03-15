#include "neural_network.hpp"

neural_network::neural_network() {
  gps = getGPS("gps");
  gps->enable(TIME_STEP);

  radio = getEmitter("radio");

  receiver = getReceiver("receiver");
  receiver->enable(TIME_STEP);

  std::string ps = "ps";
  std::string led = "led";

  for (int i = 0; i < 8; i++) {
    distanceSensors[i] = getDistanceSensor(ps.replace(2, 1, std::to_string(i)));
    distanceSensors[i]->enable(TIME_STEP);
    leds[i] = getLED(led.replace(3, 1, std::to_string(i)));
  }
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

        for (int i = 0; i<4; i++) {
          for (int j = 0; j<8; j++) {
            row.push_back(atof(match1.c_str()));
          }
          layer1.push_back(row);
          row.clear();
        }
        for(int i = 0; i<2; i++) {
            for(int j = 0; j<4; j++) {
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

  std::vector<std::vector<float>> current = weightLayer;
  std::vector<float> layerOutput;
  float sum = 0;
  for (size_t i = 0; i < current.size(); i++) {
    sum = 0;
    for (size_t j = 0; j < nodeOutputs.size(); j++) {
      sum += nodeOutputs[j] * current[i][j];
    }
    // std::cout << sum << std::endl;
    layerOutput.push_back(sum);
  }
  return activationFunc(layerOutput);
}

std::vector<float> neural_network::getOutputs(
    std::vector<float> inputs,
    Individual weights) {
  std::vector<float> firstLayer = activationFunc(inputs);

  std::vector<float> hiddenLayer = layerCalc(firstLayer, weights[0]);

  std::vector<float> outputLayer = layerCalc(hiddenLayer, weights[1]);

  std::transform(outputLayer.begin(), outputLayer.end(), outputLayer.begin(),
                 std::bind1st(std::multiplies<float>(), 1000));

  return outputLayer;
}

void neural_network::run() {
  std::vector<float> vals, dist, wheelSpeeds;
    simulationControl *controller = new simulationControl();
    // controller->run();

    Individual customWeights = controller->createIndividual();

  while (step(TIME_STEP) != -1) {
    dist = getDistanceSensorValues();
    // vals = getGPSValues();
    // ALWAYS ALWAYS ALWAYS dist first before vals
    // sendPacket(dist);
    // sendPacket(vals);

    getReceiverData();
    processReceiverData(receivedWeights);

    if (messageReceived) {
      // wheelSpeeds = getOutputs(dist, weights);
      wheelSpeeds = getOutputs(dist, customWeights);
      setSpeed(wheelSpeeds[0], wheelSpeeds[1]);
    } else {
      setSpeed(10, 10);
    }

  }
}

int main(int argc, char const *argv[]) {
  neural_network *net = new neural_network();
  net->run();
  delete net;
  return 0;
}
