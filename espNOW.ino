void getReadings(){    // here you can update values to send, with joystick etc
}

void sendReadingsEspnow(){
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress_Remote, (uint8_t *) &sendReadings, sizeof(sendReadings));
   
  if (result == ESP_OK) {
    Serial.println("Sent");
  }
  else {
    Serial.println("Error sending the data");
  }
}

void updateDisplay(){
  // Display Readings in Serial Monitor
  Serial.print("speed: ");
  Serial.print(incomingReadings.speed);
  Serial.println();
  Serial.print("numblaunchers: ");
  Serial.print(incomingReadings.numberOfLaunchers);
  Serial.println();
  Serial.print("anglex: ");
  Serial.print(incomingReadings.angleX);
  Serial.println();
  Serial.print("angleY: ");
  Serial.print(incomingReadings.angleY);
  Serial.println();
  Serial.print("launchcount: ");
  Serial.print(incomingReadings.launchCounter);
  Serial.println();


}


void setup_EspNow() {
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.begin(); 
  delay(100);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress_Remote, 6);
  peerInfo.channel = 1;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  } 
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

}


void handleEspNowCommunication() {
  unsigned long currentTime = millis();

  if (currentTime - lastTransmissionTime >= transmissionInterval) {
    lastTransmissionTime = currentTime;

    // Gather and send sensor data
    getReadings();
    sendReadings.haveLaunched = incoming_haveLaunched;  // reverse logic due to written from remotes perspective, incoming means send
    sendReadings.batteryLevel  = incoming_batteryLevel;

    sendReadingsEspnow();
    updateDisplay();
  }
}
