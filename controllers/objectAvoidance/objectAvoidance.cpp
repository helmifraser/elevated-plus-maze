#include "objectAvoidance.hpp"

class objectAvoidance : public DifferentialWheels {

private:
  DifferentialWheels *diffWheels;
  DistanceSensor *distanceSensors[8];
  LED *leds[8];
  Keyboard *keyboard;
  GPS *gps;

  std::array<int, 3> speed;
  int multiplier;

  double ps_values[8];
  bool left_obstacle, right_obstacle, front_obstacle, back_obstacle;

public:
  objectAvoidance() {
    keyboard = getKeyboard();
    keyboard->enable(TIME_STEP);

    std::string ps = "ps";
    std::string led = "led";
    for (int i = 0; i < 8; i++) {
      distanceSensors[i] =
          getDistanceSensor(ps.replace(2, 1, std::to_string(i)));
      distanceSensors[i]->enable(TIME_STEP);
      leds[i] = getLED(led.replace(3, 1, std::to_string(i)));
    }

    gps = getGPS("gps");
    gps->enable(TIME_STEP);

    left_obstacle = right_obstacle = front_obstacle = back_obstacle = false;
    speed = {100, 50, 0};
    multiplier = 1;
  }

  void run() {
    while (step(TIME_STEP) != -1) {
      std::cout << "Press 1 to object detection, 2 for teleop " << std::endl;
      int decide = readKey();
      switch (decide) {
      case 49:
        std::cout << "Start" << std::endl;
        objectDetectionMode();
        break;
      case 50:
        std::cout << "Teleop" << '\n';
        teleop();
        break;
      }
    }
  }

  void teleop() {
    while (step(TIME_STEP != -1)) {
      setLEDs(0);
      // -----get data-----
      int keyPress = readKey();

      // -----process data-----

      // -----send actuator commands-----
      keyboardControl(keyPress);
    }
  }

  void keyboardControl(int keyPress) {

    switch (keyPress) {
    // W
    case 87:
      move(multiplier * speed[0], multiplier * speed[0]);
      break;

    // S
    case 83:
      move(-multiplier * speed[0], -multiplier * speed[0]);
      break;

    // A
    case 65:
      move(-multiplier * speed[0], multiplier * speed[0]);
      break;

    // D
    case 68:
      move(multiplier * speed[0], -multiplier * speed[0]);
      break;

    // Q
    case 81:
      move(multiplier * speed[1], multiplier * speed[0]);
      break;

    // E
    case 69:
      move(multiplier * speed[0], multiplier * speed[1]);
      break;

    // UP ARROW
    case 315:
      if (multiplier < 10) {
        multiplier++;
      }
      break;

    // DOWN ARROW
    case 317:
      if (multiplier > 1) {
        multiplier--;
      }
      break;

    default:
      move(multiplier * speed[2], multiplier * speed[2]);
      break;
    }

    std::cout << "Control using WASD (Q, E for curve), speed up/down using "
                 "UP/DOWN ARROWS"
              << '\n';
    std::cout << "Going " << multiplier << " times base speed" << std::endl;
  }

  void objectDetectionMode() {
    setLEDs(1);
    while (step(TIME_STEP != -1)) {
      objectDetection(1.0);
    }
  }

  void distanceCheck() {
    left_obstacle = right_obstacle = front_obstacle = back_obstacle = false;

    for (int i = 0; i < 8; i++) {
      ps_values[i] = checkDistanceSensor(i);
    }

    // detect obsctacles
    left_obstacle = (ps_values[0] > PS_THRESHOLD) ||
                    (ps_values[1] > PS_THRESHOLD) ||
                    (ps_values[2] > PS_THRESHOLD);
    right_obstacle = (ps_values[5] > PS_THRESHOLD) ||
                     (ps_values[6] > PS_THRESHOLD) ||
                     (ps_values[7] > PS_THRESHOLD);
    front_obstacle =
        (ps_values[0] > PS_THRESHOLD) & (ps_values[7] > PS_THRESHOLD);
    back_obstacle =
        (ps_values[3] > PS_THRESHOLD) & (ps_values[4] > PS_THRESHOLD);
    std::cout << "ps_values ";
    for (size_t i = 0; i < 8; i++) {
      std::cout << ps_values[i] << " ";
    }
    std::cout << std::endl;
  }

  void objectDetection(double speedAdjust) {
    // setLEDs(1);
    distanceCheck();

    // init speeds
    double left_speed = speedAdjust * WHEEL_SPEED;
    double right_speed = speedAdjust * WHEEL_SPEED;

    // modify speeds according to obstacles
    if (left_obstacle & !right_obstacle) {
      // turn left
      left_speed -= speedAdjust * WHEEL_SPEED;
      right_speed += speedAdjust * WHEEL_SPEED;
      std::cout << "Turning left" << std::endl;
    } else if (right_obstacle & !left_obstacle) {
      // turn right
      left_speed += speedAdjust * WHEEL_SPEED;
      right_speed -= speedAdjust * WHEEL_SPEED;
      std::cout << "Turning right" << std::endl;
    } else if (right_obstacle & left_obstacle) {
      left_speed = -speedAdjust * WHEEL_SPEED;
      right_speed = -speedAdjust * WHEEL_SPEED;
      std::cout << "Backwards" << std::endl;
    }

    if (left_speed > 1000) {
      left_speed = 1000;
    }

    if (right_speed > 1000) {
      right_speed = 1000;
    }
    std::cout << "left_speed " << left_speed << " right_speed " << right_speed
              << '\n';
    move((int)left_speed, (int)right_speed);
  }

  void move(int left, int right) { setSpeed(left, right); }

  void setLEDs(int value) {
    for (int i = 0; i < 8; i++)
      leds[i]->set(value);
  }

  double checkDistanceSensor(int n) { return distanceSensors[n]->getValue(); }

  int readKey() { return keyboard->getKey(); }
};

int main(int argc, char const *argv[]) {

  objectAvoidance *robot = new objectAvoidance();
  robot->run();
  delete robot;
  return 0;
}
