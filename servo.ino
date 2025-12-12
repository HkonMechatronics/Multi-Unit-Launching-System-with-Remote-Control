void setup_Servos() {
  // put your setup code here, to run once:
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

  for (int i = 0; i < 4; i++) {
    servos[i].setPeriodHertz(50);                // Standard 50 Hz servo
    servos[i].attach(servoPins[i], 1000, 2000);  // Standard servo pulse range
  }
}