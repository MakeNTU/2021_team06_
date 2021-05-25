#include <Wire.h>
#include <Arduino.h>

const int MAXTRAIN = 2;

int s;
int temp;
int speed_limit = 150;
int temp_limit = 150;
bool overspeed;
bool brakeState;
bool invasion;
bool overtemp;

void serialOut(int i);

void setup() {
	Wire.begin();
	Serial.begin(9600);
}

void loop() {
	if (Serial.available()) {
		char c;
		int a = 0;
		while ((c = Serial.read()) != ',') {
			a *= 10;
			a += c - '0';
		}
		invasion = Serial.read() - '0';
		speed_limit = a;

    Serial.read();

//		Serial.print(a);
//		Serial.println(invasion);
	}

	for (int i = 0; i < MAXTRAIN; i++) {
		Wire.requestFrom(i, 4);

		s = Wire.read();
		temp = Wire.read();
		brakeState = Wire.read();

		if (s >= speed_limit) {
			overspeed = true;
		} else {
			overspeed = false;
		}

		if (temp >= 150) {
			overtemp = true;
		} else {
			overtemp = false;
		}

    rawOut(i);
		Wire.beginTransmission(i);
		Wire.write(overspeed);
		Wire.write(overtemp);
		Wire.write(invasion);
		Wire.endTransmission();
	}
	delay(100);
}

void serialOut(int i) {
	Serial.print(i);
	Serial.print("Receive speed:");
	Serial.print(s);
	Serial.print(", Temperature:");
	Serial.print(temp);
	Serial.print(", BrakeState:");
	Serial.print(brakeState);
	Serial.print(", Overspeed:");
	Serial.print(overspeed);
	Serial.print(", Overtemp:");
	Serial.print(overtemp);
	Serial.print(", Invasion:");
	Serial.println(invasion);
}

void rawOut(int i) {
	Serial.print(i);
	Serial.print(",");
	Serial.print(s);
	Serial.print(",");
	Serial.print(temp);
	Serial.print(",");
	Serial.print(brakeState);
	Serial.print(",");
	Serial.print(overspeed);
	Serial.print(",");
	Serial.print(overtemp);
	Serial.print(",");
	Serial.print(invasion);
	Serial.print('\n');
}
