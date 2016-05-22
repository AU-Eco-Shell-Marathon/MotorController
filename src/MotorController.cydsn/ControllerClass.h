
#ifndef CONTROLLERCLASS_H
#define CONTROLLERCLASS_H

#include <project.h>
#include "MotorController.h"
#include "Pedal.h"
#include "Logger.h"
#include "EEPROM.h"
#include "CAN.h"
#include "RPMSensor.h"
#include "powerSensor.h"

enum dataType{BMS, ADC};

void init();
void run();
//void logData(uint8* data, enum dataType Type);


#endif

