// Pin definitions for DRV8833 connected to Arduino Nano

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);  // RX, TX pins

#define InPutPin A0
const int BIN1 = 5;        // Motor control pin 1 (PWM-capable pin)
const int BIN2 = 6;        // Motor control pin 2 (PWM-capable pin)
const int DELAY_TIME = 2;  // Delay time in milliseconds
int current1 = 0;          // Current motor speed
int New_Val = 0;           // Target speed from Serial input

void RunMotors(int speed) {
  if (speed < 0) {
    analogWrite(BIN1, -speed);  // Apply PWM signal to BIN1
    digitalWrite(BIN2, LOW);    // Turn BIN2 off for backward direction
  } else if (speed > 0) {
    digitalWrite(BIN1, LOW);   // Turn BIN1 off for forward direction
    analogWrite(BIN2, speed);  // Apply PWM signal to BIN2
  } else {
    digitalWrite(BIN1, LOW);  // Stop the motor
    digitalWrite(BIN2, LOW);  // Stop the motor
  }
}

void speedChange(int val) {

  val = (val < -255) ? -255 : (val > 255 ? 255 : val);

  while (current1 != val) {
    // Gradually adjust current1 towards val
    if (current1 < val) {
      current1++;
    } else if (current1 > val) {
      current1--;
    }
    RunMotors(current1);
    // Serial.println(current1);
    delay(DELAY_TIME);

    // Check for new input during speed change
    if (Serial.available()) {
      String s = Serial.readString();
      New_Val = s.toInt();
      Serial.println("New target received: " + String(New_Val));
      return;  // Exit speedChange to process new input
    }
  }
}

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);
  mySerial.begin(9600);

  // Set the motor control pins as outputs
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Print instructions
  Serial.println("Enter a speed (-255 to 255):");
}

void loop() {
  // Check for new serial input
  if (Serial.available()) {
    String s = Serial.readString();
    New_Val = s.toInt();
    Serial.println("Target speed: " + String(New_Val));
  }
  if (mySerial.available()) {
    New_Val = mySerial.parseInt();  // Parse integer value
    Serial.print("Received Value: " + New_Val);
  }
  // Adjust speed to target value
  speedChange(New_Val);
}
