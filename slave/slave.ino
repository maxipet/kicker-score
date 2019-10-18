// Includes
#include <Wire.h>

// Pins
#define pinLight 2

// Light
bool light_sensor = false;

// Scoring
int score = 0;
int score_status = 0;
int score_threshold = 3;
int score_pause = 1000;
unsigned long last_score = 0;

void setup() {
  pinMode(2, INPUT);
  Serial.begin(9600);
  
  Wire.begin(8);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}

void loop() {
  // Light Sensor update
  checkSensor();

  // Scoring
  if(light_sensor && millis() - last_score > score_pause) {
    addScore();
  }
}

// Updates the sensor
void checkSensor() {
  if(digitalRead(pinLight) == HIGH) {
    light_sensor = true;
  } else {
    light_sensor = false;
  }
}

void requestEvent() {
  Wire.write(score_status);
  score_status = 0;
}

void receiveEvent(int howMany) {
  while(Wire.available()) {
    int data = Wire.read();
    processData(data);
  }
}

void processData(int data) {
  if(data == -1) {
    reset();
  } else if(data >= 0 && data <= 10) {
    setScore(data);
  }
}

void setScore(int s) {
  score = s;
}

void addScore() {
  score_status = 1;
}

void subScore() {
  score_status = -1;
}

// Resets everything
void reset() {
  score_status = 0;
  last_score = 0;
  light_sensor = false;
}
