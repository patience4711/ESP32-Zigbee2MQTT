//#include <AsyncJson.h>
#include <ArduinoJson.h>

#include <ESPAsyncWebSrv.h>
#include <AsyncTCP.h>
#include <AsyncEventSource.h>

#include <WiFi.h>
#include <esp_wifi.h> 
#include <DNSServer.h> 

#include "OTA.h"
#include <Update.h>
//#include <Hash.h>
#include "PSACrypto.h"
#define VERSION  "ESP32-ZIGBEE_v1_1"

#include <TimeLib.h>
#include <time.h>
#include <sunMoon.h>

#include "soc/soc.h" // ha for brownout
#include "soc/rtc_cntl_reg.h" // ha for brownout
#include <esp_task_wdt.h> // ha
#include <rtc_wdt.h>
           
#include "SPIFFS.h"
#include "FS.h"

#include <ArduinoJson.h>
//#include "AsyncJson.h"
#include <Arduino.h>

#include <Preferences.h>//#include <AsyncTCP.h>
Preferences prefs;
//#include "Async_TCP.h" //we include the customized one

//#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);
AsyncEventSource events("/events"); 
AsyncWebSocket ws("/ws");

#include <PubSubClient.h>

#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
const byte DNS_PORT = 53;
DNSServer dnsServer;
#include "HTML.h"
//#include "AAA_CONFIG_INVERTERS.H"
#include "AAA_MENUPAGE.h"
#include "PORTAL_HTML.h"
//#include "DEVICES.ino"
#include "eventSource.h"
/*
 * if we define TEST we have a special environment to test the decoding of a string
 * for the different invertertypes. We only test inv0. If we set this up as a DS3
 * a ds3 string will be analyzed.
 * - no automatic polling
 * - no automatic healthcheck
 * - no start of zigbee at boot\
 * - in the console an option to test inv 0 '10;TESTINV'
 * - more debug information in the console
 */
bool uartBusy = true;
uint8_t normalOps = 0;
typedef struct{
  char devName[20]   = "N/A";
  char devIeee[20]   = "0000000000000000";
  char devAdr[10]     = "0000";
  int  devType       = 0;
  int  devIdx        = 0;
  float values[4]    = {0.0, 0.0, 0.0, 0.0};
  int  toMqtt        = 0;
} devices; 
devices Dev_Prop[9]; 


uint8_t deviceCount = 0;

//bool readZBFF(char *inMess);
//bool waitForPairAndQuery(char* s_d, uint16_t &shortAddr, uint8_t ieeeAddr[8]);
//#define TEST // prevents healthcheck and polling
String debugLog = ""; // the debug info see consoleOut

  uint8_t procesId = 1; //determines the please wait time
  //bool USB_serial = true;
  char ssid[33] = ""; // was 33 
  char pass[64] = ""; // was 40
  bool tryConnectFlag = false;
// variables concerning time and timers
  WiFiUDP Udp; 
  //time_t daybeginTime = 0;
  time_t switchoffTime = 0;
  time_t switchonTime = 0;
  bool dayTime=true;

//  byte mDay = 0;
//  String  maan = "";
  uint8_t actionFlag = 0;
  uint8_t midnightFlag = 0;
 // variables wificonfig
  char pswd[11] = "";
  char userPwd[11] = "";  
  float longi = 0;
  float lati = 0;
  char gmtOffset[5] = "";  //+5.30 GMT
  bool zomerTijd = true;
  //char static_ip[16] = "";
  uint8_t securityLevel = 6;


// ***************************************************************************
//                             variables ECU
// ***************************************************************************
// ZIGBEE_globals
#define CC2530_MAX_SERIAL_BUFFER_SIZE 512
#define MAX_FRAME_SIZE 128
//#define YC600_MAX_NUMBER_OF_INVERTERS 9  // 0 1 2 3 4 5 6 7 8
//char inMessage[CC2530_MAX_SERIAL_BUFFER_SIZE] = {0};
int readCounter = 0;
//char messageHead[5];
int diagNose = 0; // initial true but after a successful healthcheck false
//bool Polling = false; // when true we have automatic polling
int errorCode=10;
//int recovered = 0;
  char txBuffer[90];

  //int t_saved[YC600_MAX_NUMBER_OF_INVERTERS] = {0};
  //float en_saved[YC600_MAX_NUMBER_OF_INVERTERS][4] = {0};
  
  char InputBuffer_Serial[50]; // need to be global


 uint8_t zigbeeUp = 11; // initial allways initializing, this changes to 1 = up or 0 not up after initial healthcheck
 int pollOffset = 0;
 //int inverterKeuze=0;
 //int inverterCount=0;
 //char COORDINATOR_ID[13] = "";

char requestUrl[15] = {"/"}; // to remember from which webpage we came  

// variables mqtt ********************************************
  char  Mqtt_Broker[30]=    {""};
  char  Mqtt_outTopic[26] = {""}; // was 26
  char  Mqtt_Username[26] = {""};
  char  Mqtt_Password[26] = {""};
  char  Mqtt_Port[5] =  {""};
  int   Mqtt_Format = 0; 

  int   event = 0;
  long  mqtt_lastConnect = 0;

  int dst;
  //int iKeuze;
  uint8_t devChoice;
  uint8_t dev2Join;
  //int lampState[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0 }; 
  //int hue[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0 };
  //int sat[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0 };
  //int dim[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0 };
  bool timeRetrieved = false;
  int networksFound = 0; // used in the portal
  int datum = 0; //

  typedef struct{
  bool  onoff = false;
  uint8_t state  = 0;
  uint16_t hue    = 0;
  uint8_t sat    = 0;
  uint8_t dim    = 0;
} lamps; 
lamps lampState[9]; 
  
  
  
  unsigned long previousMillis = 0;        // will store last temp was read
  static unsigned long laatsteMeting = 0; //wordt ook bij OTA gebruikt en bij wifiportal
  static unsigned long lastCheck = 0; //wordt ook bij OTA gebruikt en bij wifiportal

#define LED_AAN    HIGH   //sinc
#define LED_UIT    LOW
#define knop              0  //
#define led_onb           2  // onboard led was 2
#define ZB_RESET          5 // D5
//#define ZB_TX             17 // D8

// we use this string only to send webpages
String toSend = "";
 
int value = 0; 

//int aantal = 0;
int resetCounter=0;
bool apFlag=false;
// *******************************  log *************************************
//// variables To record and display last events on the home page.
//struct logEvent {
//  String    Log_date ;
//  String    Log_kind ;
//  String    Log_message;
//};
// *******************************  log *************************************
// variables To record and display last events on the home page.
#define Log_MaxEvents 18 
 
 typedef struct {
  char date[14] ;
  int  kind ; // zigbee, system, mqtt, pairing
  char  message[13] ;
} logEvent;
logEvent Log_Events[Log_MaxEvents];
bool Log_MaxReached = false;
byte logNr = 0;

WiFiClient espClient ;
PubSubClient MQTT_Client ( espClient ) ;
int Mqtt_stateIDX = 123;
//bool getallTrigger = false;
//bool reloadTrigger = false;

#include <Ticker.h>
Ticker resetTicker;
//AsyncWebServerRequest* pendingRequest = nullptr;
char PAN_ID[5];

int interviewFlag = 20;
int mqttFlag = 0;
unsigned long triggerTime = 0;  // when shouldDo was set true
//const unsigned long delayDuration = 60000UL; // 60 seconds in ms
//bool waitForJoin(); // predeclare

// *****************************************************************************
// *                              SETUP
// *****************************************************************************
#define RXD2 16
#define TXD2 17
void setup() {
  Serial.begin(115200);

  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  Serial.println(F("Serial Txd is on pin: 17"));
  Serial.println(F("Serial Rxd is on pin: 16"));

  
  pinMode(knop, INPUT_PULLUP); // de knop
  pinMode(led_onb, OUTPUT); // onboard led
 // pinMode(ZB_TX, OUTPUT);
 // digitalWrite(ZB_TX, LOW); //
  pinMode(ZB_RESET, OUTPUT);// resetpin cc2530   
  digitalWrite(ZB_RESET, HIGH);
  pinMode(4, OUTPUT); // 
  digitalWrite(4, LOW); //
  
   ledblink(1, 800);

   attachInterrupt(digitalPinToInterrupt(knop), isr, FALLING);

 
  SPIFFS_read();
 // we set deviceCount to the number of inverterfiles we find
  deviceCount = readDevicefiles();
  Serial.println("\ndeviceCount = " + String(deviceCount)); 
  Serial.println("\ndiagNose = " + String(diagNose));
  if (diagNose != 0) printDevices(); // show all the device files
 
  prefs.begin("my_data", false); //open preferences for r/w
 
  String key = "req";
  prefs.getString(key.c_str(), requestUrl, sizeof(requestUrl) );
  Serial.println("requestUrl = " + String(requestUrl));
  prefs.end();// takes care for the return to the last webpage after reboot
  getLamps();
  start_wifi(); // start wifi and server
 
  getTijd(); // retrieve time from the timeserver
  Update_Log(1, "boot up");

  // ****************** mqtt init *********************
  MQTT_Client.setKeepAlive(150);
  MQTT_Client.setServer(Mqtt_Broker, atoi(Mqtt_Port));
 // MQTT_Client.setBufferSize(300); //to avoid freeze ups
  MQTT_Client.setCallback ( MQTT_Receive_Callback ) ;

  if ( Mqtt_Format != 0 ) 
  {
       Serial.println(F("setup: mqtt configure"));
       mqttConnect(); // mqtt connect
  } 
  else 
  {
       Update_Log(3, "not enabled"); 
  }

  initWebSocket();
    
  Serial.println(F("booted up, checking coordinator"));
  //Serial.println(WiFi.localIP());

  delay(1000);
  ledblink(3,500);

  
  
  Update_Log(1,"healthcheck");
  healthCheck(); // check the state of the zigbee system
  if(zigbeeUp == 1) {
        Update_Log(2,"running");
     }

  resetCounter = 0;
  //events.send( "reload", "message"); //getGeneral and getAll triggered
  eventSend(0);




} // end setup()

//****************************************************************************
//*                         LOOP
//*****************************************************************************
void loop() {



// ***************************************************************************
//                       day or night mode
// ***************************************************************************
#ifdef TEST
// always daytime to be able to test
 dayTime = true;
#endif
 
   if(now() > switchonTime && now() < switchoffTime) 
    {
          if(!dayTime)  
          {
             dayTime = true;
             Update_Log(1, "woke up");
             consoleOut("woke-up");
             // reset the dayly energy at wakeup and send mqtt message
             //resetValues(true, true);
             //events.send( "reload", "message"); // refresh the data and state
             eventSend(1);
            }
    } else {
         if(dayTime) 
         {
            dayTime = false;
            //String term= "nightmode";
            Update_Log(1, "nightmode");
            consoleOut("nightmode");
            // clean memory
            //memset( &inMessage, 0, sizeof(inMessage) ); //zero out the 
            //delayMicroseconds(250);
            // we send null messages for each inverter
            //resetValues(false, true); // make all values zero exept energy and send mqtt
            //events.send( "reload", "message"); // refresh the data and state
            eventSend(0);
            midnightFlag = 250; // triggers the reset values and mqtt null message at midnight
            
         }
    }

// ******************************************************************
//              polling every 300 seconds
// ******************************************************************
#ifndef TEST
//   unsigned long nu = millis();  // the time the program is running

//    if (nu - laatsteMeting >= 1000UL * 300) // 300 sec
//    {
//      consoleOut("300 secs passed, polling" + String(millis()) ); //
//         laatsteMeting += 1000UL * 300 ; // increases each time with (300/inverterCount * miliseconds);
//         // if(dayTime && Polling) // we only poll at day and when Polling = true 
//         //    { 
//         //       ledblink(1,100);
//         //       poll_all(); //if inverterCount = 9 than we have inverters 0-8
//         //    } 
//  }

// ******************************************************************
//              healthcheck every 10 minutes
// ******************************************************************

   unsigned long nu = millis(); //  // 
   if (nu - lastCheck >= 1000UL * 6000) // =10min
   {
   Serial.println("6000 secs passed, healthcheck" + String(millis()) );
         lastCheck += 1000UL * 6000;
         //we dont do healtcheck when stopped
         healthCheck(); // checks zb radio, mqtt and time, when false only message if error
   }

  // we recalcultate the switchtimes for this day when there is a new date
  // if retrieve fails, day will not be datum, so we keep trying by healthcheck
  if (day() != datum && hour() > 2) // if date overflew and later then 2
  { 
          getTijd(); // retrieve time and recalculate the switch times
          //delay(500);
          //ESP.restart();
          //DebugPrintln("date overflew, retrieve time");
  }
 #endif

  // Check if 1 minute has passed since shouldDo was set true
  if (mqttFlag != 0 && millis() - triggerTime >= 1000UL * 60) {
      char toMQTT[100]={0};

      snprintf(toMQTT, sizeof(toMQTT), "{\"idx\":%d,\"nvalue\":0,\"svalue\":\"%.1d\"}" , mqttFlag , 0); 
      // mqttConnect() checks first if we are connected, if not we connect anyway
      //consoleOut("toMQTT = " + String(toMQTT));
      if(mqttConnect() ) MQTT_Client.publish ( Mqtt_outTopic, toMQTT );
      mqttFlag = 0;  // reset the flag (optional)
  }



// ***************************************************************************
//                       m q t t
// ***************************************************************************
       // before each transmission the connection is tested
       // so we don't do this in the loop
       if(Mqtt_Format != 0 ) MQTT_Client.loop(); //looks for incoming messages
    
  // //*********************************************************************
  // //             send null data at midnight 
  // // ********************************************************************
  // if(hour() == 0 && timeRetrieved && midnightFlag == 250)
  // {
  //     if(second() > 0 ) 
  //     {
  //       // resetValues(true, true); //set all values to zero and sent mqtt
  //       // Update_Log(1, "values wipe");
  //       // midnightFlag = 0; // to prevent repetition
  //       // //events.send( "getall", "message"); // refresh the data
  //       // eventSend(2);
  //      }
  // }
  
  test_actionFlag();
  
  if( Serial2.available() && normalOps == 0) {
    diagNose = 1; //show on console
    ledblink(2, 100);
    empty_serial2(); // discard the messages
    }
  if( Serial2.available() && normalOps == 1) {
    diagNose = 1; //show on console
    ledblink(2, 100);
    processNormal(); // read and decode
  }
   ws.cleanupClients();
   yield(); // to avoid wdt resets

  // SERIAL: *************** kijk of er data klaar staat op de seriele poort **********************
  if(Serial.available()) {
       handle_Serial();
   }


}
//****************  End Loop   *****************************





