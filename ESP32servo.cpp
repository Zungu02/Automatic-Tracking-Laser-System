#include <SCServo.h>
SMS_STS st;

#define S_RXD 18
#define S_TXD 19

#define PAN_ID 2 
#define TILT_ID 1 

// UART + tracking timeout
unsigned long lastCommandTime = 0;
const unsigned long commandTimeout = 1000;  // 1 second
String inputString = "";
bool trackingActive = false;

// Pan configuration
float panDegrees = 0;
float panTarget = 45;
float panSpeed = 0.2;
const float PAN_MIN = 0;
const float PAN_MAX = 45;
unsigned long lastPanUpdate = 0;
unsigned long panUpdateInterval = 40;

// Tilt configuration
float tiltDegrees = 110;
float tiltTarget = 120;
float tiltSpeed = 0.1;  // Slower than pan
unsigned long lastTiltUpdate = 0;
unsigned long tiltUpdateInterval = 30;

// Tilt pattern
int tiltPattern[] = {110, 115, 120, 115};
const int TILT_PATTERN_LENGTH = sizeof(tiltPattern) / sizeof(tiltPattern[0]);
int tiltPhase = 0;

// Convert degrees (0–240) to servo units (0–4095)
int degreeToServoPos(float deg) {
  return map((int)deg, 0, 240, 0, 4095);
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(1000000, SERIAL_8N1, S_RXD, S_TXD);  // Match servo UART baud
  st.pSerial = &Serial1;

  Serial.println("ESP32 Servo Tracker with fallback sweep mode");

  st.WritePosEx(PAN_ID, degreeToServoPos(panDegrees), 1000, 50);
  st.WritePosEx(TILT_ID, degreeToServoPos(tiltDegrees), 1000, 50);
  delay(2000);
}

void loop() {
  unsigned long now = millis();

  // ----------- UART Listening -----------
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      inputString.trim();

      if (inputString.startsWith("POS:")) {
        int commaIndex = inputString.indexOf(',');
        if (commaIndex > 4) {
          int cx = inputString.substring(4, commaIndex).toInt();
          int cy = inputString.substring(commaIndex + 1).toInt();

          // Map camera (640x480) to pan/tilt
          float mappedPan = map(cx, 0, 640, PAN_MIN, PAN_MAX);
          float mappedTilt = map(cy, 0, 480, 110, 120);  // still smooth tilt

          st.WritePosEx(PAN_ID, degreeToServoPos(mappedPan), 1000, 50);
          st.WritePosEx(TILT_ID, degreeToServoPos(mappedTilt), 1000, 50);

          lastCommandTime = now;
          trackingActive = true;
        }
      } else if (inputString == "IDLE") {
        trackingActive = false;
      }

      inputString = "";
    } else {
      inputString += c;
    }
  }

  // ---------- Fallback Sweep (if idle too long) ----------
  if (!trackingActive || (now - lastCommandTime > commandTimeout)) {
    trackingActive = false;

    // Smooth Pan
    if (now - lastPanUpdate >= panUpdateInterval) {
      lastPanUpdate = now;

      if (abs(panDegrees - panTarget) < 0.5) {
        panTarget = (panTarget == PAN_MAX) ? PAN_MIN : PAN_MAX;
      }

      panDegrees += (panDegrees < panTarget) ? panSpeed : -panSpeed;

      st.WritePosEx(PAN_ID, degreeToServoPos(panDegrees), 1000, 50);
    }

    // Smooth Tilt
    if (now - lastTiltUpdate >= tiltUpdateInterval) {
      lastTiltUpdate = now;

      if (abs(tiltDegrees - tiltTarget) < 0.3) {
        tiltPhase = (tiltPhase + 1) % TILT_PATTERN_LENGTH;
        tiltTarget = tiltPattern[tiltPhase];
      }

      tiltDegrees += (tiltDegrees < tiltTarget) ? tiltSpeed : -tiltSpeed;

      st.WritePosEx(TILT_ID, degreeToServoPos(tiltDegrees), 1000, 50);

      // Debug
      Serial.print("Pan: ");
      Serial.print(panDegrees, 1);
      Serial.print("° | Tilt: ");
      Serial.print(tiltDegrees, 1);
      Serial.println("°");
    }
  }
}
