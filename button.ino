// Buttons already there reffered as: Button_Up, Button_Middle, Button_Down
// Navigation switch now reffered as: Navigate_Up, Navigate_Right, Navigate_Down, Navigate_Left, Navigate_Middle


void setup_button(){
  // The 3 buttons already on the esp32 reverse 
  pinMode(Button_Up, INPUT_PULLUP);
  pinMode(Button_Middle, INPUT_PULLDOWN);
  pinMode(Button_Down, INPUT_PULLDOWN);
  // Added endswitch for easier launch button
  pinMode(Endswitch_Launch, INPUT_PULLDOWN);
}


// ===== Actions for each button =====
void checkButtonUp() {
  if (!digitalRead(Button_Up) && !buttonUpPressState) {
    if (!displayOn) {
      digitalWrite(TFT_BACKLITE, HIGH); // wake
      displayOn = true;
    } else {

      // Normal functionality
      activeScreen==1; // moves to next tft screen
      annonserSeier();
        // send with espnow
         colorState = (colorState + 1) % 3;
         send_launchCounter = colorState;

      screenNeedsUpdate = true;
    }
       // Normal functionality //
    registerActivity();
    buttonUpPressState = true;
  }
}


void checkButtonMiddle() {
  if (digitalRead(Button_Middle) && !buttonMiddlePressState) {
    if (!displayOn) {
      digitalWrite(TFT_BACKLITE, HIGH); // wake
      displayOn = true;
    } else {

      // Normal functionality
      activeScreen==1; // moves to next tft screen
        // send with espnow
         colorState = (colorState + 1) % 3;
         send_angleX = colorState;
      enterDeepSleep();
      Serial.println("will never be printed");
      screenNeedsUpdate = true;
    }
       // Normal functionality //
    registerActivity();
    buttonMiddlePressState = true;
  }
}


void checkButtonDown() {
  if (digitalRead(Button_Down) && !buttonDownPressState) {
  // Check for button pressed now, but was not pressed before


    if (!displayOn) {
      digitalWrite(TFT_BACKLITE, HIGH); // wake
      displayOn = true;
    } else {

       // Normal functionality //
        // TFT updates //
       ButtonDownActiveScreen++; // moves to next tft screen
       if(ButtonDownActiveScreen == 1){
          activeScreen=1; // modes
       }else if(ButtonDownActiveScreen == 2){
          activeScreen=5; // control mode
       }else if(ButtonDownActiveScreen == 3){
        ButtonDownActiveScreen = 0;
        activeScreen=0; // back to menu
       }

        // TFT updates //
       colorState = (colorState + 1) % 3;
       change_rgb();  
        // send with espnow
        send_speed = colorState;

       screenNeedsUpdate = true;
    }
       // Normal functionality //
    registerActivity();
    // Update last state
    buttonDownPressState = true;
  }
}




void resetButtonStates(){
    if (!digitalRead(Button_Down)) {
      buttonDownPressState = false;
    }

    if (!digitalRead(Button_Middle)){
      buttonMiddlePressState = false;
    }
    if (digitalRead(Button_Up)){
      buttonUpPressState = false;
    }


}

