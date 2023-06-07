#include <esp_now.h>
#include "WiFi.h"

// Pin number which is conncect to DRV8833 
int IN1 = 6; 
int IN2 = 4; 
int IN3 = 0; 
int IN4 = 1; 
int interval = 0;


typedef struct rc_message {
	int steering;
	int throttle;
};

rc_message transfer_data;

void init_tank() {
   pinMode(IN1, OUTPUT);
   pinMode(IN2, OUTPUT);
   pinMode(IN3, OUTPUT);
   pinMode(IN4, OUTPUT);

   digitalWrite(IN1, LOW);
   digitalWrite(IN2, LOW);
   digitalWrite(IN3, LOW);
   digitalWrite(IN4, LOW);
}

void stop(){
   digitalWrite(IN1, LOW);
   digitalWrite(IN2, LOW);
   digitalWrite(IN3, LOW);
   digitalWrite(IN4, LOW);
}

void forward(){
   digitalWrite(IN1, LOW);
   digitalWrite(IN2, HIGH);
   digitalWrite(IN3, HIGH);
   digitalWrite(IN4, LOW);
}

void backward(){
   digitalWrite(IN1, HIGH);
   digitalWrite(IN2, LOW);
   digitalWrite(IN3, LOW);
   digitalWrite(IN4, HIGH);
}

void turn_left(){
   digitalWrite(IN1, LOW);
   digitalWrite(IN2, HIGH);
   digitalWrite(IN3, LOW);
   digitalWrite(IN4, HIGH);
}

void turn_right(){
   digitalWrite(IN1, HIGH);
   digitalWrite(IN2, LOW);
   digitalWrite(IN3, HIGH);
   digitalWrite(IN4, LOW);
}
	

void OnDataResv(const uint8_t *mac, const uint8_t *incomingData, int len) {
	memcpy(&transfer_data, incomingData, sizeof(transfer_data));
	Serial.print("T: ");
	Serial.println(transfer_data.throttle);
	Serial.print("S: ");
	Serial.println(transfer_data.steering);
	if (transfer_data.throttle > 2800){
		forward();
	}
	else if (transfer_data.throttle < 2300){
		backward();
	}
	else if (transfer_data.steering < 2300){
		turn_left();
	}
	else if (transfer_data.steering > 2800){
		turn_right();
	}
	else {
		stop();
	}
}


void setup() {
	Serial.begin(115200);
	
	init_tank();

	WiFi.mode(WIFI_STA);
	
	if(esp_now_init() != ESP_OK) {
		Serial.println("Error initializing  ESP-now");
	}
	
	esp_now_register_recv_cb(OnDataResv);
	
}

void loop() {
}

