const char DEVCONFIG_START[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><meta charset='utf-8'>
<title>ESP-Z2M</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" type="image/x-icon" href="/favicon.ico" />
<link rel="stylesheet" type="text/css" href="/STYLESHEET">
<script type='text/javascript'>

function showSubmit() {
document.getElementById("sub").style.display = "inline-block";
}

function submitFunction(a) {
document.getElementById('formulier').submit();
}
</script>
<style>
.cap {
  font-weight:bold; 
  Background-color:lightgreen;
 }
div.overlay {
  display: block;
  width: 100%;
  height: 100%;
  background-color: rgba(0,0,0,0.7);
  z-index: 0;
  text-align: center;
  vertical-align: middle;
  line-height: 300px;
}
</style>
<script>
function cl() {
window.location.href='/MENU';
}
</script>
</head>
<body onload='%LOADBAG%'>

<div id='msect'><div id='bo'><div>
<div id='menu' style='height:96px' >
<a href='/MENU' class='close'>&times;</span>
<a href='/DEV?welke=0' style='display:%none'0%>dev. 0</a>
<a href='/DEV?welke=1' style='display:%none'1%>dev. 1</a>
<a href='/DEV?welke=2' style='display:%none'2%>dev. 2</a>
<a href='/DEV?welke=3' style='display:%none'3%>dev. 3</a>
<a href='/DEV?welke=4' style='display:%none'4%>dev. 4</a>
<a href='/DEV?welke=5' style='display:%none'5%>dev. 5</a>
<a href='/DEV?welke=6' style='display:%none'6%>dev. 6</a>
<a href='/DEV?welke=7' style='display:%none'7%>dev. 7</a>
<a href='/DEV?welke=8' style='display:%none'8%>dev. 8</a>
<a href='/DEV?welke=99' style='color:#66ff33; display:%none'99%>add</a>
</div>
<center><div class='divstijl' style='height:70vh;'><center>
<form id='formulier' method='get' action='deviceconfig' oninput='showSubmit()' onsubmit="return confirm('sure to save this device?')">
    %<FORMPAGE>%
    <br>
  </div>
  </center>
  <form id='formular' method='get' action='/DEV_DEL'></form>
  <div id='menu'>
    <div id='pairknop' style='display:%none'p% >
    <a class='groen' href='/JOIN' onclick="return confirm('Are you sure you want to join this device?')">join</a>
    <a href='#' onclick='delFunction("/SW=BACK")'>delete</a>
    </div> 
    <a href='#' onclick='helpfunctie()'>help</a>
    <a href="#" id="sub" style='background:green; display: none' onclick='submitFunction("/sw=BACK")'>save</a><br>
</div>
<br>
</div>
<script type="text/javascript" src="DEVSCRIPT"></script>
</body></html>
 )=====";

 const char DEVICE_GENERAL[] PROGMEM = R"=====(
<div id='inverter0' style='display:block'>
    <table><tr><td style='width:160px;'><h4>DEVICE {nr}</h4></tr>
    </table>
        
    <br>
    <table style="background-color: lightgreen; padding:10px;">
    
    <tr><td class="cap" style="width:100px;">IEEE NR<td><input class='inp4' id='ie' name='ie' value='{0000000000000000}'></input>
    <tr><td class="cap">ADDRESS<td><input class='inp3' id='ia' name='ia' value='{0000}'></input>
    
    <tr><td class="cap">TYPE<td><select name='devt' class='sb1' id='sel' onchange='myFunction()'>
    <option value='0' devtype_0>0 NOT DEFINED</option>
    <option value='1' devtype_1>1 TEMP/HUM-SENSOR</option>
    <option value='2' devtype_2>2 MOTION-SENSOR</option>
    <option value='3' devtype_3>3 SMART-BULB</option>
    <option value='4' devtype_4>4 SMART-SOCKET</option>
    </select>
    </tr>
    <tr><td class="cap" >NAME<td class="cap" ><input class='inp4' id='in' name='in' maxlength='12' value='{name}'></input>
    <tr><td class="cap">DOM. IDX<td class="cap" ><input class='inp2' name='mqidx' value='{idx}' size='4' length='4'></td></tr>

    </tr></td>

</table></form></div>
)=====";


// **********************************************************************************
//                         script
// **********************************************************************************

const char DEV_SCRIPT[] PROGMEM = R"=====(
var myLink = 
function delFunction(a) {
  if(confirm("are you sure to delete this device ?")) {  
  document.getElementById('formular').submit();   
  }
}

)====="; 




//*******************************************************************************************
//             prepare for saving the data
// *****************************************************************************************
void handleDeviceconfig(AsyncWebServerRequest *request) 
{ 
      // form action = handleInverterconfig
      // we only collect the data for this specific inverter
      // collect the serverarguments
      strcpy(Dev_Prop[devChoice].devAdr, request->arg("ia").c_str());
      
      strcpy(Dev_Prop[devChoice].devName, request->arg("in").c_str());
      strcpy(Dev_Prop[devChoice].devIeee, request->arg("ie").c_str());
      Dev_Prop[devChoice].devType = request->arg("devt").toInt(); //values are  0 1 2 3 4 
      Dev_Prop[devChoice].devIdx = request->arg("mqidx").toInt(); //values are 0 1  
      //Inv_Dev[iKeuze].calib = request->arg("tc").toInt(); //values are 0 1
      // the selectboxes
      char tempChar[1] = "";
      //if(request->hasParam("pan1")) { Dev_Prop[iKeuze].conPanels[0] = true;} else { Dev_Prop[iKeuze].conPanels[0] = false;}  // mqselect
      //if(request->hasParam("pan2")) { Dev_Prop[iKeuze].conPanels[1] = true;} else { Dev_Prop[iKeuze].conPanels[1] = false; }
      
      //DebugPrintln("checked panels are : " + String(Inv_Prop[iKeuze].conPanels[0])+ String(Inv_Prop[iKeuze].conPanels[2])+ String(Inv_Prop[iKeuze].conPanels[2])+ String(Inv_Prop[iKeuze].conPanels[3]));
      //is this an addition?

      actionFlag = 30 + devChoice; // this means save
      if(devChoice == deviceCount) 
      {
        deviceCount += 1;
        consoleOut("we appended a device, deviceCount now : " + String(deviceCount)); 
      }
      
      String toReturn = "/DEV?welke=" + String(devChoice);
      strcpy(requestUrl, toReturn.c_str() ); 
      consoleOut("requestUrl = " + String(requestUrl));
      consoleOut("\ndeviceCount after edit (saved) = " + String(deviceCount));  
      //consoleOut("list of the files we have after edit");
      //printDevices();
      confirm();
      request->send(200, "text/html", toSend);
}

//*******************************************************************************************
//             delete a device
// *****************************************************************************************
void handleDevicedel(AsyncWebServerRequest *request, uint8_t welke) 
{ 
  // form action = handleInverterconfig
  // we only collect the data for this specific inverter
  // read the serverargs and copy the values into the variables
    consoleOut("whichDevice = " + String(welke));
    consoleOut("deviceDel devChoice = " + String(devChoice));
    actionFlag = 20 + devChoice; // this means devicedelete   
    procesId = 3;
    String toReturn = "/DEV?welke=0";
    strcpy(requestUrl, toReturn.c_str() ); 
    confirm();
    request->send(200, "text/html", toSend);
}

void printDevices() { 
      if(diagNose == 0 ) return;     
      consoleOut(F(" ****** existing device files ******\n"));
      //if we have 4 devices devCount = 4 and last device has devNr3 (0 1 2 3)
      for (int x=0; x < deviceCount+1; x++) 
      {
      yield();
      String bestand = "/Dev_Prop" + String(x) + ".str";
      
      if(SPIFFS.exists(bestand)) 
          {
              consoleOut("filename: " + bestand);
              printStruct(bestand);
          }
         
      }
}

// say we have
// Inv_prop0.str
// Inv_prop1.str

// Inv_prop3.str
// Inv_Prop4.str

// after we found 3 and not 2 we have
// Inv_prop0.str
// Inv_prop1.str
// inv_prop2.str
// Inv_prop3.str we renamed this one so the gap moved
// Inv_Prop4.str

// remove the gaps
void remove_gaps() {
  String bestand_1;
  String bestand_2;
  bool found = false;  

  // say we have Inv_Prop0, Inv_Prop1, Inv_Prop3, Inv_Prop4, Inv_Prop5
  // this are 5 files 
  // there can only a gap of 1 inverter(can only remove 1 at a time) 
  // if we know the inverterCount we can search for a gap and put the last file in it
  // so if we are missing Inv_Prop1, we know that we have Inv_prop5  
  
  deviceCount = readDevicefiles(); // this should be 5 in the case above
  consoleOut("remove gaps read deviceCount " + String(deviceCount));
  for(int i=0; i < deviceCount; i++ ) { // 0 1 2 3 4
  bestand_1 = "/Dev_Prop" + String(i) + ".str";
  // if this file not exixts we know that there must be a file "?inv_Prop inverterCount.str
  if( !SPIFFS.exists(bestand_1) ) {
      consoleOut("found a gap" + bestand_1);  
      yield();
      bestand_2 = "/Dev_Prop" + String(deviceCount) + ".str"; // the last file
      if( !SPIFFS.exists(bestand_2) ) consoleOut("error, " + bestand_2 + " not exists");  
   // if we rename the last file to the gap, it keeps the old content
   // so we just copy the struct and write that to spiffs
      consoleOut("copy the last struct " + bestand_2 + " to " + bestand_1);
      
      //structCopy(i, deviceCount);
      Dev_Prop[i] = Dev_Prop[deviceCount]; // copie the entyre struct
      writeStruct(bestand_1, i); // write the copied struct
      SPIFFS.remove(bestand_2);
      //SPIFFS.rename(bestand_2, bestand_1); // file 2 becomes file 1
      return;  
      }
  }
  consoleOut("no gaps found");
} 

   

// ********************************************************************
//                     processor
// *********************************************************************
String processor(const String& var)
{
//
  // if(var == "LOADBAG") 
  // {
  //   consoleOut(F("found LOADBAG"));
  //   if(Dev_Prop[iKeuze].devType == 1) 
  //     {
  //     return F("showFunction()"); 
  //     } else {
  //     return F("hideFunction()");  
  //     }
  // }
// make the menu items visible --> works
  for(int x=0; x<9; x++) { // for every button we have to set the visibility
     String placeholder = "none'" + String(x);
     //Serial.println("placeholder = " + placeholder);
       if(var == "none'" + String(x) ) { 
        if (x < deviceCount) { return F("inline-block'"); } else { return F("none'"); }
       }
  }
//   
   if(deviceCount < 9) {
    // show the add button 
    if(var == "none'99") return F("inline-block'");   
   } else {
    if(var == "none'99") return F("none'");
   }
  
  if(var == "<FORMPAGE>"){
    consoleOut(F("found FORMPAGE"));
    return(toSend);  
  }

  if(var == "none'p") {
    String bestand = "/Dev_Prop" + String(devChoice) + ".str";
    if(SPIFFS.exists(bestand)) return "inline-block'"; else return "none'";
  }

return String(); //return empty when no match
}


// construct the form and write in a file toSend
void deviceForm() 
{
    int verklikker = 0;
    if (deviceCount >= 88 ) // if we add this = 99
    { 
        verklikker = 88;
        deviceCount -= verklikker; // restore the original inverterCount
    }
    deviceCount += verklikker; // add 88 again
    // now we have 3 situations
    // inverterCount == 0, show the page currently no inverters
    // iKeuze < invertercount, we have an existing inverter
    // iKeuze == invertercount, we are adding a new inverter
    // if we clicked the add button then invertercount is at least 88
    if( deviceCount != 0 ) 
    {
   
        // **********************************************************************
        //        construct the devicepage with actual data
        // **********************************************************************
        if (deviceCount >= 88 ) deviceCount -= 88; // restore inverterCount
        toSend = FPSTR(DEVICE_GENERAL);  
        // is there a file iKeuze then
        String bestand = "/Dev_Prop" + String(devChoice) + ".str";
        if(SPIFFS.exists(bestand)) 
        {
            consoleOut("deviceForm File exists " + bestand);
            //the file exists so we can display the values 
            toSend.replace("{nr}" , String(devChoice)); // vervang inverter nummer not available
            toSend.replace("{0000000000000000}", String(Dev_Prop[devChoice].devIeee)); // handled by the script
            toSend.replace("{name}", String(Dev_Prop[devChoice].devName));
            toSend.replace("{idx}", String(Dev_Prop[devChoice].devIdx));
            toSend.replace("{0000}", String(Dev_Prop[devChoice].devAdr) );
            // put back the select
            switch (Dev_Prop[devChoice].devType)
            {
               case 1:
                  toSend.replace("devtype_1", "selected");
                  break;
               case 2:
                  toSend.replace("devtype_2", "selected");
                  break;
               case 3:
                  toSend.replace("devtype_3", "selected");
                  break;
               case 4:
                  toSend.replace("devtype_4", "selected");
                  break;
            }

        } else {
        // the file does not exist so we show an empty page
        consoleOut("File does not exist");
        toSend.replace("devtype_0", "selected");
        toSend.replace("{0000000000000000}", "0000000000000000");
        toSend.replace("{name}", "");
        toSend.replace("{idx}", "0");
        toSend.replace("{0000}", "0000");
        }

    } else { // so if inverterCount == 0 we present this page
     toSend = "<br><br><br><h3>currently no devices</h3>"; 
    }
// now we have toSend ready to include in the inverterpage
}

// void structCopy(int a, int b) {

//    //copy all the values of struct 1 to struct 2  
//    strcpy(Dev_Prop[a].devName,     Dev_Prop[b].devName);
//    strcpy(Dev_Prop[a].devIeee,   Dev_Prop[b].devIeee);
//    strcpy(Dev_Prop[a].devAdr,       Dev_Prop[b].devAdr);
//    Dev_Prop[a].devType           = Dev_Prop[b].devType;
//    Dev_Prop[a].devIdx            = Dev_Prop[b].devIdx;
//    Dev_Prop[a]values             = Dev_Prop[b].values;
//    Dev_Prop[a].toMqtt            = Dev_Prop[b].toMqtt;
//    // now write file a and remove file b
// }

void deviceDelete(uint8_t devNr)
{
   String bestand = "/Dev_Prop" + String(devNr) + ".str"; // /Inv_Prop0.str
   Serial.println("remove file " + bestand ); 
 
   if(SPIFFS.exists(bestand) ) SPIFFS.remove(bestand);
   
   Serial.println("list of the files we have after removed one");
   yield();
   printDevices();
   // if we have 5 devices, devCount = 5 and the last device = 4
   // when we remove the last device we dont need to remove_gaps
   // say we removed dev 2 out of 4 devices, file remaining 0 1 3
   // deviceCount was 4 and lowered to 3
   // so when the devicnr was 2 (not deviceCount) then we need to remove the gaps
   // when the devicenr was 3 
   deviceCount -= 1; //
   if(devNr != deviceCount) {
       Serial.println("the device was not the last one, remove gaps");
       remove_gaps();
   }

   //printDevices(); 
    
   Serial.println("deviceCount after removal = " + String(deviceCount));
   //normalOps = 1; //restore zigbee reception
} 