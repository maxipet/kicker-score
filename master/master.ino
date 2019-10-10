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
int score_pause = 3000;
unsigned long last_score = 0;

// I2C
int request_pause = 100;
unsigned long last_request = 0;

void setup() {
  pinMode(pinLight, INPUT);
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  // Light Sensor update
  checkSensor();

  // Scoring
  if(light_sensor && millis() - last_score > score_pause) {
    addScore(0);
  }

  
}

void checkSensor() {
  if(digitalRead(pinLight) == LOW) {
    light_sensor = true;
  } else {
    light_sensor = false;
  }
}

void sendRequest() {
  Wire.requestFrom(8, 1);

    while(Wire.available())
    {
      int res = Wire.read();
      if(res != NULL) {
        processResponse(res);
      }
    }
    last_request = millis();
}

void processResponse(int response) {
  if(response == 1) { // score
    addScore(1);
  } else if (response == -1) {
    subScore(1);
  }
}

void addScore(int team) {
  score[team] += 1;
  if(score[team] >= max_score) {
    endGame(team);
  }
  last_score = millis();
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
