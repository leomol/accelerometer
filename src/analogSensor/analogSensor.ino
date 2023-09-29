/**
 * @brief Program for accelerometer sensor ADXL335.
 * @author Leonardo Molina (leonardomt@gmail.com).
 * @file analogSensor.ino
 * @date 2023-09-27
 * @version: 0.1.0
*/

// Max value returned by analogRead.
#define M 32767L
// Input pins.
const uint8_t pins[] = {A0, A1, A2};
// Duration of threshold calibration.
const uint8_t nIterations = 10;
const int iterationDelay = 50;

// State variables.
int R[] = { M,  M,  M};
int L[] = {-M, -M, -M};
int D[] = { 0,  0,  0};
int current = 0;
bool lastChanged = false;
bool changed;

void setup() {
	// Begin serial communication.
	Serial.begin(9600);
	
	// Setup input pins, resolution and first sample.
	for (uint8_t i = 0; i < 3; i++) {
		pinMode(pins[i], INPUT);
		R[i] = 1;
		L[i] = analogRead(pins[i]);
	}
}

void serialEvent() {
	// Find current activation range.
	if (Serial.read() == '0') {
		// Sample data for a short period.
		for (uint8_t iteration = 0; iteration < nIterations; iteration++) {
			for (uint8_t i = 0; i < 3; i++) {
				current = analogRead(pins[i]);
				if (current < R[i]) {
					// R: Smallest.
					R[i] = current;
				} else if (current > L[i]) {
					// L: Largest.
					L[i] = current;
				}
			}
			delay(iterationDelay);
		}
	}
	
	// Calculate an activation threshold.
	for (uint8_t i = 0; i < 3; i++) {
		// R: Resolution.
		R[i] = abs(L[i] - R[i]) + 1;
		// L: Last.
		L[i] = analogRead(pins[i]);
	}
}

void loop(){
	changed = false;
	for (uint8_t i = 0; i < 3; i++) {
		current = analogRead(pins[i]);
		D[i] = round((current - L[i]) / R[i]) * R[i];
		if (D[i] != 0) {
			L[i] = current;
			changed = true;
			lastChanged = true;
		}
	}
	if (changed) {
		report();
	} else if (lastChanged) {
		lastChanged = false;
		report();
	}
	
}

void report() {
	Serial.print(D[0]);
	Serial.print(',');
	Serial.print(D[1]);
	Serial.print(',');
	Serial.println(D[2]);
}