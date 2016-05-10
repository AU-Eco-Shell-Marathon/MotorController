#ifndef EFFECTSENSOR_H
#define EFFECTSENSOR_H

#include <project.h>

void effectSensor_init(int16 Volt, int16 Current);

void effectSensor_calibrate(int16 * Volt, int16 * Current);

uint16 effectSensor_getValue();

#endif
