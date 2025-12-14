
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Libraries
#include <ESP32Servo.h>
// ESP-NOW com
#include <esp_now.h>
#include <WiFi.h>
// neo
#include <Adafruit_NeoPixel.h>      //v1.15.1
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DC motor ///////////////  ///////////////. ///////////////. ///////////////  ///////////////
// RS550 BTS7960 one-direction test using Arduino-ESP32 ledcAttach / ledcWrite
const int R_PWM_pin = 18;   // Forward PWM (active)
const int L_PWM_pin = 19;   // Reverse PWM (always LOW)

int currentPwm = 0;
int targetPwm = 0;
int stepDelay = 50;  // ms between steps for smooth acceleration

// PWM settings
const uint32_t pwmFreq = 20000;  // 20 kHz
const uint8_t pwmResolution = 8; // 0–255
////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ESP_now ///////////////  ///////////////. ///////////////. ///////////////  ///////////////
// Timing variables for ESP-NOW communication
unsigned long lastTransmissionTime = 0;    // timestamp of last transmission
const unsigned long transmissionInterval = 2000;  // 10 seconds in ms

// Status variables
int espnow_connected = 0;  // if connected will light up green
//String success;  // Variable to store if sending data was successful

// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress_Remote[] = {};  

// Define variables to store readings to be sent
int send_speed = 0;
int send_numberOfLaunchers =1;
int send_pause = 2;
int send_angleX =3;
int send_angleY =4;
int send_launchCounter = 5;
/*
int temperature = 2;
int humidity    = 2;
int pressure    = 2;  */

// Define variables to store incoming readings
/*
int incomingTemp = 2;
int incomingHum  = 2;
int incomingPres = 2;  */
int incoming_haveLaunched;
int incoming_batteryLevel;

//Structure example to send data, Must match the receiver structure
typedef struct struct_message {
    int speed             ;
    int numberOfLaunchers ;
    int pause             ;
    int angleX            ;
    int angleY            ;
    int launchCounter     ;
    int haveLaunched        ;
    int batteryLevel        ;
} struct_message;

// Create a struct_message to hold readings you can send
struct_message sendReadings;
// Create a struct_message to hold incoming data to recieve
struct_message incomingReadings;

esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    espnow_connected = 1;
  }
  else{
    espnow_connected = 0;
  } 
  //screenNeedsUpdate = true;
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  send_speed = incomingReadings.speed;
  send_numberOfLaunchers = incomingReadings.numberOfLaunchers;
  send_pause = incomingReadings.pause;
  send_angleX = incomingReadings.angleX;
  send_angleY = incomingReadings.angleY;
  send_launchCounter = incomingReadings.launchCounter;
}
///// espnow end //////////  ///////////////. ///////////////. ///////////////  ///////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initializing neo rgb led
Adafruit_NeoPixel rgbLed(1, 8, NEO_GRB + NEO_KHZ800);
// Neo
int colorState = 2;                 // Iterating through colors on neo
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Servomotors
// Published values for SG90 servos; adjust if needed
int minUs = 1000;
int maxUs = 2000;
int pos = 0;      // position in degrees
const int servoPins[4] = {4, 5, 6, 7};  // Fixed pin assignments
Servo servos[4];                        // Fixed number of servos
ESP32PWM pwm;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {

	Serial.begin(115200);

  // put your setup code here, to run once:
  setup_Servos();
  dcmotor_setup();
  setup_NEO();
  setup_EspNow();                   // From espnow ino

}

void loop() {
  // put your main code here, to run repeatedly:
  /// espnow get data
  handleEspNowCommunication();
  run_dcmotor();
 // printControlStatus();
/*
for (int i = 1; i < 4; i++) {
  for (int pos = 0; pos <= 180; pos++) {
    servos[i].write(pos);
    delay(1);
  }
  for (int pos = 180; pos >= 0; pos--) {
    servos[i].write(pos);
    delay(1);
  }

  servos[0].write(45);
}
*/
  RBGlight();
  //delay(180);    // for button readings and not reading voltage all the time
  delay(200); 
}
