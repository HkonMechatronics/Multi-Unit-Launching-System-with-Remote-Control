

void registerActivity() {
  lastButtonPress = millis();
}


void checkInactivity() {
  if (displayOn && (millis() - lastButtonPress > displayTimeout)) {
    digitalWrite(TFT_BACKLITE, LOW);
    displayOn = false;
  }
}

void updateScreen(){
    if (screenNeedsUpdate) {
    drawMainScreen();
    screenNeedsUpdate = false;  // reset flag
  }
}

