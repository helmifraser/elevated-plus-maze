#include <webots/DifferentialWheels.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/GPS.hpp>
#include <webots/Keyboard.hpp>
#include <webots/LED.hpp>
#include <webots/LightSensor.hpp>
#include <webots/Robot.hpp>

#include <stdio.h>
#include <stdlib.h>

using namespace webots;

#define TIME_STEP 8
#define PS_THRESHOLD 150
#define WHEEL_SPEED 1000
