#include <SoftwareSerial.h>
// #include "LedControl.h"

// LedControl lc = LedControl(11, 12, 10, 1);

SoftwareSerial bluetooth(3, 2);

const int leftLed = 6;
const int leftInOne = 4;
const int leftInTwo = 7;

const int rightLed = 5;
const int rightInOne = 9;
const int rightInTwo = 8;

bool gotMove = false;
bool goForward = false;
bool goBackward = false;
bool rotateLeft = false;
bool rotateRight = false;
bool got0 = false;
bool got1 = false;
bool turn = false;

int current1 = 255;
int current2 = 255;
int Rs;
int Ls;

int nw = 100;
const int c = 20;

char x;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  bluetooth.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(rightLed, OUTPUT);
  pinMode(rightInOne, OUTPUT);
  pinMode(rightInTwo, OUTPUT);
  pinMode(leftInOne, OUTPUT);
  pinMode(leftInTwo, OUTPUT);
  pinMode(leftLed, OUTPUT);
  // lc.shutdown(0, false);
  // lc.setIntensity(0, 8);
  // lc.clearDisplay(0);
}

// void smiley() {

//   byte smiley[8] = {
//     B00111100,
//     B01000010,
//     B10100101,
//     B10000001,
//     B10100101,
//     B10011001,
//     B01000010,
//     B00111100,
//   };
//   lc.setRow(0, 0, smiley[0]);
//   lc.setRow(0, 1, smiley[1]);
//   lc.setRow(0, 2, smiley[2]);
//   lc.setRow(0, 3, smiley[3]);
//   lc.setRow(0, 4, smiley[4]);
//   lc.setRow(0, 5, smiley[5]);
//   lc.setRow(0, 6, smiley[6]);
//   lc.setRow(0, 7, smiley[7]);
// }

void toWhatDirection() {

  if (goForward || (goForward && (rotateRight || rotateLeft))) {
    digitalWrite(rightInOne, HIGH);
    digitalWrite(rightInTwo, LOW);
    digitalWrite(leftInOne, HIGH);
    digitalWrite(leftInTwo, LOW);
    turn = true;

  } else if (goBackward || (goBackward && (rotateRight || rotateLeft))) {
    digitalWrite(rightInOne, LOW);
    digitalWrite(rightInTwo, HIGH);
    digitalWrite(leftInOne, LOW);
    digitalWrite(leftInTwo, HIGH);
    turn = true;

  } else if (rotateLeft && !(goForward || goBackward)) {
    digitalWrite(rightInOne, HIGH);
    digitalWrite(rightInTwo, LOW);
    digitalWrite(leftInOne, LOW);
    digitalWrite(leftInTwo, HIGH);
    turn = false;

  } else if (rotateRight && !(goForward || goBackward)) {
    digitalWrite(rightInOne, LOW);
    digitalWrite(rightInTwo, HIGH);
    digitalWrite(leftInOne, HIGH);
    digitalWrite(leftInTwo, LOW);
    turn = false;
  }
}

void setSpeed() {
  int c2;
  c2 = (x == '2') ? c : (x == '8') ? -c
                                   : 0;
  nw = nw + c2;
  nw = (nw >= 255) ? 255 : (nw <= 0) ? 0
                                     : nw;
}

void speedChange(int val, int val2) {
  while ((current1 != val) || (current2 != val2)) {

    if (current1 < val) {
      current1++;
    } else if (current1 > val) {
      current1--;
    }
    if (current2 < val2) {
      current2++;
    } else if (current2 > val2) {
      current2--;
    }
    analogWrite(rightLed, current1);
    analogWrite(leftLed, current2);
    delay(2);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  // if (Serial.available() > 0) {
  //   String s = Serial.readString();
  //   nw = s.toInt();
  // }

  if (bluetooth.available() > 0) {
    x = bluetooth.read();
    Serial.println(x);

    switch (x) {
      case 'w':
        goForward = true;
        goBackward = false;
        gotMove = true;
        break;
      case 's':
        goForward = false;
        goBackward = true;
        gotMove = true;
        break;
      case '0':
        // if ((goBackward && (rotateRight || rotateLeft)) || (goForward && (rotateRight || rotateLeft))) {
        //   goForward = false;
        //   goBackward = false;

        //   if (rotateRight) {
        //     speedChange(0, Ls);
        //     // if (goForward) {
        //     //   digitalWrite(rightInOne, LOW);
        //     //   digitalWrite(rightInTwo, HIGH);
        //     //   // digitalWrite(leftInOne, HIGH);
        //     //   // digitalWrite(leftInTwo, LOW);
        //     // } else if (goBackward) {
        //     //   digitalWrite(rightInOne, HIGH);
        //     //   digitalWrite(rightInTwo, LOW);
        //     //   digitalWrite(leftInOne, LOW);
        //     //   digitalWrite(leftInTwo, HIGH);
        //     // }
        //     // toWhatDirection();
        //   } else if (rotateLeft) {
        //     speedChange(Rs, 0);
        //     // toWhatDirection();
        //     // if (goForward) {
        //     //   digitalWrite(rightInOne, HIGH);
        //     //   digitalWrite(rightInTwo, LOW);
        //     //   digitalWrite(leftInOne, LOW);
        //     //   digitalWrite(leftInTwo, HIGH);
        //     // } else if (goBackward) {
        //     //   digitalWrite(rightInOne, LOW);
        //     //   digitalWrite(rightInTwo, HIGH);
        //     //   digitalWrite(leftInOne, HIGH);
        //     //   digitalWrite(leftInTwo, LOW);
        //     // }
        //   }
        //   // int m = max(Rs, Ls);
        //   // speedChange(m, m);
        //   turn = false;
        // } else {
        //   speedChange(0, 0);
        //   gotMove = false;
        // }
        speedChange(0, 0);
        goForward = false;
        goBackward = false;
        rotateLeft = false;
        rotateRight = false;
        gotMove = false;
        break;
        
      case 'a':
        rotateLeft = true;
        rotateRight = false;
        gotMove = true;
        break;
      case 'd':
        rotateRight = true;
        rotateLeft = false;
        gotMove = true;
        break;
      case '1':
        rotateLeft = false;
        rotateRight = false;
        if (!turn) {
          speedChange(0, 0);
          gotMove = false;
        } else {
          int m = max(Rs, Ls);
          speedChange(m, m);
        }
        break;
      default:
        setSpeed();
        break;
    }

    // bluetooth.println(current1);
    // bluetooth.println(current2);
  }

  if ((goForward && rotateRight) || (goBackward && rotateRight)) {
    Rs = nw * 0.5;
    Ls = nw;
  } else if ((goForward && rotateLeft) || (goBackward && rotateLeft)) {
    Rs = nw;
    Ls = nw * 0.5;
  } else {
    Rs = nw;
    Ls = nw;
  }

  if (gotMove) {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
    toWhatDirection();
    speedChange(Rs, Ls);
  }
}
