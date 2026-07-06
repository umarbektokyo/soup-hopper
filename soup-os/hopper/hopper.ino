#include <esp_now.h>
#include <WiFi.h>

// ---- DRV8833 pins ----
// Wiring per @popo: GPIO2->AN2, GPIO3->AN1, GPIO4->BN1, GPIO5->BN2
#define AN2 2
#define AN1 3
#define BN1 4
#define BN2 5

// ---- PWM setup (ESP32 Arduino core 3.x LEDC API) ----
const int PWM_FREQ = 30000;  // 30kHz - above the range you can hear as whine
const int PWM_RES = 8;       // 8-bit duty cycle -> 0-255

const int CH_AN1 = 0;
const int CH_AN2 = 1;
const int CH_BN1 = 2;
const int CH_BN2 = 3;

// ---- Data received from the controller ----
// Just the joystick for now - nothing else.
typedef struct struct_message {
  int joy_y;
  int joy_x;
} struct_message;

struct_message myData;

// Drives one side of the DRV8833.
// speed: -255 (full reverse) .. 255 (full forward), 0 = coast
void driveMotor(int in1Pin, int in2Pin, int speed) {
  speed = constrain(speed, -255, 255);
  if (speed > 0) {
    ledcWrite(in1Pin, speed);
    ledcWrite(in2Pin, 0);
  } else if (speed < 0) {
    ledcWrite(in1Pin, 0);
    ledcWrite(in2Pin, -speed);
  } else {
    ledcWrite(in1Pin, 0);
    ledcWrite(in2Pin, 0);
  }
}

// Moves both motors together: forward (positive) or backward (negative).
// Motor B is wired/mounted reversed relative to A, so its command is
// inverted here to keep both wheels spinning the same physical direction.
void moveForwardBackward(int speed) {
  driveMotor(AN1, AN2, speed);
  driveMotor(BN1, BN2, -speed);
}

// Spins the two motors in opposite directions to rotate in place:
// right (positive) or left (negative).
void rotateLeftRight(int speed) {
  driveMotor(AN1, AN2, speed);
  driveMotor(BN1, BN2, speed);
}

// Joystick center + deadzone. The XIAO's ADC is 12-bit: 0-4095, center ~2048.
const int JOY_CENTER = 2048;
const int JOY_DEADZONE = 150;        // ignore small noise/drift around center
const int JOY_MAX_DEVIATION = 2048;  // roughly how far the stick can deviate

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  Serial.print("Received -> Y: "); Serial.print(myData.joy_y);
  Serial.print(" | X: "); Serial.println(myData.joy_x);

  int fwd = myData.joy_y - JOY_CENTER;
  int turn = myData.joy_x - JOY_CENTER;

  if (abs(fwd) < JOY_DEADZONE) fwd = 0;
  if (abs(turn) < JOY_DEADZONE) turn = 0;

  int fwdSpeed = map(fwd, -JOY_MAX_DEVIATION, JOY_MAX_DEVIATION, -255, 255);
  int turnSpeed = map(turn, -JOY_MAX_DEVIATION, JOY_MAX_DEVIATION, -255, 255);

  // Keep it simple: forward/backward takes priority, then rotate, else stop.
  if (fwdSpeed != 0) {
    moveForwardBackward(fwdSpeed);
  } else if (turnSpeed != 0) {
    rotateLeftRight(turnSpeed);
  } else {
    moveForwardBackward(0);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  ledcAttachChannel(AN1, PWM_FREQ, PWM_RES, CH_AN1);
  ledcAttachChannel(AN2, PWM_FREQ, PWM_RES, CH_AN2);
  ledcAttachChannel(BN1, PWM_FREQ, PWM_RES, CH_BN1);
  ledcAttachChannel(BN2, PWM_FREQ, PWM_RES, CH_BN2);

  moveForwardBackward(0); // make sure everything starts stopped

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Hopper ready. Waiting for controller...");
}

void loop() {
  // Everything happens in OnDataRecv when a packet arrives.
  delay(100);
}
