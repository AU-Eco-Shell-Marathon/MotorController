#ifndef POWERSENSOR_H
#define POWERSENSOR_H

#include <project.h>

void powerSensor_init(int16 Volt, int16 Current);

void powerSensor_calibrate(int16 * Volt, int16 * Current);

uint16 powerSensor_getValue();

#endif
