#include "GA.hpp"
#include <algorithm>

std::map<int, int> points = {{0, -10}, {1, 2},   {2, 2},   {3, 1},
                             {4, 2},   {5, 2},   {6, -1},  {7, -2},
                             {8, -3},  {9, -4},  {10, -5}, {11, -1},
                             {12, -2}, {13, -3}, {14, -4}, {15, -5}};

std::map<std::string, float> sensorPoints = {{"reward", 0.5},
                                             {"punishment", -0.25}};

void GA::printAll(Individual individual) {
  for (int z = 0; z < individual.size(); z++) {
    for (int x = 0; x < individual[z].size(); x++) {
      for (int c = 0; c < individual[z][x].size(); c++) {
        std::cout << " " << individual[z][x][c] << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
}
Individual GA::returnBestWeights(Individual individual) {
  Individual bestWeights;

  for (int i = 0; i < 2; i++) {
    bestWeights.push_back(individual[i]);
  }

  return bestWeights;
}

Population GA::populate(int popsize) {
  Population population;
  population.clear();

  Genotype current;

  for (int i = 0; i < popsize; i++) {
    current.individual = createIndividual();
    current.fitness = 0;
    population.push_back(current);
  }

  return population;
}

Individual GA::createIndividual() {
  Individual individual;
  std::vector<std::vector<float>> layerOne, layerTwo;
  layerOne.clear();
  layerTwo.clear();
  individual.clear();

  int parameters[3] = {-1, 1, 4};
  int parameters2[3] = {-1, 1, 2};

  for (int j = 0; j < 8; j++) {
    layerOne.push_back(createRow(parameters));
  }

  for (int j = 0; j < 4; j++) {
    layerTwo.push_back(createRow(parameters2));
  }

  individual.push_back(layerOne);
  individual.push_back(layerTwo);

  return individual;
}

// 0 - min of range, 1 - max of range, 2 size of vector
std::vector<float> GA::createRow(int parameters[3]) {
  // First create an instance of an engine.
  std::random_device rnd_device;
  // Specify the engine and distribution.
  std::mt19937 mersenne_engine(rnd_device());
  std::uniform_real_distribution<float> dist(parameters[0], parameters[1]);

  auto gen = std::bind(dist, mersenne_engine);
  std::vector<float> vec(parameters[2]);
  generate(begin(vec), end(vec), gen);

  return vec;
}

Individual GA::child(Individual parentA, Individual &parentB) {

  std::random_device rnd_device;
  std::default_random_engine generator(rnd_device());
  std::uniform_real_distribution<float> distribution(0.0, 1.0);

  for (int i = 0; i < parentA.size(); i++) {
    for (int j = 0; j < parentA[i].size(); j++) {
      for (int k = 0; k < parentA[i][0].size(); k++) {
        float gen = distribution(generator);
        if (gen < 0.5) {
          parentA[i][j][k] = parentB[i][j][k];
        }
      }
    }
  }

  return parentA;
}

void GA::mutateGen(Population &population, float mutateRate, float severity) {
  std::random_device rnd_device;
  std::default_random_engine generator(rnd_device());

  for (int i = 0; i < population.size(); i++) {
    std::uniform_real_distribution<float> chance(0.0, 1.0);
    if (chance(generator) < mutateRate) {
      mutate(population[i].individual, severity);
    }
  }
}

void GA::mutate(Individual &individual, float severity) {
  std::random_device rnd_device;
  std::default_random_engine generator(rnd_device());

  for (int i = 0; i < individual.size(); i++) {
    std::uniform_real_distribution<float> elements(
        0.0, individual[i].size() * individual[i][0].size() - 1);
    std::uniform_real_distribution<float> rows(0.0, individual[i].size() - 1);
    std::uniform_real_distribution<float> chromosomeLength(
        0.0, individual[i][0].size() - 1);

    int numberElements = severity * elements(generator);
    std::cout << "numberElements " << numberElements << std::endl;

    for (int j = 0; j < numberElements; j++) {
      int rowNumber = rows(generator);
      int index = chromosomeLength(generator);

      std::normal_distribution<float> mutatedValue(-1, 1);
      float newWeight =
          individual[i][rowNumber][index] * mutatedValue(generator);
      individual[i][rowNumber][index] = newWeight;
    }
  }
}

float GA::fitnessEval(int currentPosition, float sumPoints,
                      std::vector<float> sensorValues) {

  sumPoints += points[currentPosition];

  for (int i = 0; i < 8; i++) {
    if (sensorValues[i] >= 1.71) {
      sumPoints += sensorPoints["punishment"];
    } else if (sensorValues[i] < 1.71) {
      sumPoints += sensorPoints["reward"];
    }
  }

  return sumPoints;
}

Population GA::createNewGen(Population &population, int tournamentSize,
                            float elitism) {
  sortByFitness(population);

  std::random_device rnd_device;
  std::default_random_engine generator(rnd_device());
  Population random_parents, new_population;
  std::vector<int> parentIndex;
  parentIndex.clear();

  int chosenIndex = 0;
  int chosenOnes = elitism * population.size();

  for (int i = 0; i < population.size() - chosenOnes; i++) {
    for (int j = 0; j < tournamentSize; j++) {
      std::uniform_real_distribution<float> parents(0.0, population.size());
      chosenIndex = parents(generator);
      if (!parentIndex.empty()) {
        while (std::find(parentIndex.begin(), parentIndex.end(), chosenIndex) !=
               parentIndex.end()) {
          chosenIndex = parents(generator);
        }
      }
      parentIndex.push_back(chosenIndex);
      random_parents.push_back(population[chosenIndex]);
    }

    sortByFitness(random_parents);
    Genotype offspring = {
        child(random_parents[0].individual, random_parents[1].individual), 0.0};
    new_population.push_back(offspring);
    for (int i = 0; i < random_parents.size(); i++) {
      random_parents[i].fitness = 0;
    }
    random_parents.clear();
    parentIndex.clear();
  }

  for (int i = 0; i < chosenOnes; i++) {
    new_population.push_back(population[i]);
  }

  return new_population;
}

void GA::sortByFitness(Population &population) {
  std::sort(population.begin(), population.end(),
            [](const auto &i, const auto &j) { return i.fitness > j.fitness; });
}

void GA::updatePosition(int positions[15], int currentPosition) {
  positions[currentPosition - 1]++;
}

int GA::position(std::vector<float> coordinates) {
  int current = 0;
  float x = coordinates[0];
  float z = coordinates[2];

  float temp = 0;

  int count = 5;

  if ((x < 0.097) & (x > -0.003) & (z >= -1.09) & (z < 0)) {
    for (int i = 0; i < count; i++) {
      if ((z <= temp) & (z > temp - 0.23)) {
        current = i + 1;
        i = 5;
      }
      temp = temp - 0.23;
    }
  } else if ((z >= -0.689) & (z < -0.46)) {

    if ((x > 0)) {
      temp = 0.097;
      for (int i = 0; i < count; i++) {
        if ((x >= temp) & (x < temp + 0.1)) {
          current = i + 6;
          i = 5;
        }

        temp = temp + 0.1;
      }
    } else if (x < 0) {
      temp = -0.003;
      for (int i = 0; i < count; i++) {
        if ((x <= temp) & (x > temp - 0.1)) {
          current = i + 11;
          i = 5;
        }
        temp = temp - 0.1;
      }
    }
  }

  return current;
}

int roundNum(float x) { return floor(x * 100 + 0.5) / 100; }

void GA::printPopToFile(Population population) {
  time_t now = time(0);
  tm *ltm = localtime(&now);
  std::string time_now =
      std::to_string(ltm->tm_mday) + "-" + std::to_string(ltm->tm_hour) + ":" +
      std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec);

  std::string filename = time_now + "weights.xml";

  pugi::xml_document doc;

  std::string str;

  str.append("<nodes>");
  for (int i = 0; i < population.size(); i++) {
    std::cout << i << std::endl;
    std::string node1 = "<node" + std::to_string(i) + "> ";
    str.append(node1);
    for (int j = 0; j < population[i].individual.size(); j++) {
      std::string layer = "<layer" + std::to_string(j) + " val=\"";
      str.append(layer);
      for (int k = 0; k < population[i].individual[j][0].size(); k++) {
        str.append("[");
        for (int l = 0; l < population[i].individual[j].size(); l++) {
          std::string r =
              std::to_string(population[i].individual[j][l][k]) + ",";
          str.append(r);
        }
        str.append("]/");
      }
      std::string layerEnd = "]\" ";
      std::string layer1 = "/>";
      str.append(layerEnd);
      str.append(layer1);
    }
    std::string end1 = "</node" + std::to_string(i) + ">\n";
    str.append(end1);
  }
  str.append("</nodes>");
  doc.load_string(str.c_str());
  std::cout << "Saving result: " << doc.save_file(filename.c_str())
            << std::endl;
}

std::vector<std::string> GA::parseFile(std::string filename, int popsize) {
  std::vector<std::string> layerWeights;
  pugi::xml_document doc;

  pugi::xml_parse_result result = doc.load_file(filename.c_str());

  for (int i = 0; i < popsize; i++) {
    std::string nodeNum = "node" + std::to_string(i);
    std::cout << "Weight " << i << std::endl;
    std::string layerone = doc.child("nodes")
                               .child(nodeNum.c_str())
                               .child("layer0")
                               .attribute("val")
                               .value();
    std::string layertwo = doc.child("nodes")
                               .child(nodeNum.c_str())
                               .child("layer1")
                               .attribute("val")
                               .value();
    layerWeights.push_back(layerone);
    layerWeights.push_back(layertwo);
    // std::cout << "Load result: " << result.description() <<
    // doc.child("nodes").child(nodeNum.c_str()).child("layer0").attribute("val").value()
    // << std::endl;
    // std::cout << "Load result: " << result.description() <<
    // doc.child("nodes").child(nodeNum.c_str()).child("layer1").attribute("val").value()
    // << std::endl;
  }

  return layerWeights;
}
Individual GA::returnFileWeights(std::vector<std::string> fileWeights) {
  Individual weights;
  std::vector<float> row;
  std::vector<std::vector<float>> layer;
  float temp = 0;
  const std::string s = ",";
  const std::string t = " ";

  std::string::size_type n = 0;
  for (int i = 0; i < fileWeights.size(); i++) {
    fileWeights[i].erase(
        std::remove(fileWeights[i].begin(), fileWeights[i].end(), '['),
        fileWeights[i].end());
    fileWeights[i].erase(
        std::remove(fileWeights[i].begin(), fileWeights[i].end(), ']'),
        fileWeights[i].end());
    std::stringstream test(fileWeights[i]);
    std::string r;
    std::vector<std::string> rows;

    while (std::getline(test, r, '/')) {
      rows.push_back(r);
    }
    for (int m = 0; m < rows.size(); m++) {

      std::stringstream test(rows[m]);
      std::string element;
      while (std::getline(test, element, ',')) {

        row.push_back(atof(element.c_str()));
      }

      layer.push_back(row);

      row.clear();
    }

    weights.push_back(layer);
    layer.clear();
  }
  return weights;
}
