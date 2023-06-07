#include <esp_now.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "WiFi.h"

/*
#define WIDTH 128 
#define HEIGHT 64

#define OLED_RESET -1 
*/

int x_axes = 1;
int y_axes = 0;

// Adafruit_SSD1306 display(WIDTH, HEIGHT, &Wire, OLED_RESET);

uint8_t broadcastAddress[] = {0x84, 0xF7, 0x03, 0xa9, 0x46, 0x40};

typedef struct rc_message {
	int steering;
	int throttle;
} rc_message;

rc_message transfer_data;

esp_now_peer_info_t peerInfo;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
	Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sending OK" : "Sending failed");
}


void setup() {
	Serial.begin(115200);

	pinMode(x_axes, INPUT);
	pinMode(y_axes, INPUT);
/*
	display.clearDisplay();
	display.setTextColor(WHITE);
	display.println("Setting Up...");
	display.display();
*/

	WiFi.mode(WIFI_STA);
	
	if(esp_now_init() != ESP_OK) {
		Serial.println("Error initializing  ESP-now");
	}
	
	esp_now_register_send_cb(OnDataSent);

	memcpy(peerInfo.peer_addr, broadcastAddress, 6);
	peerInfo.channel = 0;
	peerInfo.encrypt = false;

	if (esp_now_add_peer(&peerInfo) != ESP_OK) {
		Serial.println("Failed to add peer!!");
		return;
	}
}

void loop() {
	int throttle = analogRead(y_axes);
	int steering = analogRead(x_axes);
	
	transfer_data.throttle = throttle;
	transfer_data.steering = steering;
	
	esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &transfer_data, sizeof(transfer_data));
	
	if (result == ESP_OK) {
		Serial.println("Sending confirmed!");
	}
	else {
		Serial.println("Sending error!");
	}
/*
	display.clearDisplay();
	display.setCursor(0, 10);
	display.println(String(throttle));
	display.println(String(steering));
	display.display();
	delay(20);
*/
}
