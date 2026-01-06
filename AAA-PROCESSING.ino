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
      //consoleOut("Dev_Prop[x].devAdr = " + String(Dev_Prop[x].devAdr));
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
        consoleOut("the message could not be linked to a device");
        return;
    }
       
    if(deviceType == 1 && strlen(messageToDecode) < 90) 
    {
          tail = split(messageToDecode, "44810000"); // remove the 0000 as well
          consoleOut("tail " + String(tail) );
          //consoleOut("length messageToDecode = " + String(strlen(messageToDecode)));
          //char devAddr[5];
          //FE1C448100000204 295F 0101009D001B00C400000808920A0000297008295F1D49
          //now tail =  02040487 010100710021FF7B00000808DA0A0000297A080487
          //if(strstr(messageToDecode, "448100000204") && strlen(messageToDecode) < 90) {
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

void processHealth(char * frame)
{
Serial.println("processing a health frame");
Serial.println("The frame is " + String(frame));
}

void processJoin(char * frame)
{
Serial.println("processing a join frame");
Serial.println("The frame is " + String(frame));
}