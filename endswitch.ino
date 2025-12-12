void checkEndswitchLaunch() {
  if (!digitalRead(Endswitch_Launch)) {

    // Normal functionality //
    //send_launchCounter ++;       // sending to launcher initiating a launch, dont matter if incremented twice cause checking for bigger than last time
    //colorState = (colorState + 1) % 3;
    //send_launchCounter = colorState;
    Serial.println("endswtch pressed");

    // Normal functionality //
  }
}