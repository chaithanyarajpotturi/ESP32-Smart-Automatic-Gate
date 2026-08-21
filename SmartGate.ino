#include <ESP32Servo.h>

Servo gateServo;

#define TRIG_PIN 5
#define ECHO_PIN 18
#define SERVO_PIN 19

const int openAngle = 90;
const int closeAngle = 0;

const int detectDistance = 20;  // Person detection distance in cm
const unsigned long closeDelay = 1000; // 1 second

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

  // Start with gate closed
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

    // Keep gate OPEN while person is present
    if (!gateOpen) {
      gateServo.write(openAngle);
      gateOpen = true;

      Serial.println("Person Detected - Gate OPEN");
    }

    // Reset closing timer
    personGoneTime = 0;
  }

  // Person not detected
  else {

    if (gateOpen) {

      // Start timer when person leaves
      if (personGoneTime == 0) {
        personGoneTime = millis();
        Serial.println("Person Gone - Waiting...");
      }

      // Close only after 3 seconds
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