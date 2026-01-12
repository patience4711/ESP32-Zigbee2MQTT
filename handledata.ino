void handleDataRequests(AsyncWebServerRequest *request) 

{
      //consoleOut("handleDataRequest");
      consoleOut("handleDataRequest the request is " + String(requestUrl));
      if( request->hasArg("devnr") )
        {
        // consoleOut("found POWER");  
         int i = atoi(request->arg("devnr").c_str()) ;
         AsyncResponseStream *response = request->beginResponseStream("application/json");
         JsonDocument root;
         //we need some properties like device type, name
         root["adr"] = String(Dev_Prop[i].devAdr); // put the address
         root["name"] = String(Dev_Prop[i].devName); // put the name
         root["type"] = String(Dev_Prop[i].devType); // put the type
         root["control"] = 0;
         if(Dev_Prop[i].devType == 3) root["control"] = 3;  
         if(Dev_Prop[i].devType == 4) root["control"] = 4; 
          serializeJson(root, * response);
          request->send(response);
          return; 
        } else

      if( request->hasArg("General") )
    { 
      //consoleOut("found arg General");
      char temp[15]={0};
      uint8_t remote = 0;
      if(checkRemote( request->client()->remoteIP().toString()) ) remote = 1; // for the menu link
      
      AsyncResponseStream *response = request->beginResponseStream("application/json");
      JsonDocument root; //(160);
      //JsonObject root = doc.to<JsonObject>();
      root["cnt"] = deviceCount;    
      root["rm"] = remote;
      root["st"] = zigbeeUp;    
      serializeJson(root, * response);
      request->send(response);
      return;
    } 
  
    if( request->hasArg("Slider") )
    { 
      consoleOut("found arg Slider");
      consoleOut("devChoice = " + String(devChoice));
      char temp[15]={0};
      uint8_t remote = 0;
      if(checkRemote( request->client()->remoteIP().toString()) ) remote = 1; // for the menu link
      //int devNr = atoi(request->arg("Slider").c_str()) ;
      AsyncResponseStream *response = request->beginResponseStream("application/json");
      JsonDocument root; 
      root["devnr"] = devChoice;
      root["hue"] = lampState[devChoice].hue;  // to set the slider  
      root["sat"] = lampState[devChoice].sat; // to set the slider
      root["dim"] = lampState[devChoice].dim;  //to set the slader  
      root["state"] = lampState[devChoice].state; //to set which button
      root["onoff"] = lampState[devChoice].onoff; //to set which button
      serializeJson(root, * response);
      request->send(response);
      return;
    } 
    if( request->hasArg("Switch") )
    { 
      consoleOut("found arg Switch");
      consoleOut("devChoice = " + String(devChoice));
      char temp[15]={0};
      uint8_t remote = 0;
      if(checkRemote( request->client()->remoteIP().toString()) ) remote = 1; // for the menu link
      //int devNr = atoi(request->arg("Slider").c_str()) ;
      AsyncResponseStream *response = request->beginResponseStream("application/json");
      JsonDocument root;
      root["devnr"] = devChoice; 
      root["onoff"] = Dev_Prop[devChoice].values[0]; //1 or 0
      serializeJson(root, * response);
      request->send(response);
      return;
    } 

    // if we are here no maching request was found
       String term = "invalid request";
       request->send(200, "text/plain", term);
}    
