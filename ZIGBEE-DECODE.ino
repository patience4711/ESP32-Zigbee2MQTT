
//                    decode general zigbee answer
// ******************************************************************
// int decodeGeneralAnswer()
// {
//     char messageToDecode[CC2530_MAX_SERIAL_BUFFER_SIZE] = {0};
  
//     char s_d[CC2530_MAX_SERIAL_BUFFER_SIZE] = {0};
//     uint8_t Message_begin_offset = 0;    
//     int deviceNr = -1;
//     float temp;
//     float hum;
//     //char Mqtt_send[26]={0}; 
//     //retrieve the answer
//     strcpy(messageToDecode, readZB(s_d));
//     if (readCounter == 0) {
//         consoleOut(F("decodeGeneralAnswer: no answer on request"));  
//         return 50; //no answer
//       }


//     char *tail;
//     int fault=0;               
   
//     // temperature/humidity  sensor reading   4481000002040487010100  710021FF7B   00000808DA0A0000297A080487
//     if(strstr(messageToDecode, "44810000"))
//     {
//         consoleOut("deviceCount = " + String(deviceCount));
//          //the first thing we need to find out is for which device the message is
//        for(int x=0; x < deviceCount; x++)
//        {
//           //consoleOut("Dev_Prop[x].devAdr = " + String(Dev_Prop[x].devAdr));
//           if (strstr(messageToDecode, Dev_Prop[x].devAdr) != NULL) {
//               deviceNr = x ;
//               consoleOut("Found device # " + String(x));
//               consoleOut("the device type = " + String(Dev_Prop[x].devType));
//               break;
//           }
//        }
//        if( deviceNr == -1 ) {
//            consoleOut("the message could not be linked to a device");
//            return 51;
//        }
       
//        if(Dev_Prop[deviceNr].devType == 1 && strlen(messageToDecode) < 90) 
//         {
//               tail = split(messageToDecode, "44810000"); // remove the 0000 as well
//               consoleOut("tail " + String(tail) );
//               //consoleOut("length messageToDecode = " + String(strlen(messageToDecode)));
//               //char devAddr[5];
//               //FE1C448100000204 295F 0101009D001B00C400000808920A0000297008295F1D49
//               //now tail =  02040487 010100710021FF7B00000808DA0A0000297A080487
//               //if(strstr(messageToDecode, "448100000204") && strlen(messageToDecode) < 90) {
//                 if (strncmp(tail, "0204", 4) == 0)
//                   {
//                   //memcpy(devAddr, messageToDecode + 16, 4);
//                   consoleOut("temperature from 0x" + String(Dev_Prop[deviceNr].devAdr));
//                   //deviceNr = deviceLookup(devAddr);
//                   temp =  extractValue(42, 4, 0.01f, 0, tail); //
//                   //temp = extractValueBB(42, 2, 0.01f, 0, true, tail);
//                   consoleOut("temperature = " + String(temp, 1));
//                   Dev_Prop[deviceNr].values[0] = temp;
//                   Dev_Prop[deviceNr].toMqtt ++;
//                   //printf("Temperature = %.2f°C\n", temp);
                  
//                   }
//                 //if(strstr(messageToDecode, "448100000504") && strlen(messageToDecode) < 70) 
//                 if (strncmp(tail, "0504", 4) == 0)
//                 {
//                   //memcpy(devAddr, messageToDecode + 16, 4);
//                   consoleOut("humidity from 0x" + String(Dev_Prop[deviceNr].devAdr));
//                   //deviceNr = deviceLookup(devAddr);
//                   hum =  extractValue(42, 4, 0.01f, 0, tail);
//                   // float hum = extractHumidity(tail);
//                   consoleOut("humidity = " + String(hum,1));
//                   Dev_Prop[deviceNr].values[1] = hum;
//                   Dev_Prop[deviceNr].toMqtt ++;
//                   if(interviewFlag == deviceNr) 
//                      { 
//                       interviewDevice(deviceNr); 
//                       consoleOut("interviewing device " + String(deviceNr));
//                       interviewFlag = 20;
//                      }
//                 }
        
//                 // decide if we should send an mqtt message
//                 // for a temperature humidity sensor we do this when both values are present
//                 if(Dev_Prop[deviceNr].toMqtt == 2)
//                 {
//                     consoleOut("deviceNr = " + String(deviceNr));
//                     char toMQTT[100]={0};
//                     snprintf(toMQTT, sizeof(toMQTT), "{\"idx\":%d,\"nvalue\":0,\"svalue\":\"%.1f;%.2f\"}" , Dev_Prop[deviceNr].devIdx , Dev_Prop[deviceNr].values[0], Dev_Prop[deviceNr].values[1]); 
//                     // mqttConnect() checks first if we are connected, if not we connect anyway
//                     consoleOut("toMQTT = " + String(toMQTT));
//                     //if(mqttConnect() ) MQTT_Client.publish ( Mqtt_send, toMQTT, false );
//                     if(mqttConnect() ) MQTT_Client.publish ( Mqtt_outTopic, toMQTT ); 
//                     Dev_Prop[deviceNr].toMqtt = 0;
//                 }
//               return 0;
//         } // end if devicetype == 1

//         if(Dev_Prop[deviceNr].devType == 2 )  // the motion sensor
//          {
//           consoleOut("decoding a motion sensor");
//           // the reading consist of 3 consecutive messages
//           //  we discard the last 2 messages
//           char *secondFE = strstr(messageToDecode + 2, "FE");  // find second "FE" after the first one
//           if (secondFE != NULL) { *secondFE = '\0'; }  // terminate the first message
//           // now we have the message that probably means there was motion
//           // if the device needs to be interviewed 
//           if(interviewFlag == deviceNr) 
//           { 
//               interviewDevice(deviceNr); 
//               consoleOut("interviewing device " + String(deviceNr));
//               interviewFlag = 20;
//           }
          
          
//           char toMQTT[100]={0};
//           snprintf(toMQTT, sizeof(toMQTT), "{\"idx\":%d,\"nvalue\":0,\"svalue\":\"%.1d\"}" , Dev_Prop[deviceNr].devIdx , 1); 
//           // mqttConnect() checks first if we are connected, if not we connect anyway
//           consoleOut("toMQTT = " + String(toMQTT));
//           if(mqttConnect() ) MQTT_Client.publish ( Mqtt_outTopic, toMQTT );      
//           triggerTime = millis();
//           mqttFlag = Dev_Prop[deviceNr].devIdx; 
//         }


//     } else {
//         consoleOut("there was no 44810000 in the incoming message");
//         return 51;
//     }
//     return 0;
// } 






