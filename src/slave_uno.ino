#include <Wire.h>
#include <Arduino.h>

const int ADDRESS = 0;

const int speedPin = A0;
const int tempPin = A1;

const int brakeSwitchPin = 2;
const int SwitchPin = 3;
const int alarmPin = 4;

const int brakeLED =  ;
const int speedLED = 5;
const int invasionLED = 8;
const int tempLED = 6;

int s = 0, s_old = 0, s_new = 0;
int temp = 0, temp_old = 0, temp_new = 0;

bool overspeed;
bool brakeState;
bool invasion;
bool overtemp;
bool button2;

void serialOut();

void setup() {
	Wire.begin(ADDRESS);

	Wire.onRequest(requestEvent);
	Wire.onReceive(receiveEvent);
	Serial.begin(9600);

	pinMode(brakeSwitchPin, INPUT);
	pinMode(SwitchPin, INPUT);

	pinMode(alarmPin, OUTPUT);
	pinMode(brakeLED, OUTPUT);
	pinMode(speedLED, OUTPUT);
	pinMode(invasionLED, OUTPUT);
	pinMode(tempLED, OUTPUT);
}

void loop() {
	s_new = map(analogRead(speedPin), 0, 1024, 0, 255);
	temp_new = map(analogRead(tempPin), 0, 1024, 0, 255);
	brakeState = digitalRead(brakeSwitchPin);

	if (abs(s_new - s) > 20) {
		s = s_old;
	} else {
		s = s_new;
	}

	if (abs(temp_new - temp) > 20) {
		temp = temp_old;
	} else {
		temp = temp_new;
	}

	digitalWrite(brakeLED, brakeState);
	digitalWrite(speedLED, overspeed);
	digitalWrite(tempLED, overtemp);
	digitalWrite(invasionLED, invasion);
	digitalWrite(alarmPin, overspeed || overtemp || invasion || brakeState);

//	delay(10);
	s_old = s;
	temp_old = temp;
}

void requestEvent() {
	serialOut();

	Wire.write(s);
	Wire.write(temp);
	Wire.write(brakeState);
	Wire.write(button2);
}

void receiveEvent(int num) {
	overspeed = Wire.read();
	overtemp = Wire.read();
	invasion = Wire.read();
}

void serialOut() {
	Serial.print("Send speed:");
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
