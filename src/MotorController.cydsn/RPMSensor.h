#ifndef RPMSENSOR_H
#define RPMSENSOR_H

#include <project.h>
    
void RPMSensor_init();

uint16 RPMSensor_getValue();

uint16 RPMSensor_getSpeed();

#endif