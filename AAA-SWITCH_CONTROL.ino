const char SWITCH_PAGE[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><meta charset='utf-8'>
<title>ESP32-Zib=gbee2Mqtt</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link href="data:image/x-icon;base64,AAABAAEAEBAQAAEABAAoAQAAFgAAACgAAAAQAAAAIAAAAAEABAAAAAAAgAAAAAAAAAAAAAAAEAAAAAAAAAAu29YA0Q8YALTd3gBm4eMA0+blAIHh4wCa2tsAdd/gALUUnQC24+IAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAARERERERERERJkZmRGZkZlEkhIhIhIhKUSSFmFmFmEpRJIVUVUVUSlEkhV3EXcRGUSSZQAAB1YpRJJlczMHVilEiGWIOAdWKESIZYgIh1aIRIhliHiIiIhEiIiIVYhYiUSIZohmaCgpRIgiiCIoiClEiJmImZmJmUREREREREREQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" rel="icon" type="image/x-icon"/>
<link rel="stylesheet" type="text/css" href="/STYLESHEET">
<style>
#demoLPM:hover {
  cursor: pointer;
  background-color: yellow;
  font-size: 20px;
}
.btlabel {text-align:center; color: white;font-size:25px;}
       
.bt2 {
  font-size:16px;
  padding: 10px;
  margin:6px;
  text-align:center;
  border-radius:8px;
  color:white;
  background: #8742f5;
  box-shadow: 0 6px #302442;
  display: block;
  }

@media only screen and (max-width: 600px) {

}
</style>
</head>
<body onload='loadScript()'>
<div id='msect'>
<div id="menu">
<a href="/MENU" style="float:right">menu</a>
</div>
</div>
<div id='msect'>
<br>
<kop>ESP32-Z2M SWITCH <span id="NAME"></span></kop>
<br></div>
<div id='msect'>
  <div class='divstijl' id = 'maindiv' style='height:74vh; background: grey;'>
  <center> <br><span id='pwdby'style='font-size:11px; margin:auto; display:table; color:white;'>powered by Hansiart</span>
<br>
<button id="bt0" class="bt" onclick="buttonFunction(11)"> </button>
<br>
</div><br>
</div></body>

<script>

document.addEventListener("visibilitychange", function() {
    if (!document.hidden){
        console.log("Browser tab is visible")
        getData(); // update the page 
    } 
});


function loadScript() {
// the first time we load the settings at once
getData();
// request data at onLoad and onEvent
}

function buttonFunction(button) {
var xhttp = new XMLHttpRequest();
var buttonPressed="?BUTTON=" + button;
var what = "/BUTTON" + buttonPressed;
xhttp.open("GET", what, true);
xhttp.send();
//if(button==4) alert("preset saved!"); 
}

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) 
    {
      var antwoord = this.responseText;
      var obj = JSON.parse(antwoord);
      var onoff = obj.onoff;
      var devnr = obj.devnr;     
      document.getElementById("NAME").textContent = devnr;
      if (onoff == 0) {
      document.getElementById("bt0").style.background="#b9b9c1";
      document.getElementById("maindiv").style.background="grey";
      document.getElementById("bt0").textContent = "OFF";
      document.getElementById("pwdby").style.color="white";
      } else { 
      document.getElementById("bt0").style.background ="green";
      document.getElementById("bt0").textContent = "ON";
      document.getElementById("maindiv").style.background="linear-gradient(#e8edc8, #c8eaed)";
      document.getElementById("pwdby").style.color="green";
      }
    }
  };
  xhttp.open("GET", "get.Data?Switch=1", true);
  xhttp.send();
}

if (!!window.EventSource) {
 var source = new EventSource('/events');

 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);

 source.addEventListener('message', function(e) {
  console.log("message", e.data);
  if(e.data == "getdata") {
     getData();
  }

 }, false);

}

</script>
</html>
)=====";


void switchSetOnOff(int devNr, bool onoff, bool mosQ) 
{
    char command[50];
    char toMQTT[100];
     
                // mqttConnect() checks first if we are connected, if not we connect anyway

    //working on command  2401CB06010106000401000E03010100
    //off                 2401CB06010106000401000E03010000
    //uint16_t id; // = Dev_Prop[devNr].devAdr;
    //uint16_t id = (uint16_t)strtoul(Dev_Prop[devNr].devAdr, NULL, 16);
    sprintf(command,
        "2401%s010106000401000E0301%s00",
        Dev_Prop[devNr].devAdr,
        onoff ? "01" : "00");
    consoleOut("the on/off command = " + String(command));
    if(onoff) Dev_Prop [devChoice].values[0] = 1; else Dev_Prop [devChoice].values[0] = 0;
    sendZB(command);
    waitSerial2Available();
    empty_serial2(); // discard the answer 
    eventSend(0);
    if(mosQ)
    {
      snprintf(toMQTT, sizeof(toMQTT), "{\"idx\":%d,\"nvalue\":%d}",Dev_Prop[devNr].devIdx, onoff);
      consoleOut("switch toMQTT = " + String(toMQTT));
      if(mqttConnect() ) MQTT_Client.publish ( Mqtt_outTopic, toMQTT );
    }
}