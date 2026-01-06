void handleColor(AsyncWebServerRequest *request) 
{
    // deze functie wordt opgeroepen door de sliders
    consoleOut("handleColor");
    int sat = request->getParam("SAT")->value().toInt();
    int hue = request->getParam("HUE")->value().toInt();
    //if (sat != saturation) { // has been changed?
    //   saturation = sat;
    bulbSetSat(devChoice, sat, 0); // saves the values
    bulbSetHue(devChoice, hue, 0); // saves the values
    //Update_Log("sliders" ,"unicolor on");
    //MQTT_Zend();
    // sla alle waarden op in spiffs
    //colorConfigsave();
     eventSend(0);
}
void handleDim(AsyncWebServerRequest *request) 
{
    // this function is called by a slider
    consoleOut("handleDim");
    int level = request->getParam("level")->value().toInt();

    bulbDim(devChoice, level, 0);
    eventSend(0);
    lampState[devChoice].dim = level;
    //Update_Log("sliders" ,"unicolor on");
    //MQTT_Zend();
    // sla alle waarden op in spiffs
    //colorConfigsave();
    eventSend(0);
}



