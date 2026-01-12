// *************************************************************************
//                   system healtcheck 
//**************************************************************************

void healthCheck() {
    #ifdef TEST
    return;
    #endif
    if(Mqtt_Format != 0 && Mqtt_Format != 5) {
       if (mqttConnect() ) {
       consoleOut("healt mosquitto ok");
       }
 
    }

    if(!timeRetrieved) { 
      getTijd();
      eventSend(1);
    }

    //normalOps = 7; 
    if(checkCoordinator() ) // send the 2700 command 
    {
        zigbeeUp = 1; // all oke
        //diagNose = 0; // reset diagNose as this costs cpu
        //normalOps = 1; // normal working process
    } else {
        consoleOut("zb down");
        // try to start the coordinator
        consoleOut("hc starting coordinator");
        if (coordinator() ) zigbeeUp = 1; else zigbeeUp = 0;
        eventSend(1);
      }      

}

bool checkCoordinator() {
// this is basically the 2700 command  
// the answer can mean that the coordinator is up, not yet started or no answer
// we evaluate that
// first empty serial2, comming from coordinator this is necessary;
    //empty_serial2(); is done in the loop
    //char coordinator_id[13];
   // getCoordinatorId(coordinator_id);        // fill the array CCDBA71699808099

    char * tail;

    // the response = 67 00, status 1 bt, IEEEAddr 8bt, ShortAddr 2bt, DeviceType 1bt, Device State 1bt
    //  FE0E 67 00 00 FFFF 80971B01A3D8 0000 0709001
   //   FE0E 67 00 00 D400 271F004B1200 0000 070900 D2 rc=19

    char checkCommand[10]; // we send 2700 to the zb
    //uartBusy = true;
    strncpy(checkCommand, "2700", 5);
    
    char reCeived[254]={0}; // a buffer for the received message
    char s_d[100]={0};
       // now we do this 3 times
    for (int x=1; x<3; x++)
    {
      sendZB( checkCommand );
      s_d[0]='\0';  
      // now read the answer if there is one
      strcpy(reCeived, readFilteredFrame(s_d, 7));
      delayMicroseconds(250);

      // we get this : FE0E670000 FFFF80971B01A3D8 0000 07090011 or
      //    received : FE0E670000 FFFF80971B01A3D6 0000 0709001F when ok
  
      //check if coordinator_id is in the string, then split it there

      consoleOut("checkCoordinator tail = " + String(reCeived));
          // the tail should contain 0709
      if( strstr(reCeived, "0709") ) 
      {
        consoleOut("found 0709, running oke");
        zigbeeUp = 1;
        return true;            
      } 
           
      delay(700);
      reCeived[0] = '\0';
   
      consoleOut("retrying.."); 
   }
   // if we come here 3 attempts failed       
    return false;
}



// *************************************************************************
//                          hard reset the cc25xx
// *************************************************************************
void ZBhardReset() 
    {
    digitalWrite(ZB_RESET, LOW);
    delay(500);
    digitalWrite(ZB_RESET, HIGH);
    //char term[20] = {"ZBmodule hard reset"} ;
    Update_Log(2, "hard reset");
     consoleOut("ZB module hard reset");
    delay(2000); //wait for the cc2530 to reboot
    }


    
