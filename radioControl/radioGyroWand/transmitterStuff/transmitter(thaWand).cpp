#include <SPI.h>
#include "RF24.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define CE_PIN 8
#define CSN_PIN 10

#define ledCheckPin 2

RF24 radio(CE_PIN, CSN_PIN);

Adafruit_MPU6050 mpu;

uint8_t address[][6] = { "1Node", "2Node" };

bool radioNumber = 1;

int payload[3] = { 0, 0, 0 };

bool mode = true;
bool notPressed = true;

void setAndCheckMPU() {

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      digitalWrite(ledCheckPin, HIGH);
      delay(250);
      digitalWrite(ledCheckPin, LOW);
      delay(250);
    }
  }
  Serial.println("MPU6050 Found!");

  digitalWrite(ledCheckPin, HIGH);
  delay(200);
  digitalWrite(ledCheckPin, LOW);
  delay(200);
  digitalWrite(ledCheckPin, HIGH);
  delay(200);
  digitalWrite(ledCheckPin, LOW);

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void SetRF() {
  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {
      digitalWrite(ledCheckPin, HIGH);
      delay(100);
      digitalWrite(ledCheckPin, LOW);
      delay(200);
    }
    Serial.println("radio hardware is found.");
    delay(500);
    digitalWrite(ledCheckPin, HIGH);
    delay(100);
    digitalWrite(ledCheckPin, LOW);
    delay(100);
    digitalWrite(ledCheckPin, HIGH);
    delay(100);
    digitalWrite(ledCheckPin, LOW);
  }  // hold in infinite loop

  Serial.println(F("RF24 GettingStarted"));
  delay(1000);
  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(sizeof(payload));
  radio.openWritingPipe(address[radioNumber]);
  radio.openReadingPipe(1, address[!radioNumber]);
  radio.stopListening();
}

void monitor(bool report, unsigned long end_timer, unsigned long start_timer) {
  if (report) {
    Serial.print(F("Transmited: "));
    Serial.print("X : ");
    Serial.print(payload[0]);
    Serial.print("  Y : ");
    Serial.print(payload[1]);
    Serial.print("  mod : ");
    Serial.print(payload[2]);
    Serial.print(F(" in "));
    Serial.print(end_timer - start_timer);  // print the timer result
    Serial.println(F(" uS"));
  } else {
    Serial.println(F("Transmission failed or timed out"));  // payload was not delivered
  }
  Serial.print("X : ");
  Serial.print(payload[0]);
  Serial.print("  Y : ");
  Serial.print(payload[1]);
  Serial.print("  mod : ");
  Serial.println(payload[2]);
  Serial.print(F(" in "));
  Serial.print(end_timer - start_timer);  // print the timer result
  Serial.println(F(" uS"));
}

void setup() {

  Serial.begin(115200);
  while (!Serial) {
  }
  pinMode(4, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(ledCheckPin, OUTPUT);

  setAndCheckMPU();
  SetRF();
}

void loop() {

  // This device is a TX node
  int s = digitalRead(4);

  if (s == 0 && notPressed) {
    notPressed = false;
    mode = !mode;
    delay(5);
  } else if (s == 1) {
    notPressed = true;
  }

  switch (mode) {
    case 1:
      digitalWrite(ledCheckPin, LOW);
      payload[0] = map(analogRead(A0), 0, 1023, -255, 255);
      payload[1] = map(analogRead(A1), 0, 1023, -100, 100);
      break;
    case 0:
      digitalWrite(ledCheckPin, HIGH);
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);
      payload[0] = map(a.acceleration.x, -9, 9, -255, 255);
      payload[1] = map(a.acceleration.y, -9, 9, -100, 100);

      break;
  }
  payload[0] = payload[0] > 255 ? 255 : payload[0] < -255                                                                       ? -255
                                      : (((payload[0] <= 10) && (payload[0] > 0)) || ((payload[0] >= -10) && (payload[0] < 0))) ? 0
                                                                                                                                : payload[0];
  payload[1] = payload[1] > 100 ? 100 : payload[1] < -100                                                                       ? -100
                                      : (((payload[1] <= 10) && (payload[1] > 0)) || ((payload[1] >= -10) && (payload[1] < 0))) ? 0
                                                                                                                                : payload[1];
  payload[2] = mode;

  unsigned long start_timer = micros();                  // start the timer
  bool report = radio.write(&payload, sizeof(payload));  // transmit & save the report
  unsigned long end_timer = micros();                    // end the timer


  // to make this example readable in the serial monitor
  // monitor(report, end_timer, start_timer);
  delay(50);  // slow transmissions down by 50 ms
}
// loop
