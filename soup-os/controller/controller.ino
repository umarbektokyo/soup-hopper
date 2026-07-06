#include <esp_now.h>
#include <WiFi.h>

// Define Controller Pins
#define JOY_Y_PIN A0  // Joystick Y (Wired to D0)
#define JOY_X_PIN A1  // Joystick X (Wired to D1)
#define WATER_BTN D2  // Back-Top Button

// Broadcast MAC address to avoid hardcoding
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// The data structure to transmit
typedef struct struct_message {
  int joy_y;
  int joy_x;
  bool shoot_water;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  // Use INPUT_PULLUP so the pin isn't floating. 
  // It reads LOW when you press the button.
  pinMode(WATER_BTN, INPUT_PULLUP); 

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);
}

void loop() {
  // Read joystick values (0 to 4095)
  myData.joy_y = analogRead(JOY_Y_PIN);
  myData.joy_x = analogRead(JOY_X_PIN);
  
  // Read button (LOW means pressed)
  myData.shoot_water = (digitalRead(WATER_BTN) == LOW); 

  // Send message
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(50); // Small delay to prevent network flooding
}
