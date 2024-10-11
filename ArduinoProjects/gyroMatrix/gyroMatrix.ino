#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ledcontrol.h>

#define LED_PIN1 5
#define LED_PIN2 6
#define LED_PIN3 7

Adafruit_MPU6050 mpu;

const int DIN_PIN = 11; 
const int CLK_PIN = 13; 
const int CS_PIN = 10;
 
// Define the number of dot matrix modules in your setup 
const int NUM_MODULES = 1; 

// Create an instance of the LedControl library 
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, NUM_MODULES);

void setup() {
  //Wire.begin();
  Serial.begin(9600);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
    
    lc.shutdown(0, false); 
    lc.setIntensity(0, 8);
    lc.clearDisplay(0);

  if (!mpu.begin()) {
    Serial.println("Failed to initialize MPU6050 sensor!");
    while (1);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a,&g,&temp);
  
  float xa = a.acceleration.x;
  float ya = a.acceleration.y;
  
  Serial.println(xa);
    
  if(-8<=xa<-6){
        lc.setLed(NUM_MODULES,0,0,true);
  }else if(-6<=xa<-4){
        lc.setLed(NUM_MODULES,0,1,true);
  }else if(-4<=xa<2){
        lc.setLed(NUM_MODULES,0,2,true);
  }else if(-2<=xa<0){
        lc.setLed(NUM_MODULES,0,3,true);
  }else if(0<xa<=2){
        lc.setLed(NUM_MODULES,0,4,true);
  }else if(2<xa<=4){
        lc.setLed(NUM_MODULES,0,5,true);
  }else if(4<xa<=6){
        lc.setLed(NUM_MODULES,0,6,true);
  }else if(6<xa<=8){
        lc.setLed(NUM_MODULES,0,7,true);
  }else{
        lc.setLed(NUM_MODULES,0,7,false);
        lc.setLed(NUM_MODULES,0,6,false);
        lc.setLed(NUM_MODULES,0,5,false);
        lc.setLed(NUM_MODULES,0,4,false);
        lc.setLed(NUM_MODULES,0,3,false);
        lc.setLed(NUM_MODULES,0,2,false);
        lc.setLed(NUM_MODULES,0,1,false);
        lc.setLed(NUM_MODULES,0,0,false);
        }
    
        lc.setLed(NUM_MODULES,0,7,false);
        lc.setLed(NUM_MODULES,0,6,false);
        lc.setLed(NUM_MODULES,0,5,false);
        lc.setLed(NUM_MODULES,0,4,false);
        lc.setLed(NUM_MODULES,0,3,false);
        lc.setLed(NUM_MODULES,0,2,false);
        lc.setLed(NUM_MODULES,0,1,false);
        lc.setLed(NUM_MODULES,0,0,false);
    
  delay(100);
}
