/*
        
readZB  FE0C 45CA 3143 7C826D8FAB38C1A4  00001B rc=17
short address 4331 and ieee A4C138AB8F6D827C

FE14670000FFFFCCDBA71699800000070903295F87E93143AB rc=25

endpoint reg 240001040100010004000006000800000303060008000003

working on /of 2401314301010600040100  0E03010000   
dim 50%        2401314301010800040100  06030104 7F0A00 50% = 0x7F so 100 is 254 0x00 … 0xFE   (0 … 254 decimal)
color temp     2401314301010003040100  0703010A 7201 0A00 warm white
               2401314301010003040100  0703010A FA00 0A00 neutral white
               2401314301010003040100  0703010A 9900 0A00 cool white
RGB            2401314301010003040100  08030107 2CB33F4C 0A00   red  xy method X, Y range: 0x0000 … 0xFEFF (0 … 65279)
               2401314301010003040100  08030107 2C23 BF7F 0A00   green
               2401314301010003040100  08030107 8F153A0A 0A00   blue

*/

void bulbOnOff(int devNr, bool onoff) 
{
    char command[50];

    //uint16_t id; // = Dev_Prop[devNr].devAdr;
    uint16_t id = (uint16_t)strtoul(Dev_Prop[devNr].devAdr, NULL, 16);
    sprintf(command,
        "2401%02X%02X01010600040100040301%s00",
        id & 0xFF, (id >> 8) & 0xFF,
        onoff ? "01" : "00");
    consoleOut("the on/off command = " + String(command));
    if(onoff)lampState[devChoice].onoff = true; else lampState[devChoice].onoff = false;
    sendZB(command);
    waitSerial2Available();
    empty_serial2(); // discard the answer  
}

void bulbDim(int devNr, int Dim, int trans) 
{
    char command[50];
    // clamp inputs
    if (Dim > 100) Dim = 100;
    if (trans > 0xFFFF) trans = 0xFFFF;
    //uint16_t id; // = Dev_Prop[devNr].devAdr;
    uint16_t id = (uint16_t)strtoul(Dev_Prop[devNr].devAdr, NULL, 16);
    uint8_t level = (Dim * 254) / 100;
    //example 24013143 01010800040100060301 047F0A00
    //        24013143 01010003040100060301  00440000
    // Move to Level (with On/Off) — command 0x00
    sprintf(command,
        "2401%02X%02X0101080004010006030104%02X%02X%02X",
        id & 0xFF,
        (id >> 8) & 0xFF,
        level,
        trans & 0xFF,
        (trans >> 8) & 0xFF
    );
    consoleOut("the dim command = " + String(command));
    lampState[devChoice].dim = Dim;
    saveLamp(devChoice, lampState[devChoice]);
    sendZB(command);
    waitSerial2Available();
    empty_serial2(); // discard the answer  
}

void bulbColorTemp(int devNr, int temp) 
{
//consoleOut("we are in bulbColorTemp");
    char command[60];
    char lightTemp[5];
    strcpy(lightTemp, "7201");
    lampState[devChoice].state = 2;
    if(temp == 3) {strcpy(lightTemp, "FA00"); lampState[devChoice].state = 3;}
    if(temp == 4) {strcpy(lightTemp, "9900"); lampState[devChoice].state = 4;}
    //consoleOut("lightTemp = " + String(lightTemp));
    
    //uint16_t id;
    //sscanf(Dev_Prop[devNr].devAdr, "%4x", &id);
    uint16_t id = (uint16_t)strtoul(Dev_Prop[devNr].devAdr, NULL, 16);
    //                2401 3143   010100030401000703010A 7201 0A00 warm white
    sprintf(command, "2401%02X%02X010100030401000703010A%s0A00", id & 0xFF,(id >> 8) & 0xFF, lightTemp );
    //if(temp==2) sprintf(command, "2401%02X%02X010100030401000703010A%s0A00", id & 0xFF,(id >> 8) & 0xFF, "FA00" ); 
    //if(temp==3) sprintf(command, "2401%02X%02X010100030401000703010A%s0A00", id & 0xFF,(id >> 8) & 0xFF, "9900" ); 
    consoleOut("the colortemperature command = " + String(command));
    saveLamp(devChoice, lampState[devChoice]);
    sendZB(command);
    waitSerial2Available();
    empty_serial2(); // discard the answer
}

void bulbSetHue(int devNr, int Hue, int trans) 
{
consoleOut("we are in bulbSetHue");
    char command[60];
    if (Hue > 360) Hue = 360;
    lampState[devChoice].hue = Hue;
    uint8_t zigHue = (uint8_t)((Hue * 254) / 360);
    uint16_t id = (uint16_t)strtoul(Dev_Prop[devNr].devAdr, NULL, 16);
    //                2401 3143   010100030401000703010A 7201 0A00 warm white
    //command move to hue
        sprintf(command,
        "2401%02X%02X0101000304010008030100%02X00%02X%02X",
        id & 0xFF,
        (id >> 8) & 0xFF,
        zigHue,
        trans & 0xFF,
        trans >> 8
    );
    
    // a valid command is 24013143010100030401000803010055000100
    //sprintf(command, "2401%02X%02X0101000304010008030100%s0000", id & 0xFF,(id >> 8) & 0xFF, zigHue );
    consoleOut("the color command = " + String(command));
    lampState[devChoice].state = 5;
    saveLamp(devChoice, lampState[devChoice]);
    sendZB(command);
    waitSerial2Available();
    empty_serial2(); // discard the answer

}
void bulbSetSat(int devNr, int Sat, int trans) 
{
    consoleOut("we are in bulbSetSat");
    char command[60];
    //if (sat < 0) sat = 0;
    if (Sat > 100) Sat = 100;
    lampState[devChoice].sat = Sat;
    uint8_t zigSat = (uint8_t)((Sat * 254) / 100);
    uint16_t id = (uint16_t)strtoul(Dev_Prop[devNr].devAdr, NULL, 16);
    //                2401 3143   010100030401000703010A 7201 0A00 warm white
        
    // a valid command is 24013143010100030401000803010355000100
   
    // Command 0x03 Move to Saturation
    sprintf(command,
        "2401%02X%02X0101000304010007030103%02X%02X%02X",
        id & 0xFF,
        (id >> 8) & 0xFF,
        zigSat,
        trans & 0xFF,
        trans >> 8
    );

    consoleOut("the saturation command = " + String(command));
    lampState[devChoice].state = 5;
    saveLamp(devChoice, lampState[devChoice]);
    sendZB(command);
    waitSerial2Available();
    empty_serial2(); // discard the answer
}
