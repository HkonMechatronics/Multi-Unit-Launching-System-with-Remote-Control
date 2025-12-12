void setup_navigation(){
  pinMode(Navigate_Up, INPUT_PULLUP);
  pinMode(Navigate_Right, INPUT_PULLUP);
  pinMode(Navigate_Down, INPUT_PULLUP);
  pinMode(Navigate_Left, INPUT_PULLUP);
  pinMode(Navigate_Middle, INPUT_PULLUP);

}

void readJoystick() {
  if (digitalRead(Navigate_Up) == LOW) {
    Serial.println("Joystick: UP");
  }
  if (digitalRead(Navigate_Right) == LOW) {
    Serial.println("Joystick: RIGHT");
  }
  if (digitalRead(Navigate_Down) == LOW) {
    Serial.println("Joystick: DOWN");
  }
  if (digitalRead(Navigate_Left) == LOW) {
    Serial.println("Joystick: LEFT");
  }
  if (digitalRead(Navigate_Middle) == LOW) {
    Serial.println("Joystick: MIDDLE");
  }
}