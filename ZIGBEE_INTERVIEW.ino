//example device with address 92e6
//active endpoint request 2505E692E692
//simple descripter 2504E692E69201 (assume endpoind 01)
//mode descripter    2502E692E692
//power descriptor   2503E682E692 optional but z2m does it
//basic cluster attributes
void interviewDevice(int which)
{
        consoleOut("interview triggered, check log!");
        debugLog = ""; // make the log empty
        diagNose = 3; // we direct all interview output to log
        consoleOut("interview device " + String(which));
        activeEpReq(which);
        customReq(); // this only works for dev 0
        diagNose = 1; // when ready direct output to console
}


void activeEpReq(int devNr) 
{
    char activeEpReqCommand[40];  // enough space for "2505" + 2 + 8*2 chars + '\0'
    char s_d[254] = {0};
    // Format the string: CMD0CMD1 + 0xFFFF + IEEE (LSB first)
    //sprintf(ActiveEpReqCommand, "2505%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
    uint16_t id; // = Dev_Prop[devNr].devAdr;
    sscanf(Dev_Prop[devNr].devAdr, "%4x", &id);
    sprintf(activeEpReqCommand, "2505%02X%02X%02X%02X",
        id & 0xFF,        // low byte  -> 0x92
        (id >> 8) & 0xFF,  // high byte -> 0xE6
        id & 0xFF,        // low byte  -> 0x92
        (id >> 8) & 0xFF  // high byte -> 0xE6
        );
    consoleOut("sending activeEpReqCommand = " + String(activeEpReqCommand));
    sendZB(activeEpReqCommand); 
    readZB(s_d);
    consoleOut(String(s_d));

}

void SimpleDescReq(int devNr) {
    uint8_t frame[2+3]; 
    int pos = 0;
    char DescReqCommand[20];
    // Format: "2504" + [shortAddr LSB] + [shortAddr MSB] + [endpoint]
    uint16_t id; // = Dev_Prop[devNr].devAdr;
    sscanf(Dev_Prop[devNr].devAdr, "%4x", &id);
    sprintf(DescReqCommand, "2504%02X%02X%02X", 
            id & 0xFF,        // LSB
            (id >> 8) & 0xFF, // MSB, endpoint
            "01");

    sendZB(DescReqCommand);
}

void customReq()
{
char s_d[254] = {0};
char customReqCommand[]={"2401E6920101000001003008000100000004000500"};
consoleOut("sending customReqCommand = " + String(customReqCommand)); 
sendZB(customReqCommand);
readZB(s_d);
consoleOut(String(s_d));
}