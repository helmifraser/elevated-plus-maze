#include "neural_network.hpp"

std::vector<float> v3 = {1, 1, 1, 1, 1, 1, 1, 1};
std::vector<float> v5 = {1, 1, 1, 1, 1, 1, 1, 1};
std::vector<float> v6 = {1, 1, 1, 1, 1, 1, 1, 1};
std::vector<float> v7 = {1, 1, 1, 1, 1, 1, 1, 1};

std::vector<float> v8 = {1, 1, 1, 1};
std::vector<float> v9 = {1, 1, 1, 1};
std::vector<float> v10 = {1, 1, 1, 1};
std::vector<float> v11 = {1, 1, 1, 1};

std::vector<std::vector<float>> layer1weights = {v3, v5, v6, v7};
std::vector<std::vector<float>> layer2weights = {v8, v9, v10, v11};
std::vector<std::vector<std::vector<float>>> bigWeights = {layer1weights,
                                                           layer2weights};

// layer1weights = {v3, v4};

// bigWeights = {layer1weights};

float scaleVal(float parameters[4], float value) {
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

int main() {
  // neural_network *myNet = new neural_network;
  // std::cout << "I'm heeeere" << std::endl;
  // std::vector<float> testOut = {1, 1, 1, 1, 1, 1, 1, 1};
  //
  // std::vector<float> layerTest = myNet->layerCalc(testOut, bigWeights, 0);
  // std::vector<float> layer2 = myNet->layerCalc(layerTest, bigWeights, 1);
  //
  // std::cout << "layerTest: ";
  // for (int i = 0; i < layerTest.size(); i++) {
  //   std::cout << layerTest[i] << " ";
  // }
  // std::cout << std::endl;
  //
  // std::cout << "layer2: ";
  // for (int i = 0; i < layer2.size(); i++) {
  //   std::cout << layer2[i] << " ";
  // }
  // std::cout << std::endl;
  float parametersTest[4] = {50, 1000, -2.5, 2.5};
  // parametersTest[0] = 50;
  // parametersTest[1] = 1000;
  // parametersTest[2] = -2.5;
  // parametersTest[3] = 2.5;

  std::cout << "scale " << scaleVal(parametersTest, 525) << std::endl;
}
