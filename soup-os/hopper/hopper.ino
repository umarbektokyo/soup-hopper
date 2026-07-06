#include <esp_now.h>
#include <WiFi.h>

// Wiring (per @popo):
// Joystick Y  -> GPIO0 = A0
// Joystick X  -> GPIO1 = A1
// Back-Up btn -> GPIO2 = D2  (this is the "back-top" button used for water)
#define JOY_Y_PIN A0
#define JOY_X_PIN A1
#define WATER_BTN D2

// Broadcast MAC - sends to any listening ESP32, no pairing needed
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

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
  pinMode(WATER_BTN, INPUT_PULLUP);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Controller ready. Starting transmission...");
}

void loop() {
  myData.joy_y = analogRead(JOY_Y_PIN);
  myData.joy_x = analogRead(JOY_X_PIN);
  myData.shoot_water = (digitalRead(WATER_BTN) == LOW);

  Serial.print("Y: "); Serial.print(myData.joy_y);
  Serial.print(" | X: "); Serial.print(myData.joy_x);
  Serial.print(" | Water: "); Serial.println(myData.shoot_water ? "ON" : "OFF");

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  if (result != ESP_OK) {
    Serial.println("[Status: Send FAILED]");
  }

  delay(50);
}