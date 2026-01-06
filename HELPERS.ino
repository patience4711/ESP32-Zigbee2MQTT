int readDevicefiles() {
  // we must try to read from 0 to 8 because in some situations there can be a gap
  int count = 0;
  for (int x=0; x < 9; x++) 
  {
    String bestand = "/Dev_Prop" + String(x) + ".str";
  //consoleOut("reading file " + bestand);
      if (!readStruct(bestand)) 
      { 
        // if the file not exists we break
        consoleOut("no file " + bestand); 
        //break;
      } else { count += 1; }
  }
  return count;
}

// flash the led
void ledblink(int i, int wacht) {
  for(int x=0; x<i; x++) {
    digitalWrite(led_onb, LED_AAN);
    delay(wacht);
    digitalWrite(led_onb, LED_UIT);
    delay(wacht);
   }
}


void empty_serial() { // remove any remaining data in serial buffer
  while(Serial.available()) {
    Serial.read();
  }
}
void empty_serial2() { // remove any remaining data in serial buffer
  while(Serial2.available()) {
    Serial2.read();
  }
}

void delayedReset() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  SPIFFS.end();       // Recommended before formatting
  SPIFFS.format();
  ESP.restart();
}

void flashErase(AsyncWebServerRequest *request) {
// call this from the portal to erase the wifi and the flash
     procesId = 2;
     confirm();
     request->send(200, "text/html", toSend);
     resetTicker.once(3.0, delayedReset); // 1-second delay
}

void eventSend(byte what) {
  if (what == 1) {
      events.send( "general", "message"); //getGeneral triggered            
  } else {
     events.send( "getdata", "message"); // both triggered
  }
}

// int readInverterfiles() {
//   // we must try to read from 0 to 8 because in some situations there can be a gap
//   int count =0;
//   for (int x=0; x < 9; x++) 
//   {
//   String bestand = "/Inv_Prop" + String(x) + ".str";  
//   //consoleOut("reading file " + bestand);
//       if (!readStruct(bestand)) 
//       { 
//         // if the file not exists we break
//         consoleOut("no file " + bestand); 
//         //break;
//       } else { count += 1; }
//   }
//   return count;
// }

    

String getChipId(bool sec) {
    uint32_t chipId = 0;
    for(int i=0; i<17; i=i+8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  if(sec) return String(chipId); else return "ESP32-ZIGBEE-" + String(chipId);
}

void showDir() {
    char temp[50]={0};
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file) {
      sprintf(temp, "%s  size %u<br>" , file.name(), file.size() );
      consoleOut(String(temp));
      delay(100);
      file = root.openNextFile();
    }
  }

  // function to 
void consoleOut(String toLog) {

 switch (diagNose) 
    { case 0:  
          break;
      case 1:       
          delay(100); // otherwise the socket cannot keep up
          ws.textAll(toLog);
          break;
      case 2: 
          Serial.println(toLog);
          break;
      case 3:
           toLog += "\n";
          debugLog += toLog;   
    }
}

//  ************** preferences ****************

void saveLamp(uint8_t index, lamps &lamp) 
{
  if (index >= 9) return;

  prefs.begin("lampdata", false);

  char key[8];
  snprintf(key, sizeof(key), "lamp%u", index);

  prefs.putBytes(key, &lamp, sizeof(lamps));
  prefs.end();
}

void getLamps()
{
  for (uint8_t i = 0; i < 9; i++) {
    loadLamp(i, lampState[i]);
  }
}

void loadLamp(uint8_t index, lamps &lamp) 
{
  if (index >= 9) return;
  prefs.begin("lampdata", true);

  char key[8];
  snprintf(key, sizeof(key), "lamp%u", index);

  if (prefs.isKey(key)) {
    prefs.getBytes(key, &lamp, sizeof(lamps));
  } else {
    // Defaults if never saved
    lamp = { false, 0, 0, 0, 0 };
  }

  prefs.end();
}