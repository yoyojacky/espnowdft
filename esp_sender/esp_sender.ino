#include <esp_now.h>
#include "WiFi.h"

// MAC: 84:f7:03:a9:4c:98
uint8_t broadcastAddress[] = {0x84, 0xf7, 0x03, 0xa9, 0x46, 0x40};

typedef struct struct_message {
	int b;
	float c;
} struct_message;

// instance 
struct_message myData;

// 
esp_now_peer_info_t peerInfo;

// callback function 
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
	Serial.print("\r\nLast Packet Send Status:\t");
	Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" :
			"Delivery Fail");
}

float floatMap(float x, float in_min, float in_max, float out_min, float
		out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {

	Serial.begin(115200);

	WiFi.mode(WIFI_STA);
	
	if (esp_now_init() != ESP_OK) {
		Serial.println("Error initializing ESP-NOW");
	}

	esp_now_register_send_cb(OnDataSent);

	memcpy(peerInfo.peer_addr, broadcastAddress, 6);
	peerInfo.channel = 0;
	peerInfo.encrypt = false;

	if (esp_now_add_peer(&peerInfo) != ESP_OK) {
		Serial.println("Failed to add peer");
		return;
	}

}

void loop() {
	int analogValue = analogRead(0);
 	float voltage = floatMap(analogValue, 0, 4095, 0, 3.3);

	myData.b = 666;
	myData.c = voltage;

	esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

	if (result == ESP_OK) {
		Serial.println("Sending confirmed");
	}
	else {
		Serial.println("Sending error");
	}
	// delay(2000);
}
