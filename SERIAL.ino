void handle_Serial () 
{
      //DebugPrintln(F("we are in handle serial"));  
      int SerialInByteCounter = 0;
      InputBuffer_Serial[0] = '\0'; // make it like empty
      byte SerialInByte;  
      delay(200); // wait untill maybe more data available
      Serial.println("serial data available: " + String(Serial.available()) );
  
      while(Serial.available()) 
      {
             SerialInByte=Serial.read(); 
            
            if(isprint(SerialInByte)) {
              if(SerialInByteCounter<100) InputBuffer_Serial[SerialInByteCounter++]=SerialInByte;
            }    
            if(SerialInByte=='\n') {                                              // new line character
              InputBuffer_Serial[SerialInByteCounter]=0;
              break; // serieel data is complete
            }
      }  
     Serial.println("InputBuffer_Serial = " + String(InputBuffer_Serial) );
     diagNose = 2; // direct the output to serial
     Serial.println("\nType 10;HELP to list available commands");
     if (strlen(InputBuffer_Serial) > 6) 
     {                                // need to see minimal 8 characters on the serial port
        if (strncmp (InputBuffer_Serial,"10;",3) == 0) 
        {                 // Command from Master to RFLink
  
          if (strncasecmp(InputBuffer_Serial+3,"HELP",4) == 0) {
              scroll(4);
              Serial.println(F("*** AVAILABLE COMMANDS ***"));
              Serial.println(F("10;DIAG=x; (if x = 1 set diagNose for serial debug)"));
              Serial.println(F("10;DEVICES; (list deivices in spiffs")); 
              Serial.println(F("10;STARTZB; (start zb coordinator)"));                           
             // Serial.println(F("10;ZBT=; (send zigbee message, e.g. 10;zbt=2101 (ping))"));
              Serial.println(F("10;DELETE=<file>; (delete a file)"));              
              Serial.println(F("10;HEALTH; (perform healthcheck zigbee)"));             
              Serial.println(F("10;SET_OPS; (set normalOps 0 1 5 7 9=unfiltered)"));

              #ifdef TEST
              Serial.println(F("10;TESTINV; (decode a testanswer for inv 0)"));
              #endif
              return;
            } else 
              
 

          if (strncasecmp(InputBuffer_Serial+3,"DEVICES",7) == 0) {  
              printDevices();
              return;           
          } else

          if (strncasecmp(InputBuffer_Serial+3,"STARTZB",7) == 0) {  
              startCoordinator();
              return;           
          } else
           if (strncasecmp(InputBuffer_Serial+3,"HEALTH",6) == 0) {  
              Serial.println(F("\n\nchecking the ZB system"));
              healthCheck();
              return;             
          } else
  
          if (strncasecmp(InputBuffer_Serial+3,"SPIFFS",5) == 0) {  
              Serial.println("printing SPIFFS");
              File root = SPIFFS.open("/");
              File file = root.openNextFile();
              uint16_t counter = 0;
              while (file) 
              {
                Serial.print("FILE: ");
                Serial.print(file.name());
                Serial.print(" | SIZE: ");
                Serial.println(file.size());
                file.close();          // REQUIRED
                yield();
                counter++;
                if (counter > 10) {
                  Serial.println("BREAK");
                  break;
                }
                file = root.openNextFile();
              }
              root.close(); 
             return;  
                        
          } else 

           if (strncasecmp(InputBuffer_Serial+3,"SET_OPS=",8) == 0) {  
              normalOps = atoi(InputBuffer_Serial + 11);
              Serial.println("write normalOps value: " + String(normalOps));
               return; 

          } else
           if (strncasecmp(InputBuffer_Serial+3,"DUMPVARS",8) == 0) {  
              Serial.println(F("\n\nprint some variables"));
              Serial.println("normalOps = " + String(normalOps));
              Serial.println("diagNose = " + String(diagNose));
              Serial.println("devChoice = " + String(devChoice));
              Serial.println("deviceCount = " + String(deviceCount));
              return; 
           } //else
    
          //{
          // if we are here no command was recongized
          Serial.print( String(InputBuffer_Serial)); Serial.println(F(" INVALID COMMAND" ));     
          
        } // end if if (strncmp (InputBuffer_Serial,"10;",3) == 0)
    
      } //  end if strlen(InputBuffer_Serial) > 6
    Serial.print(String(InputBuffer_Serial)); Serial.println(F(" UNKNOWN COMMAND"));
    // the buffercontent is not making sense so we empty the buffer
    empty_serial();
} // 

  void scroll(int aantal) {
    for (int x=0;x<aantal; x++) { Serial.println(F("\n")); }
  }
 