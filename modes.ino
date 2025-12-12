void mode(){
    if (ButtonDownActiveScreen==2){
        ControlMode();
    }
}


void ControlMode() {
    // Read buttons
    bool up     = digitalRead(Navigate_Up) == LOW;
    bool down   = digitalRead(Navigate_Down) == LOW;
    bool left   = digitalRead(Navigate_Left) == LOW;
    bool right  = digitalRead(Navigate_Right) == LOW;
    bool middle = digitalRead(Navigate_Middle) == LOW;
    bool launchPressed = digitalRead(Endswitch_Launch) == LOW;

    // Toggle speed adjust mode on middle press
    if (middle && lastNavMiddleState == HIGH) {
        speedAdjustMode = !speedAdjustMode;
    }
    lastNavMiddleState = middle;

    // Adjust speed if speed adjust mode is active
    if (speedAdjustMode) {
        if (up) send_speed++;
        if (down) {
            send_speed--;
            if (send_speed < 0) send_speed = 0;
        }
    }

    // Cumulative angles from directional buttons
    if (left)  send_angleX--;
    if (right) send_angleX++;
    if (up && !speedAdjustMode)    send_angleY++;  // only move angle if not adjusting speed
    if (down && !speedAdjustMode)  send_angleY--;

    // Launch counter increments on end switch press (edge detection)
    if (launchPressed) {
        send_launchCounter++;
        Serial.println("Launch pressed, counter incremented");
    }
    //lastEndswitchState = launchPressed;
    printControlStatus(); // be removed

    screenNeedsUpdate = true;  // to display joystick values etc
    
    if (left || right || up || down) { // Register activity if any button was pressed
        registerActivity();
    }
}

void printControlStatus() {
    Serial.print("AngleX: ");
    Serial.print(send_angleX);
    Serial.print("\tAngleY: ");
    Serial.print(send_angleY);
    Serial.print("\tSpeed: ");
    Serial.print(send_speed);
    Serial.print("\tLaunchCounter: ");
    Serial.println(send_launchCounter);
    Serial.print(" ");
    Serial.println(speedAdjustMode);
}

