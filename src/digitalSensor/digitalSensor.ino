/**
 * @brief This is a minimalistic version of the accelerometer program.
 * @author Leonardo Molina (leonardomt@gmail.com).
 * @file digitalSensor.ino
 * @date 2022-05-03
 * @version: 0.1.0
*/

#include <SPI.h>
#include <ADXL362.h>

// Accelerometer sensor.
ADXL362 sensor;
// Output BNC pin.
const uint8_t bncPin = 9;
// Current value read.
int16_t x;
// Previous value reported.
int16_t last = 0;

void setup() {
	// Begin serial communication.
	Serial.begin(9600);
	// Setup SPI protocol, issue device soft reset
	sensor.begin(10);
	// Switch ADXL362 to measure mode
	sensor.beginMeasure();
	// Configure BNC pin.
	pinMode(bncPin, OUTPUT);
}

void loop(){
	// Read abs of data; range is (-2048, +2048).
	do {
		x = sensor.readXData();
	} while (abs(x) > 1000);
	x = abs(x);
	// Reproduce value in BNC port in the range (0, 512).
	analogWrite(bncPin, x / 4);
	if (x != last) {
		last = x;
		Serial.println(x);
	}
	delay(50);
}

