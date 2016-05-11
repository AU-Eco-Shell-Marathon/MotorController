#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <project.h>
#include "PID.h"
#include "effectSensor.h"
#include "RPMSensor.h"
#include "Motor_lookupTable.h"
#include "Pedal.h"


void MC_stop();
void MC_start();
void MC_init(const struct PIDparameter * pidval);
void MC_ChangePID(const struct PIDparameter * pidval);

#endif
