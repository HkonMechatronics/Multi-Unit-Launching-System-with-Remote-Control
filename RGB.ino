
void setup_RGB() {
  //pinMode(PIN_NEOPIXEL, INPUT);   // Disconnect pin, stops leakage current, did not reduce current
  pinMode(PIN_NEOPIXEL, OUTPUT);
  digitalWrite(PIN_NEOPIXEL, LOW);


  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  // Start with everything off
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
}

void connectionStatus(){
  if (espnow_connected){
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
  } else{
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
  }
}


void change_rgb() {
    if (colorState == 0) {
     // Red on
     digitalWrite(redPin, HIGH);
     digitalWrite(greenPin, LOW);
    } else if (colorState == 1) {
    // Green on
     digitalWrite(redPin, LOW);
     digitalWrite(greenPin, HIGH);
    } else if (colorState == 2){
    // Off
     digitalWrite(redPin, LOW);
     digitalWrite(greenPin, LOW);
    }
/*
  // Yellow (red + green)
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  delay(1000);
*/

}


//////////////////////// NOT USING NEO //////////////////////////
/*
void setup_NEO(){
    rgbLed.begin();
    rgbLed.show();
}


void RBGlight()  {
    if (colorState == 0) {
     rgbLed.setPixelColor(0, 200, 0, 0); // red
    } else if (colorState == 1) {
     rgbLed.setPixelColor(0, 0, 200, 0); // green
    } else if (colorState == 2){
     rgbLed.setPixelColor(0, 0, 0, 0);
    }
    rgbLed.show();
  // Add your code here to act upon BRgb change
}
*/