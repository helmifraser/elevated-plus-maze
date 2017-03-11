#include "neural_network.hpp"

std::vector<float> neural_network::activationFunc(std::vector<float> input) {
  std::vector<float> out;

  for (int i = 0; i < input.size(); i++) {
    out.push_back(tanh(input[i]));
  }

  return out;
}

std::vector<floatf> neural_network::layerCalc(std::vector<float> nodeOutputs,
          std::vector<std::vector<float>> weightLayer) {

  std::vector<std::vector<float>> current = weightLayer;
  std::vector<float> layerOutput;
  float sum = 0;
  for (int i = 0; i < current.size(); i++) {
    sum = 0;
    for (int j = 0; j < nodeOutputs.size(); j++) {
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
