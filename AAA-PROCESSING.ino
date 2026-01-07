void processNormal()
{
// the input of this function is a string containing 4481

    int deviceNr = -1;
    float temp;
    float hum;
    int idx = 0;
    int deviceType = 0;
    bool mosquitto = false;
    char toMQTT[100]={0};
    char messageToDecode[CC2530_MAX_SERIAL_BUFFER_SIZE] = {0};
    char s_d[CC2530_MAX_SERIAL_BUFFER_SIZE] = {0};
    consoleOut("processing a normal frame");
    strcpy(messageToDecode, readFilteredFrame(s_d, 1));
    consoleOut("The messageToDecode is " + String(messageToDecode));
    
    if(strlen(messageToDecode) < 5) {
      consoleOut("nothing usefull to decode");
      return;
    }

    char *tail;
    int fault=0;               

    consoleOut("deviceCount = " + String(deviceCount));
      //the first thing we need to find out is for which device the message is
    for(int x=0; x < deviceCount; x++)
    {
      if (strstr(messageToDecode, Dev_Prop[x].devAdr) != NULL) {
          deviceNr = x ;
          consoleOut("Found device # " + String(x));
          idx=Dev_Prop[x].devIdx;
          deviceType = Dev_Prop[x].devType;
          consoleOut("the device type = " + String(deviceType));
          consoleOut("the device idx = " + String(idx));
          break;
      }
    }
    if( deviceNr == -1 ) {
        consoleOut("unknown device");
        return;
    }
       
    if(deviceType == 1 && strlen(messageToDecode) < 90) 
    {
          tail = split(messageToDecode, "44810000"); // remove the 0000 as well
          consoleOut("tail " + String(tail) );
            if (strncmp(tail, "0204", 4) == 0)
              {
              //memcpy(devAddr, messageToDecode + 16, 4);
              consoleOut("temperature from 0x" + String(Dev_Prop[deviceNr].devAdr));
              //deviceNr = deviceLookup(devAddr);
              temp =  extractValue(42, 4, 0.01f, 0, tail); //
              //temp = extractValueBB(42, 2, 0.01f, 0, true, tail);
              consoleOut("temperature = " + String(temp, 1));
              Dev_Prop[deviceNr].values[0] = temp;
              Dev_Prop[deviceNr].toMqtt ++;
              //printf("Temperature = %.2f°C\n", temp);
              
              }
            //if(strstr(messageToDecode, "448100000504") && strlen(messageToDecode) < 70) 
            if (strncmp(tail, "0504", 4) == 0)
            {
              //memcpy(devAddr, messageToDecode + 16, 4);
              consoleOut("humidity from 0x" + String(Dev_Prop[deviceNr].devAdr));
              //deviceNr = deviceLookup(devAddr);
              hum =  extractValue(42, 4, 0.01f, 0, tail);
              // float hum = extractHumidity(tail);
              consoleOut("humidity = " + String(hum,1));
              Dev_Prop[deviceNr].values[1] = hum;
              Dev_Prop[deviceNr].toMqtt ++;
              if(interviewFlag == deviceNr) 
                  { 
                  interviewDevice(deviceNr); 
                  consoleOut("interviewing device " + String(deviceNr));
                  interviewFlag = 20;
                  }
            }
    
            // decide if we should send an mqtt message
            // for a temperature humidity sensor we do this when both values are present
            if(Dev_Prop[deviceNr].toMqtt == 2)
            {
                consoleOut("deviceNr = " + String(deviceNr));
                //char toMQTT[100]={0};
                snprintf(toMQTT, sizeof(toMQTT), "{\"idx\":%d,\"nvalue\":0,\"svalue\":\"%.1f;%.2f\"}" , idx, Dev_Prop[deviceNr].values[0], Dev_Prop[deviceNr].values[1]); 
                // mqttConnect() checks first if we are connected, if not we connect anyway
                consoleOut("toMQTT = " + String(toMQTT));
                //if(mqttConnect() ) MQTT_Client.publish ( Mqtt_send, toMQTT, false );
                if(mqttConnect() ) MQTT_Client.publish ( Mqtt_outTopic, toMQTT ); 
                Dev_Prop[deviceNr].toMqtt = 0;
            }
          return;
    } // end if devicetype == 1

    if(deviceType == 2 )  // the motion sensor
      {
          consoleOut("decoding a motion sensor");
          // the message probably means there was motion o there is no more motion
          // if the device needs to be interviewed 
          // if(interviewFlag == deviceNr) 
          if (idx != 0) {
              snprintf(toMQTT, sizeof(toMQTT), "{\"idx\":%d,\"nvalue\":0,\"svalue\":\"%.1d\"}" , idx , 1); 
              consoleOut("toMQTT = " + String(toMQTT));
              if(mqttConnect() ) MQTT_Client.publish ( Mqtt_outTopic, toMQTT );
          }
      }
    if(deviceType == 4 )  // the motion sensor
      {
          consoleOut("decoding a smart socket ");
          // the reading consist of 3 consecutive messages
          //  we discard the last 2 messages

          // now we have the message that probably means there was motion
          // if the device needs to be interviewed 
          //if(interviewFlag == deviceNr) 
      }


      //    
      
      
      // if(mosquitto){ 
      // // mqttConnect() checks first if we are connected, if not we connect anyway
      // consoleOut("toMQTT = " + String(toMQTT));
      // if(mqttConnect() ) MQTT_Client.publish ( Mqtt_outTopic, toMQTT );      
      // triggerTime = millis();
      // mqttFlag = idx; 
      // }
} 

void processAll()
{
// this function reeds all incoming messages unfiltered
    char messageToDecode[CC2530_MAX_SERIAL_BUFFER_SIZE] = {0};
    char s_d[CC2530_MAX_SERIAL_BUFFER_SIZE] = {0};
    //consoleOut("processing a normal frame");
    strcpy(messageToDecode, readZB(s_d));
    consoleOut("The messageToDecode is " + String(messageToDecode));
    consoleOut("normalOps = 9, no processing");
    }

    // *******************************************************************************************************************
//                             extract values
// *******************************************************************************************************************
float extractValue(uint8_t startPosition, uint8_t valueLength, float valueSlope, float valueOffset, char toDecode[CC2530_MAX_SERIAL_BUFFER_SIZE])
    {
    char tempMsgBuffer[64] = {0}; // was 254
    yield();
    // first the last byte
    strncpy(tempMsgBuffer, toDecode + startPosition + 2, valueLength/2);
    //than the fst byte
    strncat(tempMsgBuffer, toDecode + startPosition, valueLength/2);
    consoleOut ("extracted at offset 41: " + String(tempMsgBuffer));
    // now we have the part of the string "startposition - number of bytes"
    // we calculate the value it is representing with strtol and correct it with valueSlope and offset
    yield();
    // *** IMPORTANT: interpret as signed 16-bit ***
    int16_t signedValue = (int16_t)strtol(tempMsgBuffer, NULL, 16);
    //return (valueSlope * (float)strtol(tempMsgBuffer, 0, 16)) + valueOffset;
    return (valueSlope * signedValue) + valueOffset;
}


// ********************************************************************************************************
//                                    byte based extract value
// ********************************************************************************************************
float extractValueBB(uint8_t startPosition, uint8_t byteCount,
                   float valueSlope, float valueOffset,
                   bool isSigned,
                   const uint8_t *payload)
{
    // Build the integer from bytes (Zigbee sensors normally send BIG-endian)
    uint32_t raw = 0;

    for (uint8_t i = 0; i < byteCount; i++) {
        raw = (raw << 8) | payload[startPosition + i];
    }

    // Handle signed values using two's complement
    if (isSigned) {
        uint32_t signBit = 1UL << (byteCount * 8 - 1);

        if (raw & signBit) {
            uint32_t mask = (1UL << (byteCount * 8)) - 1;
            raw = raw - (mask + 1);
        }
    }

    return valueSlope * (int32_t)raw + valueOffset;
}



int deviceLookup(const char compare[5])
{
  for (int z=0; z < deviceCount; z++) 
        if (strcmp(compare, Dev_Prop[z].devAdr) == 0) {
            return z;
        }
  return 99;
}