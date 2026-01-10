// this function can handle all buttons on the various pages


void handleButtons(AsyncWebServerRequest *request) 
{
      int button = request->arg("BUTTON").toInt();
      //if(button == 0) { event = 0; } else { event = 1;} //manual switched on (for security)
      Serial.println("handleButtons, argument = " + String(button));
      switch(button) 
      {
         case 0: // on/off
                      
            if(lampState[devChoice].onoff) bulbOnOff(devChoice, false, true); else bulbOnOff(devChoice, true, true);
            break;        
          case 1:
            if(lampState[devChoice].onoff) bulbColorTemp(devChoice, 2); // warmwhite
            break;
          case 2:
            if(lampState[devChoice].onoff) bulbColorTemp(devChoice, 3); // neutral white
            break;
          case 3:
            if(lampState[devChoice].onoff) bulbColorTemp(devChoice, 4); // cool white
            break;
          case 4:
            if(lampState[devChoice].onoff) 
            { 
                bulbSetHue(devChoice, lampState[devChoice].hue, 0);
                bulbSetSat(devChoice, lampState[devChoice].sat, 0);
            }  
            break;
          case 11:
            //on/off button on switch control
                if(Dev_Prop[devChoice].values[0] == 1) switchOnOff(devChoice, false, true); else switchOnOff(devChoice, true, true);
            break;                                               
       }
   eventSend(0);
}