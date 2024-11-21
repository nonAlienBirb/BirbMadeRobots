# Overview  
_This Python-Arduino project uses a Bluetooth connection to a PC or laptop, allowing movement commands to be sent via the keyboard._  

The goal was to create a car controlled like those in games, with a real-time front-view feed displayed on the laptop using an ESP32-Cam. This setup offers a gaming-like experience with a robot car!  

## Ingredients  

While any motor control module can be used, I chose the L298n motor driver, so the code is tailored for it.  

### Hardware  

#### Modules  
- Arduino Pro Mini  
- HC-05 Bluetooth Module  
- L298n Motor Driver Module  

#### Motors  
- Yellow gearbox 6V motors (x2)  

#### Battery  
- 12V Lithium Battery  

### Software  

#### Python Controller Libraries  
- `pySerial`  
- `keyboard`  
