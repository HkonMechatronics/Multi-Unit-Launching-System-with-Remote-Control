void getReadings(){    // here you can update values to send, with joystick etc, dont need it 
}

void sendReadingsEspnow(){
  // Send message via ESP-NOW
  esp_err_t result_M1 = esp_now_send(broadcastAddress_M1, (uint8_t *) &sendReadings, sizeof(sendReadings));
  esp_err_t result_M2 = esp_now_send(broadcastAddress_M2, (uint8_t *) &sendReadings, sizeof(sendReadings));
   
  if (result_M1 == ESP_OK) {
    Serial.println("Sent");
  }
  else {
    Serial.println("Error sending the data");
  }
}

void updateDisplay(){
  // Display Readings in Serial Monitor
  /*
  Serial.println("INCOMING READINGS");
  Serial.print("Temperature: ");
  Serial.print(incomingReadings.haveLaunched);
  Serial.println(" ºC");
  Serial.print("Humidity: ");
  Serial.print(incomingReadings.batteryLevel);
  Serial.println(" %");
  Serial.println();
  */
}


void setup_EspNow() {
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  //WiFi.setSleep(true);   // enable modem sleep // did nothing


  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  
  // Register peer M1
  memcpy(peerInfo.peer_addr, broadcastAddress_M1, 6);
  peerInfo.channel = 1;  
  peerInfo.encrypt = false;
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 1");
    return;
  } 

  // Register peer M2
  memcpy(peerInfo.peer_addr, broadcastAddress_M2, 6);
  peerInfo.channel = 1;  
  peerInfo.encrypt = false;
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 2");
    return;
  } 

}


void handleEspNowCommunication() {
  unsigned long currentTime = millis();

  if (currentTime - lastTransmissionTime >= transmissionInterval) {
    lastTransmissionTime = currentTime;

    // Gather and send sensor data
    //getReadings();
    sendReadings.speed = send_speed;
    sendReadings.numberOfLaunchers  = send_numberOfLaunchers;
    sendReadings.pause = send_pause;
    sendReadings.angleX = send_angleX;
    sendReadings.angleY = send_angleY;
    sendReadings.launchCounter = send_launchCounter;

    sendReadingsEspnow();
    updateDisplay();
  }
}
