#pragma config(Sensor, S4,     HTSMUX,         sensorI2CCustom)
#pragma config(Sensor, S3,     SONAR,          sensorSONAR)

#include "drivers/hitechnic-sensormux.h"
#include "drivers/hitechnic-gyro.h"

task main() {
	while(true) {
		float irvalue = SensorValue[IR];
		float sonarvalue = SensorValue[SONAR];
		nxtDisplayBigStringAt(0,50, "IR:%d   ", irvalue);
		nxtDisplayBigStringAt(0, 25, "Son:%d  ", sonarvalue);
	}
}
