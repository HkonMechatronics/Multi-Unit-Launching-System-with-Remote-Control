void dcmotor_setup() {

  // Reverse pin always LOW
  pinMode(L_PWM_pin, OUTPUT);
  digitalWrite(L_PWM_pin, LOW);

  // Attach forward PWM pin
  ledcAttach(R_PWM_pin, pwmFreq, pwmResolution);

  // Initialize motor stopped
  ledcWrite(R_PWM_pin, 0);
}

void run_dcmotor() {
  // Check for new serial input
    if (send_speed >= 0 && send_speed <= 255) {
      targetPwm = send_speed +30; // 30 er slingringsrom
      //Serial.print("Target PWM set to: ");
      //Serial.println(targetPwm);
    } else {
      //Serial.println("Invalid value. Enter 0–255.");
    } 

  // Smooth ramp toward target PWM
  if (currentPwm < targetPwm) {
    currentPwm++;
    ledcWrite(R_PWM_pin, currentPwm);
    delay(stepDelay);
  } else if (currentPwm > targetPwm) {
    currentPwm--;
    ledcWrite(R_PWM_pin, currentPwm);
    delay(stepDelay);
  }
}