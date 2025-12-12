void setup_NEO(){
    rgbLed.begin();
    rgbLed.show();
}

void RBGlight()  {
    if (send_speed == 0) {
     rgbLed.setPixelColor(0, 100, 0, 0); // red
    } else if (send_speed == 1) {
     rgbLed.setPixelColor(0, 0, 100, 0); // green
    } else if (send_speed == 2) {
     rgbLed.setPixelColor(0, 100, 100, 0); // 
    } else if (send_speed == 3){
     rgbLed.setPixelColor(0, 0, 0, 0);
    }
    rgbLed.show();
  // Add your code here to act upon BRgb change
}