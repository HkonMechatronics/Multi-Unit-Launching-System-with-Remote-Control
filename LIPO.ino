

void setup_LIPO(){        // *********************** can remove hole function exept pinmodes
  if (!lipo.begin()) {
    Serial.println(F("Couldnt find Adafruit MAX17048?\nMake sure a battery is plugged in!"));
    while (1) delay(10);
  }
  Serial.print(F("Found MAX17048"));
  Serial.print(F(" with Chip ID: 0x")); 
  Serial.println(lipo.getChipID(), HEX);
  delay(20);
  lastVoltage = lipo.cellVoltage(); // initial reading
}

void checkIfCharging(){
  unsigned long now_for_voltage = millis();
  if (now_for_voltage - lastCheck_voltage >= interval_voltage_check) {
    lastCheck_voltage = now_for_voltage;

    float voltage = lipo.cellVoltage();
   // Serial.println(voltage);

    if (voltage > lastVoltage + threshold_voltage_jump) {
      Serial.println("Voltage jumped → assuming charging, going to deep sleep");
      digitalWrite(TFT_BACKLITE, LOW);
      chargingDetectedDisplay();
      digitalWrite(TFT_BACKLITE, HIGH);

      delay(4000);  // flush serial
      enterDeepSleep();
    }

    lastVoltage = voltage;
  }

}



// Draw battery symbol and percentage at top-right






