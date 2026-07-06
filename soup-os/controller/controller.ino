#include <esp_now.h>
#include <WiFi.h>

// Analog stick: Y -> GPIO0, X -> GPIO1
// NOTE: this board is an ESP32-C3 Super Mini, not a XIAO - its pin
// numbers ARE the literal GPIO numbers, there's no A0/A1 alias here.
// Make sure Tools > Board is set to a plain "ESP32C3 Dev Module",
// not "XIAO_ESP32C3" - the wrong board definition silently points
// A0/A1 at the wrong physical pins.
#define JOY_Y_PIN 0
#define JOY_X_PIN 1

// Broadcast MAC - sends to any listening ESP32, no pairing needed
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Just the joystick for now - nothing else.
typedef struct struct_message {
  int joy_y;
  int joy_x;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

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

  Serial.print("Sending -> Y: "); Serial.print(myData.joy_y);
  Serial.print(" | X: "); Serial.println(myData.joy_x);

  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(50);
}
