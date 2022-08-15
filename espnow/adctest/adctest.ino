float floatMap(float x, float in_min, float in_max, float out_min, float
		out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
	Serial.begin(9600);
}

void loop() {
	int analogValue = analogRead(0);
	float voltage = floatMap(analogValue, 0, 4095, 0, 3.3);
	Serial.print("Analog: ");
	Serial.println(analogValue);
	Serial.print(", Voltage: ");
	Serial.println(voltage);
}
