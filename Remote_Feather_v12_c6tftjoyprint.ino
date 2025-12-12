/* Info
ESP32-s3 tft feather reverse
board: esp32, adafruit feather esp32-s3 reverse tft
       esp32 v3.3.0,  arduino esp32 boards v2.0.18-arduino5
additional urls: https://dl.espressif.com/dl/package_esp32_index.json
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
arduino ide v2.3.5


 TODO:
REMOVE LINES WITH *********** Also in other tabs
-Add serial print of ondatarecieve variables like havelaunched
-Remove getreadings() not needed
-Only functions in setup and loop
-Add two more variables similar so both launchers sends havelaunched !
-remember feathers have different mac
-Dont use string deifinitions, and dont use variables in canvas print, 
this is for neo led, which i dont use anymore so maybe look away from this on
-have libraries and information in another file? 
-Cant use analogwrite, fucks with neo. ? 
-remove button that lets you adjust backlite on and off, it is just automatic
-add try again to add peer if not worked, but i dont think necesarry, 
*/
// testing gut


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Libraries 
//#include <Arduino.h>              //auto included
#include "Adafruit_MAX1704X.h"      //v1.0.3
//#include <Adafruit_NeoPixel.h>      //v1.15.1  // dont use neopixel anymore but this is the lib
#include <Adafruit_ST7789.h>        //v1.11.0  Adafruit ST7735 and ST7789 
#include <Fonts/FreeSans12pt7b.h>
// ESP-NOW com
#include <esp_now.h>                // auto
#include <WiFi.h>                   // auto
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initializing TFT display
Adafruit_MAX17048 lipo;
Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
GFXcanvas16 canvas(240, 135);

// Initializing neo rgb led
//Adafruit_NeoPixel rgbLed(1, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800); // dont use anymore, but this is init
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Neo
// RGB: Pins for Red and Green channels
int colorState = 2;                 // Iterating through colors 
const int redPin = 8;    // a5
const int greenPin = 14; // a4

// Auto saver backlite
bool displayOn = true;                       // start with display on
unsigned long lastButtonPress = 0;           // timestamp of last activity
const unsigned long displayTimeout = 15000;  // e.g. 10 sec timeout

// TFT updating
bool screenNeedsUpdate = true; // initially draw screen
int activeScreen = 0;
int ButtonDownActiveScreen = 0;
bool buttonDownPressState = false;
bool buttonMiddlePressState = false;
bool buttonUpPressState = false;

// MODES control mode
bool lastNavMiddleState = HIGH;       // for speed control
bool speedAdjustMode = false;     // true = speed adjust mode active

// Buttons already on feather
const int Button_Up     = 0;
const int Button_Middle = 1;
const int Button_Down   = 2;

// Added endswitch for easier launch button
const int Endswitch_Launch = 17; //a1

// Navigation buttons
const int Navigate_Right  = 9;  // correct pin miten til høyre på footprint
const int Navigate_Up     = 10; // correct pin oppe til venstre på footprint
const int Navigate_Left   = 11; // correct pin midten til venstre på footprint
const int Navigate_Down   = 12; // correct pin nederst til venstre på footprint
const int Navigate_Middle = 6; // correct pin opperst til høyre på footprint

// Buzzer
const int buzzerPin       = 13;

// Lipo
float lastVoltage = 0;
unsigned long lastCheck_voltage = 0;
const unsigned long interval_voltage_check = 2000; // check every 1 second
const float threshold_voltage_jump = 0.06;        // 10 mV jump is enough

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ESP_now ///////////////  ///////////////. ///////////////. ///////////////  ///////////////
// Timing variables for ESP-NOW communication
unsigned long lastTransmissionTime = 0;    // timestamp of last transmission
const unsigned long transmissionInterval = 2000;  //  seconds in ms

// Status variables
int espnow_connected = 0;  // if connected will light up green
//String success;  // Variable to store if sending data was successful

// REPLACE WITH THE MAC Address of your receivers
uint8_t broadcastAddress_M1[] = {0x54, 0x32, 0x04, 0x08, 0x4F, 0x1C};   // blackled //c6= 54:32:04:ff:fe:08:4f:1c. 54:32:04:08:4F:1C
uint8_t broadcastAddress_M2[] = {0x68, 0x67, 0x25, 0x07, 0x80, 0x50};   // blueled

// Define variables to store readings to be sent
int send_speed = 0;
int send_numberOfLaunchers =1;
int send_pause = 2;
int send_angleX =3;
int send_angleY =4;
int send_launchCounter = 5;

// Define variables to store incoming readings
int incoming_haveLaunched;
int incoming_batteryLevel;
//int incmoing 2, samme variabler for begge espene?

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

  //screenNeedsUpdate = true;
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  espnow_connected = 1;

  incoming_haveLaunched = incomingReadings.haveLaunched;
  incoming_batteryLevel = incomingReadings.batteryLevel;
}
///// espnow end //////////  ///////////////. ///////////////. ///////////////  ///////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {                      // Runs once in the start
  Serial.begin(115200);             // Start serail *** CAN BE REMOVED in final code
  delay(500);                       // Giving serial som init time *** CAN BE REMOVED in final code

  setup_buzz();                     // From buzz ino, buzzer
  setup_TFT();                      // From TFT ino, TFT rgb display
  setup_RGB();                      // From RGB ino, RGB led
  setup_LIPO();                     // From LIPO ino, LIPO battery
  setup_button();                   // From button ino, buttons
  setup_navigation();               // From joy ino, joystick/navigation switch

  setup_EspNow();                   // From espnow ino, ESP-NOW
  delay(100);                       

  setup_Frequency();                // From sleep.ino, set MCU power mode
  setup_Sleep();                    // From sleep.ino, set sleep mode
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {                       // runs repeatedly
  handleEspNowCommunication();      // handles all espnow updating values and sending them

  checkButtonUp();                  // checking if button up is pressed
  checkButtonMiddle();              // checking if button middle is pressed
  checkButtonDown();                // checking if button down is pressed

  updateScreen();                   // see autosaver.ino, updates display based on status true or flase
  checkInactivity();                // turns off tft backlite if no buttons pressed for given time
  checkIfCharging();                // if voltage increase detected, charging is active, go into deepsleep
  resetButtonStates();              // edge detection, prevent multiple presses
  connectionStatus();               // led green if recieving data from a peer, else red
  mode();                           // runs different base on control mode, standard or personalized 

  delay(200);                       // Can use ESPSLEEP or millis instead, also prevents button and voltage readings done all the time
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SETUP EXTRA
  //setDisplay(true); // start with screen on, for autodisplay
  //drawMainScreen();
  //setup_NEO();                      // From RGB ino

//LOOP EXTRA
    //Serial.println(lipo.cellPercent());
  //Serial.println(lipo.cellVoltage());
   // checkEndswitchLaunch();
   //test_rgb();

