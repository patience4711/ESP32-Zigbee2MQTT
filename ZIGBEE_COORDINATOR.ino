/* this is a tricky process as it is hard to check if it does what it should.
 *  if brought up with sprintf constructed commands, it seems we can pair but 
 *  get no polling answers.
 *  This is not caused by only the last NO command. It seems that if we feed sendZB() with a 
 *  char that contains empty space, it goes wrong. 
 *  To send the relative commands i tried something like:
 *  for ( int y=0; y < 8; Y++ ) {
 *    snprintf(command, "A123B456C%sA1A2B3B4C5C6", invID); // or similar
 *    sendZB(command);
 *    etc; 
 *  }
 *  This works with pairing ( at least we get the invID ) but maybe it wasn't paired at all. 
 *  Anyway the polling fails constantly, also after rebooting the inverter.   
 *  command 2700 gives us the answer that the coordinator is up. So that proves nothing.
 *  I haven't investigate this further, for now only the method used here works
 */

bool coordinator() 
{ // if true we send the extra command for normal operations
    Serial.println(F("starting coordinator"));
 
    startCoordinator(); // will result in zigbeeUP 1 or 0
    
    if ( zigbeeUp == 1 )
    {
        Update_Log(2, "started");
        consoleOut(F("ZB coordinator started"));  
        ledblink(5,100);
        return true;
      
    } else {
        Update_Log( 2 , "failed");
        consoleOut(F("starting ZB coordinator failed"));
        return false;  
    }
        
    return false;
}

void startCoordinator() 
{
    //  char initCommand[][200] = {
    //   "260587000100", // Set logical type = Coordinator
    //   "2605830002631A",    // Set PAN ID = 0x1A63
    //   "260584000400F8FF07", // Channel mask
    //   "26050108FFFF", // 3  coordinator id
    //   "2605870100",  // 4 what
    //   "4100",  // 5  reset
    //   "2600", // ZB_START_REQUEST
    // };
    char sendCmd[50];
    ////"26050008AA01BB01CC01DD01",
    uartBusy = true;
    //char messageToDecode[100] = {0};
    Serial.println("init zb coordinator");
    char s_d[100]={0}; // provide a buffer for the call to readZB
    zigbeeUp = 11; //initial it is initializing 11, 0=down 1=up
    yield();
    //char initCmd[100];    
    //uint8_t payload[30];
    // first empty the buffer inMessage
    empty_serial2(); // make the buffer empty
    // first get the mac of the esp
    
    // we start with a hard reset of the zb module
    ZBhardReset();
    delay(500);
    writeCoordinatorConfig();
    
    strcpy(sendCmd,"2600");
    sendZB(sendCmd); // oke  
    decodeGeneralAnswer();

    zigbeeUp = 0;
    
    if(checkCoordinator() == 0)
    {
        consoleOut("Coordinator started successfully");
        zigbeeUp = 1;
        register_endpoints();
    } else {
        consoleOut("Coordinator start failed");
    }
    
  empty_serial2(); //cleanup
  uartBusy = false;
} 

void writeCoordinatorConfig()
{
Serial.println(F("\n\nwrite settings to cc25xx"));
char mes[50];
        //ZB_WRITE_CONFIGURATION (0x2604)
        // we start with a hard reset of the zb module
        ZBhardReset();
        delay(500);
        strcpy(mes, "2605870001"); // ZB_WRITE_CONFIGURATION CMD Set as Coordinator oke  attr 87
        consoleOut("Set as coordinator = " + String(mes));
        sendZB(mes);
        waitSerial2Available();
        empty_serial2();
        decodeGeneralAnswer();
        
        strcpy(mes, "260585000400000800"); // Channel List 11  oke
        consoleOut("Set Channel List = " + String(mes));
        sendZB(mes);
        waitSerial2Available();
        empty_serial2();
        //decodeGeneralAnswer();

        strcpy(mes, "2605830002621A"); // Set PAN ID = 0x1A62 attr 83
        consoleOut("Set PAN ID = " + String(mes));
        sendZB(mes);
        waitSerial2Available();
        empty_serial2();
        //decodeGeneralAnswer(); 
        
        strcpy(mes, "2605840008CCCCCCCC00000000"); //Extended PANID
        consoleOut("Set Extended PAN ID = " + String(mes));
        sendZB(mes);
        waitSerial2Available();
        empty_serial2();
        //decodeGeneralAnswer(); 

                  // 260587005A6967626565416C6C69616E63653039
        strcpy(mes, "260562105A6967426565416C6C69616E63653039"); // ZCD_NV_PRECFGKEY set preconfigured key ZigbeeAlliance09
        consoleOut("set preconfigured key = " + String(mes)); // ok 
        sendZB(mes);
        waitSerial2Available();
        empty_serial2();
        //decodeGeneralAnswer(); 
         
        strcpy(mes, "2605640101"); // ZCD_NV_PRECFGKEY set proconfigured key
        consoleOut("enable security mode = " + String(mes)); // enale
        sendZB(mes);
        waitSerial2Available();
        empty_serial2();
        //decodeGeneralAnswer(); 

        strcpy(mes, "26058F0100"); // startup option (clear before start)
        consoleOut("start-up option = " + String(mes)); // enale
        sendZB(mes);
        waitSerial2Available();
        empty_serial2();
        //decodeGeneralAnswer(); 

        strcpy(mes, "2601"); // reset
        consoleOut("reset = " + String(mes)); // enale
        sendZB(mes);
        waitSerial2Available();
        empty_serial2();
        //decodeGeneralAnswer(); 
        
}
  


void register_endpoints() 
{
  char mess[100];
  strcpy(mess, "2400010401000001010402010000"); //temperature sensor
  sendZB(mess);
  decodeGeneralAnswer();
  strcpy(mess, "24000104010100010003000006000604010600"); // motion sensor
  sendZB(mess);
  decodeGeneralAnswer();
  strcpy(mess, "240001040100010004000006000800000303060008000003"); // smart bulb
  sendZB(mess);
  decodeGeneralAnswer();
}