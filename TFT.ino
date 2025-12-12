void setup_TFT(){
  display.init(135, 240);           // Init ST7789 240x135
  display.setRotation(3);
  canvas.setFont(&FreeSans12pt7b);
  canvas.setTextColor(ST77XX_WHITE); 
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);
  canvas.fillScreen(ST77XX_BLACK);
  testShuttlecock();
  delay(1500);
  //lastButtonPress = millis();
}
///////////////////
void Display_Showing_Always(){
  canvas.fillScreen(ST77XX_BLACK);
  // ----- Battery info top right -----
  canvas.setTextColor(ST77XX_GREEN); 
 // canvas.setTextSize(1);
  canvas.setCursor(240 - 54, 19); // 40 = enough space for "100%"
  canvas.print(lipo.cellPercent(), 0);
  canvas.println("%");

  // ----- ESP-NOW connection status middle right -----
 // drawConnectionStatus(240 - 63, 75);

  // shuttlecock symbol
  drawShuttlecockSimple();
  // ----- Actions vertically -----
  canvas.setCursor(0, 19);            // first button
}
//////////////////
//////////////////////////////////////////////////////////////////////
// Draw the main screen
void drawMainScreen() {
  if (activeScreen==0){
  Display_Showing_Always();
  canvas.setTextColor(ST77XX_YELLOW);
  canvas.println(" Single");
  canvas.println("");
  canvas.setTextColor(ST77XX_BLUE);
  canvas.println(" 2 ~comm~");
  canvas.println("");
  canvas.setTextColor(ST77XX_RED);
  canvas.setCursor(0, 127);   
  canvas.println(" Multiple");

  // Push canvas to display
  display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
  }
  ////////////////////////
  if (activeScreen==1){
    //alwaysshowing minus battery
  canvas.fillScreen(ST77XX_BLACK);
  drawShuttlecockSimple();
  // ----- Actions vertically -----
  canvas.setCursor(0, 19);            // first button
  // end //

  canvas.setTextColor(ST77XX_YELLOW);
  canvas.println("Standard Programs");
  canvas.println("");
  canvas.setTextColor(ST77XX_BLUE);
  canvas.println("Personal Programs ⚙️");
  canvas.println("");
  canvas.setTextColor(ST77XX_RED);
  canvas.setCursor(0, 127);   
  canvas.println("Control Mode 🕹️");
  // Push canvas to display
  display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
  }
  if (activeScreen==5) { // control mode
  Display_Showing_Always();
  canvas.setTextColor(ST77XX_YELLOW);
  canvas.print("Speed: ");
  canvas.println(send_speed);
  canvas.println("");
  canvas.setTextColor(ST77XX_BLUE);
  canvas.print("AngleX: ");
  canvas.print(send_angleX);
  canvas.print(", AngleY: ");
  canvas.println(send_angleY);
  canvas.println("");
  canvas.setTextColor(ST77XX_RED);
  canvas.setCursor(0, 127);   
  canvas.println("Exit");  // make activescreen = 0 ?
  // Push canvas to display
  display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
  }

}
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
void testShuttlecock() {
  canvas.fillScreen(ST77XX_BLACK);

  // Bottom cork (circle)
  int cx = canvas.width() / 2;
  int cy = canvas.height() - 20;
  int headRadius = 14;

  canvas.fillCircle(cx, cy, headRadius, ST77XX_WHITE);
  canvas.drawCircle(cx, cy, headRadius, ST77XX_MAGENTA);
  display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
  delay(200);

  // Feather settings
  int featherLength = 80;
  int featherCount = 10;
  int featherBaseWidth = 3;
  int featherTipWidth = 14;

  // Grow step by step
  for (int step = 10; step <= featherLength; step += 6) {
    for (int i = 0; i < featherCount; i++) {
      // Angle spread
      float angle = -PI/2 + (i - (featherCount-1)/2.0) * (PI/featherCount);

      // Base of feather
      int x1 = cx + (headRadius + 2) * cos(angle);
      int y1 = cy + (headRadius + 2) * sin(angle);

      // Feather tip for this step
      int x2 = cx + (headRadius + step) * cos(angle);
      int y2 = cy + (headRadius + step) * sin(angle);

      // Direction vector
      float dx = x2 - x1;
      float dy = y2 - y1;
      float len = sqrt(dx*dx + dy*dy);
      dx /= len; dy /= len;

      // Width grows with step
      int currWidth = map(step, 0, featherLength, featherBaseWidth, featherTipWidth);

      // Perpendicular offsets
      int bx1 = x1 - dy * featherBaseWidth;
      int by1 = y1 + dx * featherBaseWidth;
      int bx2 = x1 + dy * featherBaseWidth;
      int by2 = y1 - dx * featherBaseWidth;

      int tx1 = x2 - dy * currWidth;
      int ty1 = y2 + dx * currWidth;
      int tx2 = x2 + dy * currWidth;
      int ty2 = y2 - dx * currWidth;

      // Draw feather segment
      uint16_t color = ST77XX_GREEN + step * 30; // cycle color like triangles
      canvas.fillTriangle(bx1, by1, bx2, by2, tx1, ty1, color);
      canvas.fillTriangle(bx2, by2, tx1, ty1, tx2, ty2, color);
    }

    // Update after each growth step
    display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
    delay(100);
  }
}

//////////////////////////////////////////////////////////////////////
/*
void Start_Screen(){
  canvas.fillScreen(ST77XX_BLACK);
  canvas.setCursor(0, 17);
  canvas.setTextColor(ST77XX_RED);
  canvas.println("HKON HKON");
  canvas.setTextColor(ST77XX_YELLOW);
  canvas.println("ESP32-S3 TFT");
  canvas.setTextColor(ST77XX_GREEN); 
  canvas.print("Battery: ");
  canvas.setTextColor(ST77XX_WHITE);
  canvas.print(lipo.cellVoltage(), 1);
  canvas.print(" V  /  ");
  canvas.print(lipo.cellPercent(), 0);
  canvas.println("%");
  canvas.setTextColor(ST77XX_BLUE); 
  canvas.print("Shuttlecock ");
  canvas.setTextColor(ST77XX_WHITE);

  canvas.println("");
  canvas.print("Buttons: ");
}
*/
////////////////////////////////////////////////////////////////////////////////////
void drawShuttlecockSimple() {
    const int size = 48;

    // Fixed position
    const int x = 187;
    const int y = 84;

    // Base (white circle)
    const int baseRadius = size / 6;
    const int cx = x + size / 2;
    const int cy = y + size - baseRadius;
    canvas.fillCircle(cx, cy, baseRadius, ST77XX_WHITE);

    // Feathers (triangle)
    const int featherHeight = size / 2;
    const int apexX = cx;
    const int apexY = cy - baseRadius;          
    const int baseY = apexY - featherHeight;    
    const int halfBase = baseRadius * 3;        

    canvas.fillTriangle(
        apexX, apexY,                 
        cx - halfBase, baseY,         
        cx + halfBase, baseY,         
        ST77XX_ORANGE
    );

    // Feather lines (simulate veins)
    int featherLines = 5; // number of lines
    for (int i = 1; i < featherLines; i++) {
        float t = (float)i / featherLines;
        int startX = apexX;
        int startY = apexY;
        int endX = cx - halfBase + t * (2 * halfBase); // interpolate along base
        int endY = baseY;
        canvas.drawLine(startX, startY, endX, endY, ST77XX_WHITE);
    }
}

void drawConnectionStatus(int x, int y) {
  if (espnow_connected == 1) {
    canvas.setTextColor(ST77XX_GREEN);
    canvas.setCursor(x, y);
    canvas.print("Succ");
  } else {
    canvas.setTextColor(ST77XX_RED);
    canvas.setCursor(x, y);
    canvas.print("Fail");
  }
}

void chargingDetectedDisplay(){
      canvas.fillScreen(ST77XX_BLACK);
      drawShuttlecockSimple();
      // ----- Battery info top right -----
      canvas.setTextColor(ST77XX_GREEN); 
      // canvas.setTextSize(1);
      canvas.setCursor(0, 17);            // first button
      canvas.println("Charging");
      canvas.println("");
      canvas.setTextColor(ST77XX_RED); 
      canvas.println("Press reset button");
      canvas.println("to deactivate");
      canvas.println("deepsleep");
      display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
      delay(50);
}
