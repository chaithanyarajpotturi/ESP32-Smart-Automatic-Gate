# 🚪 ESP32 Smart Automatic Gate

A simple automatic gate project using ESP32, HC-SR04 Ultrasonic Sensor, and SG90 Servo Motor.

## 🎥 Project Demo

YouTube Video:
https://youtu.be/t9ygYz_aVdY

## 🛠️ Components Used

- ESP32 DevKit V1
- HC-SR04 Ultrasonic Sensor
- SG90 Micro Servo Motor
- 1kΩ Resistor
- 10kΩ Resistor
- Breadboard
- Jumper Wires
- USB Cable

## 🔌 Circuit Connections

### HC-SR04 Ultrasonic Sensor

| HC-SR04 | ESP32 |
|---|---|
| VCC | VIN / 5V |
| GND | GND |
| TRIG | GPIO 5 |
| ECHO | GPIO 18 |

> ⚠️ HC-SR04 ECHO is 5V. Use a voltage divider before connecting ECHO to the ESP32 GPIO.

### SG90 Servo Motor

| Servo Wire | ESP32 |
|---|---|
| Brown / Black | GND |
| Red | VIN / 5V |
| Orange / Yellow | GPIO 19 |

## ⚙️ Working

1. The ultrasonic sensor measures the distance.
2. When a person comes within 20 cm, the gate opens.
3. The servo motor rotates to 90°.
4. While the person is detected, the gate remains open.
5. When the person moves away, the system waits for 1 second.
6. The gate then automatically closes.
7. The servo returns to 0°.

## 💻 Software

- Arduino IDE
- ESP32 Board Package
- ESP32Servo Library

## 📂 Project File

The Arduino code is available here:

`SmartGate/SmartGate.ino`

## 🚀 Future Improvements

- Add IR sensors
- Add LCD display
- Add RFID access control
- Add Wi-Fi control
- Add mobile app control
- Add automatic gate status monitoring

## 🎬 YouTube

Watch the complete project demonstration:

https://youtu.be/t9ygYz_aVdY

## 👨‍💻 Author

**Chaithanya Raju Potturi**

B.Tech – Electronics and Communication Engineering

Embedded Systems / ESP32 Projects

---

⭐ If you find this project useful, consider giving the repository a star!


Arduino Code


#include <ESP32Servo.h>

Servo gateServo;

#define TRIG_PIN 5
#define ECHO_PIN 18
#define SERVO_PIN 19

const int openAngle = 90;
const int closeAngle = 0;

const int detectDistance = 20;
const unsigned long closeDelay = 1000;

bool gateOpen = false;
unsigned long personGoneTime = 0;

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return 999;
  }

  return duration * 0.034 / 2;
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  gateServo.attach(SERVO_PIN);

  gateServo.write(closeAngle);
  gateOpen = false;

  Serial.println("Smart Gate Started");
}

void loop() {

  long distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Person detected
  if (distance <= detectDistance) {

    if (!gateOpen) {
      gateServo.write(openAngle);
      gateOpen = true;

      Serial.println("Person Detected - Gate OPEN");
    }

    personGoneTime = 0;
  }

  // Person not detected
  else {

    if (gateOpen) {

      if (personGoneTime == 0) {
        personGoneTime = millis();
        Serial.println("Person Gone - Waiting...");
      }

      if (millis() - personGoneTime >= closeDelay) {
        gateServo.write(closeAngle);
        gateOpen = false;
        personGoneTime = 0;

        Serial.println("Gate CLOSED");
      }
    }
  }

  delay(100);
}
