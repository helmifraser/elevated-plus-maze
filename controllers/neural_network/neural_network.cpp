#include "neural_network.hpp"

neural_network::neural_network(){
  gps = getGPS("gps");
  gps->enable(TIME_STEP);

  radio = getEmitter("radio");

  std::string ps = "ps";
  std::string led = "led";


  for (int i = 0; i < 8; i++) {
    distanceSensors[i] =
        getDistanceSensor(ps.replace(2, 1, std::to_string(i)));
    distanceSensors[i]->enable(TIME_STEP);
    leds[i] = getLED(led.replace(3, 1, std::to_string(i)));
  }

}

void neural_network::sendPacket(std::vector<float> sensorValues) {
  std::vector<float> packet = sensorValues;
  radio->send(&packet, packet.size());
}

std::vector<float> neural_network::getDistanceSensorValues() {
  std::vector<float> distanceSensorsValues;
  for (int i = 0; i < 8; i++) {
    distanceSensorsValues.push_back(distanceSensors[i]->getValue());
  }
  return distanceSensorsValues;
}

std::vector<float> neural_network::getGPSValue() {
  const double *dataGPS = gps->getValues();
  std::vector<float> GPSout;
  for (int i = 0; i < 3; i++) {
    GPSout[i] = dataGPS[i];
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

std::vector<float> neural_network::layerCalc(std::vector<float> nodeOutputs,
          std::vector<std::vector<float>> weightLayer) {

  std::vector<std::vector<float>> current = weightLayer;
  std::vector<float> layerOutput;
  float sum = 0;
  for (size_t i = 0; i < current.size(); i++) {
    sum = 0;
    for (size_t j = 0; j < nodeOutputs.size(); j++) {
      sum += nodeOutputs[j] * current[i][j];
    }
    std::cout << sum << std::endl;
    layerOutput.push_back(sum);
  }
  return activationFunc(layerOutput);
}

std::vector<float> neural_network::getOutputs(std::vector<float> inputs, std::vector<std::vector<std::vector<float>>> weights) {
  std::vector<float> firstLayer = activationFunc(inputs);

  std::vector<float> hiddenLayer = layerCalc(firstLayer, weights[0]);

  std::vector<float> outputLayer = layerCalc(hiddenLayer, weights[1]);

  std::transform(outputLayer.begin(), outputLayer.end(), outputLayer.begin(),
               std::bind1st(std::multiplies<float>(),1000));

  return outputLayer;
}

int main(int argc, char const *argv[]) {

  return 0;
}
