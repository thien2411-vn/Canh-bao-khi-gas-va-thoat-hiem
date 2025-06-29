// Blynk cấu hình
#define BLYNK_TEMPLATE_ID "TMPL6O0Hlfghu" //thay đổi địa chỉ Blynk IoT
#define BLYNK_TEMPLATE_NAME "Cảnh báo khí gas và thoát hiểm"
#define BLYNK_AUTH_TOKEN "EviL06cjkRD4aCNOlwPNIbxlUdJtU_f4"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <math.h>
#include <ESP32Servo.h>

char ssid[] = "92"; //thay đổi địa chỉ wifi
char pass[] = "1234567891011"; // thay đổi mật khẩu wifi

// Chân kết nối phần cứng
#define MQ2_PIN     34
#define RELAY_PIN   26
#define SERVO_PIN   25
#define BUZZER_PIN  27
#define LED_PIN     14

Servo myServo;

// MQ2 gas
const float RL = 10.0;
const float Ro = 10.0;
const float ppmThreshold = 200.0;

// Servo
int currentAngle = 180;
int targetAngle = 180;
unsigned long previousServoMove = 0;
const int servoSpeed = 10;

// Cảnh báo
bool alertState = false;
bool blinkState = false;

unsigned long previousGasRead = 0;
unsigned long previousBlink = 0;
const long gasReadInterval = 1000;
const long blinkInterval = 1000;

// Blynk
bool manualServoControl = false; // true nếu đang bật V1 switch

// Led chớp tắt kết nối
unsigned long lastLedBlink = 0;
bool ledBlinkState = false;

// Gửi dữ liệu gas lên V0
float calculateGasPPM() {
  int adc = analogRead(MQ2_PIN);
  float voltage = adc * 5 / 4095.0;
  if (voltage == 0) return 0;
  float Rs = ((5 - voltage) / voltage) * RL;
  float ratio = Rs / Ro;
  float ppm = 370.0 * pow(ratio, -1.38);
  return ppm;
}

// Làm mượt servo
void smoothServoMove() {
  if (millis() - previousServoMove >= servoSpeed) {
    previousServoMove = millis();
    if (currentAngle < targetAngle) {
      currentAngle++;
      myServo.write(currentAngle);
    } else if (currentAngle > targetAngle) {
      currentAngle--;
      myServo.write(currentAngle);
    }
  }
}

// Điều khiển servo từ Blynk (V1)
BLYNK_WRITE(V1) {
  int state = param.asInt();
  manualServoControl = (state == 1);
  if (manualServoControl) {
    targetAngle = 90;
  } else {
    targetAngle = 180;
  }
}

// Setup
void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(currentAngle);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

// Loop
void loop() {
  Blynk.run();

  unsigned long currentMillis = millis();

  // Gửi giá trị gas và kiểm tra cảnh báo mỗi giây
  if (currentMillis - previousGasRead >= gasReadInterval) {
    previousGasRead = currentMillis;

    float gasPPM = calculateGasPPM();
    Serial.print("Gas PPM: ");
    Serial.println(gasPPM, 2);

    Blynk.virtualWrite(V0, gasPPM);

    if (!manualServoControl) {
      if (gasPPM > ppmThreshold && !alertState) {
        alertState = true;
        digitalWrite(RELAY_PIN, HIGH);
        targetAngle = 90;
      } else if (gasPPM <= ppmThreshold && alertState) {
        alertState = false;
        digitalWrite(RELAY_PIN, LOW);
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(LED_PIN, LOW);
        targetAngle = 180;
      }
    }
  }

  smoothServoMove();

  if (alertState && (currentMillis - previousBlink >= blinkInterval)) {
    previousBlink = currentMillis;
    blinkState = !blinkState;
    digitalWrite(BUZZER_PIN, blinkState);
    digitalWrite(LED_PIN, blinkState);
  }

  // Chớp tắt LED ảo V2 báo kết nối
  if (millis() - lastLedBlink >= 500) {
    lastLedBlink = millis();
    ledBlinkState = !ledBlinkState;
    Blynk.virtualWrite(V2, ledBlinkState);
  }
}
