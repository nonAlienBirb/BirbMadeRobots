#include <SPI.h>
#include <printf.h>
#include <RF24.h>
#include <SoftwareSerial.h>

#define CE_PIN 7
#define CSN_PIN 8

#define ledCheckPin 13

#define OutPutPin A0

SoftwareSerial mySerial(2, 3);

RF24 radio(CE_PIN, CSN_PIN);

uint8_t address[][6] = { "1Node", "2Node" };

bool radioNumber = 0;

int payload[3] = { 0 };

void setRF() {
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {
      digitalWrite(ledCheckPin, HIGH);
      delay(100);
      digitalWrite(ledCheckPin, LOW);
      delay(200);
    }
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

  Serial.println("Starting...");
  delay(1000);

  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(sizeof(payload));
  radio.openWritingPipe(address[radioNumber]);
  radio.openReadingPipe(1, address[!radioNumber]);
  radio.startListening();
}

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);

  pinMode(OutPutPin, OUTPUT);
}

void loop() {
  uint8_t pipe;
  if (radio.available(&pipe)) {
    uint8_t bytes = radio.getPayloadSize();  // get the size of the payload
    radio.read(&payload, sizeof(payload));   // Serial.print(F("Received payload: ["));
    Serial.print("recived : ");
    Serial.print("X : ");
    Serial.print(payload[0]);
    Serial.print("  Y : ");
    Serial.print(payload[1]);
    Serial.print("  mod : ");
    Serial.println(payload[2]);

    mySerial.println(payload[0]);
  }
  delay(50);
}
