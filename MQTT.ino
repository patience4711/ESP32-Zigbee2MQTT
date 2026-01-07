bool mqttConnect() {   // 
/* this function checks if we are connected to the broker, if not connect anyway */  
    if( MQTT_Client.connected() ) {
    //consoleOut("mqtt was connected");
    return true;
    }
    // we are here because w'r not connected. Signal with the LED
    consoleOut("mqtt connecting");
    ledblink(2,70);

    if (Mqtt_Port[0] == '\0' ) strcpy(Mqtt_Port, "1883");   // just in case ....
    uint8_t retry = 3;
    
    //String Clientid = getChipId(false); 

    while (!MQTT_Client.connected()) {

      if ( MQTT_Client.connect( getChipId(false).c_str(), Mqtt_Username, Mqtt_Password) )
      {
         //connected, so subscribe to inTopic (not for thingspeak)
        if(Mqtt_Format != 5 ) {
        //String clientid = getChipId(false) + "/in"; 
        //if(  MQTT_Client.subscribe ( clientid.c_str() ) ) {
        if(  MQTT_Client.subscribe ( Mqtt_inTopic ) ) { 
               consoleOut("subscribed to " + String(Mqtt_inTopic ));
               //consoleOut("subscribed to " + clientid);
           }
        }
         consoleOut(F("mqtt connected"));
         Update_Log(3, "connected");
      
       return true;

    } else {
        //String term = "connection failed state: " + String(MQTT_Client.state());
        Update_Log(3, "failed");
        if (!--retry) break; // stop when tried 3 times
        delay(500);
    }
  }
  // if we are here , no connection was made.

  consoleOut(F("mqtt connection failed"));
  return false;
}

// *************************************************************************
//                   process received mqtt
// *************************************************************************

void MQTT_Receive_Callback(char *topic, byte *payload, unsigned int length)
{
    JsonDocument doc;       // We use json library to parse the payload                         
    String jsonString = "";
    //  The function deserializeJson() parses a JSON input and puts the result in a JsonDocument.
     DeserializationError error = deserializeJson(doc, payload); // Deserialize the JSON document
    if (error)            // Test if parsing succeeds.
    {
       consoleOut("mqtt no valid json ");
        return;
    } 
    consoleOut("Deserialized JSON:");
    serializeJson(doc, jsonString);     // Print in one line
    consoleOut("incoming mqtt " + jsonString);
    // the incoming message can contain
    uint8_t dev = 0;
    uint8_t nvalue = 0;
    int idx = 0;
    /*{"Battery":255,"LastUpdate":"2026-01-07 17:53:04","RSSI":12, 
    "description":"","dtype":"Light/Switch","hwid":"5","id":"00014401","idx":945,
    "name":"ZIGBEE-SOCKET","nvalue":1,"org_hwid":"5","stype":"Switch",
    "svalue1":"0","switchType":"On/Off","unit":1}
    */
    if (!doc["idx"].isNull())
    {
      idx = doc["idx"].as<int>();
      consoleOut("there is an idx " + String(idx)) ;
      //now lookup the device
      dev = devnrLookup(idx);
      consoleOut("devNr " + String(dev)) ;
    } else {
      consoleOut("there is no idx, abort");
      return;
    }
    // so there is an idx, there should also be an nvalue
    // if (!doc["devnr"].isNull()) dev = doc["devnr"].as<uint8_t>();
    // if (!doc["onoff"].isNull()) onoff = doc["onoff"].as<uint8_t>();

    if (!doc["nvalue"].isNull()) 
    {
      nvalue = doc["nvalue"].as<uint8_t>();
    } else {
      consoleOut("no nvalue, abort");
      return;
    }
    // now we should have a device nr that can be controlled
    if(dev > -1 && dev < deviceCount)
    {
        if(Dev_Prop[dev].devType == 3) // smartbulb
        {
          consoleOut("mqtt switch " + String(dev));
          bulbOnOff(dev, nvalue, false);
          return;
        }
        if(Dev_Prop[dev].devType == 4) // smartsocket
        {
          consoleOut("mqtt switch " + String(dev));
          switchSetOnOff(dev, nvalue, false);
          return;
        }
    } 
        
    if(dev == 99) consoleOut("unknown device"); else consoleOut("nothing familiair found in mqtt");
}

int devnrLookup(int idx)
{
    uint8_t devNr;
    for(int x=0; x < deviceCount; x++)
    {
      if( Dev_Prop[x].devIdx == idx ) return x;
    }
    return 99;
}
