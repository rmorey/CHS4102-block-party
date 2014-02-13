#pragma config(Sensor, S4,     IR,             sensorHiTechnicIRSeeker600)
#pragma config(Sensor, S3,     SONAR,          sensorSONAR)

task main() {
	while(true) {
		float irvalue = SensorValue[IR];
		float sonarvalue = SensorValue[SONAR];
		nxtDisplayBigStringAt(0,50, "IR:%d   ", irvalue);
		nxtDisplayBigStringAt(0, 25, "Son:%d  ", sonarvalue);
	}
}
