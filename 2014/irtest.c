#pragma config(Sensor, S3,     IRsensor,       sensorHiTechnicIRSeeker600)

task main() {
	while(true) {
		float irvalue = SensorValue[IRsensor];
		nxtDisplayBigStringAt(50,50, "%d", irvalue);
	}
}
