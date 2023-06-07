// testing joystick 


int x_axes = 1;
int y_axes = 0;

void setup() {
    pinMode(0, INPUT);
    pinMode(1, INPUT);
   
    Serial.begin(115200);
    Serial.println("Setting up...");
    delay(2000);
}

void loop() {
    Serial.print("x_axes:");
    Serial.println(analogRead(x_axes));
    Serial.print("y_axes:");
    Serial.println(analogRead(y_axes));
    Serial.println("----------------------");
    delay(2);
}
