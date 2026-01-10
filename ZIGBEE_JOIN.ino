
void joinOnActionflag() 
{
   //start with setup the coordinator
   //can we pair when the radio is up for normal operation
   //we want the joining process logged in a file so diagNose = 3
   char term[20];
   debugLog="";
   diagNose = 1;
   consoleOut("trying join inv " + String(devChoice));
   bool shouldSave = false;
   if( zigbeeUp != 1) {
      //term="pairing failed, zb down";
      //Update_Log(4, "failed");
      consoleOut("joining failed, zb down");
      return;
    }
   // now that we know that the radio is up, we don't need to test this in the pairing routine
   enableJoin();
   // now the serial2 buffr should be empty
   normalOps = 5;
  
   
   if( waitJoin() ) 
    {
       //DebugPrintln("joining success, saving configfile");
       String term = "success, device got Address " + String(Dev_Prop[devChoice].devAdr);
       //Update_Log(2, "success");
       consoleOut(term);
       shouldSave = true;   

    } else {
       // strncpy(Dev_Prop[devChoice].devAdr, "0000", 6);
       //String term = "joining failed");
       // Update_Log(4, "failed");
       consoleOut("joining failed");
    }
    if(shouldSave)
    {
        String bestand = "/Dev_Prop" + String(dev2Join) + ".str"; 
        consoleOut("saving the file " + bestand);
        writeStruct(bestand, dev2Join); // alles opslaan in SPIFFS   
    }
    disableJoin();
    normalOps = 1;
}

void handleJoin(AsyncWebServerRequest *request) {

     strcpy(Dev_Prop[dev2Join].devAdr, "1111"); // this value makes the pairing page visable
     
     actionFlag = 60; // we do this because no delay is alowed within an async request
     toSend=FPSTR(WAIT_JOIN);
     toSend.replace("{#}", String(dev2Join));
     request->send(200, "text/html", toSend); //send the html code to the client
}

/* joining a device means
 - we set a permit join flag
 - we search the incoming zigbee for messages starting with 45CA or  
*/

bool waitJoin()
{
    char s_d[CC2530_MAX_SERIAL_BUFFER_SIZE] = {0};
    char toDecode[MAX_FRAME_SIZE] = {0};
    uint16_t shortAddr;
    uint8_t ieeeAddr[8];
    char shortAddress[5];
    char ieeeAddress[17];
    unsigned long timeoutMs = 60000;
    unsigned long start = millis();
    char comp[5] = {0};
    joinAbort = false;
    while (millis() - start < timeoutMs) 
    {
        if ( joinAbort ) break;
         
        strcpy(toDecode, readFilteredFrame(s_d, 5)); // get a filtered frame
    
        int len = strlen(toDecode);
        if(len > 0){
            if (len < 10 || len > 120) 
            {
              consoleOut("len not correct"); 
              continue; // too short
            }
        }
        // we need to analyze the incomming frame on 45CA or 4606
        // in case of 
        memcpy(comp, toDecode + 4, 4);
        comp[4] = '\0';
        
        if (strcmp(comp, "45CA") == 0 || strcmp(comp, "4606") == 0) 
        {
             
             if(strcmp(comp, "45CA") == 0) consoleOut("Device announce received (45CA)"); else consoleOut("Device announce received (4606)");
                // Short address (little endian)
                //memcpy(shortAddress, toDecode + 8, 4); shortAddress[4]='\0';
                //memcpy(ieeeAddress, toDecode + 12, 16); ieeeAddress[16]='\0';
                // copy directly to the Dev_Prop
               // char devAdr[5];   // 4 hex chars + null terminator
               // devAdr[0] = toDecode[8 + 2];
               // devAdr[1] = toDecode[8 + 3];
               // devAdr[2] = toDecode[8 + 0];
               // devAdr[3] = toDecode[8 + 1];
               // devAdr[4] = '\0';
               // if we reverse this the control goes wrong
               //copy devAdr
               // char *dst = Dev_Prop[dev2Join].devAdr;
               // dst[0] = toDecode[10];  // E
               // dst[1] = toDecode[11];  // 9
               // dst[2] = toDecode[8];   // 8
               // dst[3] = toDecode[9];   // 7
               // dst[4] = '\0';
               
                memcpy(Dev_Prop[dev2Join].devAdr, toDecode + 8, 4); Dev_Prop[dev2Join].devAdr[4]='\0';
                
                memcpy(Dev_Prop[dev2Join].devIeee, toDecode + 12, 16); Dev_Prop[dev2Join].devIeee[16]='\0';
                      
               consoleOut("New device joined: short=" +
                String(Dev_Prop[dev2Join].devAdr) + " ieee=" + String(Dev_Prop[dev2Join].devIeee));
                
                //addDevice(shortAddr, ieeeAddr);
                //sendSequence(shortAddr, ieeeAddr);
                
                return true;
        }
        
    
    }
   if(joinAbort) 
   {
      consoleOut("joining aborted");
   } else {
      consoleOut("No device joined within timeout");
   }

   strcpy(Dev_Prop[dev2Join].devAdr, "0000"); // put back the original value
   return false;
}

void enableJoin() 
{
    //uartBusy = true;  // reserve the uart for the pairing
    char permitCmd[]={"260836FFFCFF"};
    sendZB(permitCmd);
    empty_serial2(); // discard optional confirmation
    consoleOut("Permit join enabled for all devices");
    consoleOut("Put your device in pairing mode");
}

void disableJoin()
{
    //sendZB("260836FFFC00"); 
    char disableCmd[]={"260836FFFC00"};
    sendZB(disableCmd);
    consoleOut("Permit join disabled");
    //uartBusy = false;
}

void addDevice(uint16_t shortAddr, uint8_t ieee[8]) 
{
    if (deviceCount >= 20) return;

       // Convert shortAddr to hex string
    snprintf(Dev_Prop[dev2Join].devAdr, sizeof(Dev_Prop[dev2Join].devAdr), "%04X", shortAddr);
      
   // Convert IEEE to hex string
    snprintf(Dev_Prop[dev2Join].devIeee, sizeof(Dev_Prop[dev2Join].devIeee), "%02X%02X%02X%02X%02X%02X%02X%02X",
         ieee[0], ieee[1], ieee[2], ieee[3],
         ieee[4], ieee[5], ieee[6], ieee[7]);

    //consoleOut("New device joined: short=" + String(shortAddr, HEX) +
    //           " ieee=" + String(ieee, HEX) );
   consoleOut("New device joined: short=" +
           String(Dev_Prop[dev2Join].devAdr) +
           " ieee=" +
           String(Dev_Prop[dev2Join].devIeee));            
}


// ieeeAddr[8] = LSB first


// void sendSequence(uint16_t shortAddr, uint8_t ieeeAddr[8]) {
//     consoleOut("sending pairing sequence");
//     char s_d[254]={0}; // provide a buffer for the call to readZB
//     char messageToDecode[CC2530_MAX_SERIAL_BUFFER_SIZE] = {0};
//     // Step 1: Send Active Endpoint Request
//     sendActiveEpReq(shortAddr, ieeeAddr);

//     // Step 2: Wait for Active Endpoint Response AREQ (0x45C5)
//     // You need to read from UART here and parse the frame.
//     strcpy(messageToDecode, readZB(s_d));
//     if (readCounter < 10 ) {
//     consoleOut(F("sendSeuence: no answer on request"));  
//     return; //no answer
//     }
    
//     //uint8_t resp[64];
//     uint8_t *resp = (uint8_t*) messageToDecode;
//     // Step 3: verify it's an Active Endpoint Response (0x45C5)
//     if (resp[2] != 0x45 || resp[3] != 0xC5) {
//         consoleOut(F("Not an Active EP Response"));
//         return;
//     }

//     uint8_t status   = resp[4];
//     //uint16_t shortAddr = resp[5] | (resp[6] << 8);
//     uint8_t epCount  = resp[7];

//     if (status != 0x00) {
//         consoleOut("Active EP Req failed, status = " + String(status));
//         return;
//     }

//     //consoleOut("Device short address = " + String(shortAddr));
//     consoleOut("Device endpoints = "+ String(epCount));
//     // Step 4: Send Simple Descriptor Requests for each endpoint
//     for (int i = 0; i < epCount; i++) {
//         uint8_t ep = resp[4+i];
//         consoleOut("Requesting SimpleDesc for EP " + String(ep));
//         sendSimpleDescReq(shortAddr, ep);
//     }
// }
