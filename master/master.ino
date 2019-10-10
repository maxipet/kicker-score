// Includes
#include <Wire.h>

// Pins
#define pinLight 2

// Light
bool light_sensor = false;

// Scoring
int score[] = {0,0};
int max_score = 10;
int score_threshold = 3;
int score_pause = 1000;
unsigned long last_score = 0;

// I2C
int request_pause = 100;
unsigned long last_request = 0;

void setup() {
  pinMode(2, INPUT);
  
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  // Light Sensor update
  checkSensor();

  // Scoring
  if(light_sensor) {
    addScore(0);
  }
  
  if(millis() - last_request > request_pause) {
    Serial.println("[DEBUG] Request send!");
    sendRequest(); 
  }
}

void checkSensor() {
  if(digitalRead(pinLight) == HIGH) {
    light_sensor = true;
  } else {
    light_sensor = false;
  }
}

void sendRequest() {
  Wire.requestFrom(4, 1);
    while(Wire.available())
    Serial.println("[DEBUG] Response Received!");
    {
      int res = Wire.read();
      if(res != 0) {
        processResponse(res);
      }
    }
    last_request = millis();
}

void processResponse(int response) {
  if(response == 1) {
    addScore(1);
  } else if (response == -1) {
    subScore(1);
  }
}

void addScore(int team) {
  if(millis() - last_score > score_pause) {
    score[team] += 1;
    Serial.println(String(score[0]) + ":" + String(score[1]));
    if(score[team] >= max_score) {
      endGame(team);
    }
    last_score = millis();
  }
}

void subScore(int team) {
  if(score[team] > 0) {
    score[team] -= 1;
  }
}

void setScore(int team, int nScore) {
  score[team] = nScore;
}

void endGame(int team) {
  reset();
}

void reset() {
  resetScore();
}

void resetScore() {
  score[0] = 0;
  score[1] = 0;
}
