void setup_Frequency(){
   // Reduce CPU Frequency
  setCpuFrequencyMhz(80); // 80, 160, 240 for wifi apps
  delay(20);
  
  // consider sleeping upon not in use and whilie charging

}
  
void setup_Sleep(){

}

void enterDeepSleep(){     // wake up by restart or turning on and off. wake up soruce gpio??
  
  esp_deep_sleep_start();
}