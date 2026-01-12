const char BULB_CONTROL[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><meta charset='utf-8' name="viewport" content="width=device-width, initial-scale=1"><title>ESP32=Z2M</title>
<link rel="stylesheet" type="text/css" href="/STYLESHEET">  
<link rel="icon" type="image/x-icon" href="/favicon.ico"  />
  <style type='text/css'>
  .slidecontainer {  width: 90%;}
  .slider {  -webkit-appearance: none;  width: 94%;  height: 28px;  background: linear-gradient(to left, white, black);  border: solid 1px;  border-radius:10px;  outline: none;  opacity: 1.0;  -webkit-transition: .2s;  transition: opacity .2s;}
  .slider:hover {  opacity: 1;}
  .slider::-webkit-slider-thumb {  -webkit-appearance: none;  appearance: none;  width: 25px;  height: 25px;  background: white;  border:solid 1px;  border-radius:10px;  cursor: pointer;}
  .slider::-moz-range-thumb {  width: 25px;  height: 25px;  background: white;  border:solid 1px; cursor: pointer;}
  .BSat{background: linear-gradient(to left, grey, white); }
  .BHue{background: linear-gradient(to right, white, red);}
  .BDim{background: linear-gradient(to left, white, black); }
  .bt_on{ background:green; color:black;}
  .bt_on::before{content:"RESUME";}
.bt, .bt2{
    display:block;
    width:124px;
    height:44px;
    padding:6px;
    margin:6px;
    text-align:center;
    border-radius:10px;
    color:white;
    font-weight:bold;
    font-size:18px;
    text-decoration:none;
    box-shadow: 0 6px #999; 
    }

  .bt:active {
    background-color: #3e8e41;
    box-shadow: 0 2px #666;
    transform: translateY(4px);
  }  
   .red{
    background:#b9b9c1;
    color:white;
    border:2px solid black;
    }
  .rad{
    background:green;
    color:white;
    border:2px solid black;
    }
  #slider-label, {
    font-size: 12px;
    color: black;
  }

@media only screen and (max-width: 800px) { 
.bt {width: 80px; font-size: 12px;}
}


</style> 
</head>
<body onload='loadScript()'>
<div id='msect'>
<div id='menu'>
<a  href="/" class='close' >&times;</a>
<a href=# id="goBack">devices</a>
<a href="#" id="sub" style='background:green; display: none' onclick='submitFunction()'>save</a><br>
</div>
<kop>ESP32-Z2M CONTROL BULB <span id="NAME"></span></kop><br>
<div class='divstijl' font-size:20px;' id='maindiv' style='height:72vh;'>
<center>  
<b id="pwdby">powered by Hansiart</b><br>
<br>
<table>
  <tr><td><button id='bt0' onclick='buttonFunction(0)' class='bt red'>OFF</button></td>
  <td><button id='bt1' onclick='buttonFunction(1)' class='bt red'>WARM</button></td>
  <td><button id='bt2' onclick='buttonFunction(2)' class='bt red'>NEUTRAL</button></td>  
  <tr><td><button id='bt3' onclick='buttonFunction(3)' class='bt red'>COOL</button></td>
  <td><button id='bt4' onclick='buttonFunction(4)' class='bt red'>COLOR</button></td>
  <td><button id='bt5' onclick='buttonFunction(5)' class='bt red'>PRESET</button></td>
  </table>
  <br><br>
  <div class='slidecontainer'>
  <label for="hue" id="slider-label">hue</label>
  <input type="range" id="tuneHue" name="hue" min="0" max="359" class="slider BHue" value="{HUE}" onchange='colorFunction()'>
  </div><br>
  <div class='slidecontainer'>
  <label for="tuneHue" id="slider-label">saturation</label>
  <input type='range' id='tuneSat' name="sat" max='99' min='1' class='slider BSat' value='{SAT}' onchange='colorFunction()'></td></tr>
  </div><br>
  <div class='slidecontainer'> 
   <label for="dim" id="slider-label">level</label>
  <input type="range" id="tuneDim" name="dim" min="0" max="100" class="slider BDim" value="{Dim}" onchange='dimFunction()'>
  </div><br>
  </div>
</div>
<script defer>
var myLink=document.getElementById("goBack");
document.addEventListener("visibilitychange", function() {
    if (!document.hidden){
        console.log("Browser tab is visible")
        getData(); // update the page 
    } 
});

const btn = document.getElementById('bt4');
var lampwaarde = 0;
function loadScript() {
  loadData();
}

function buttonFunction(button) {
var xhttp = new XMLHttpRequest();
var buttonPressed="?BUTTON=" + button;
var what = "/BUTTON" + buttonPressed;
xhttp.open("GET", what, true);
xhttp.send();
//if(button==4) alert("preset saved!"); 
}

function colorFunction() {
var xhttp = new XMLHttpRequest();
var sliderSat = "?SAT=" + document.getElementById('tuneSat').value;
var sliderHue = "&HUE=" + document.getElementById('tuneHue').value;
var what = "/COLOR" + sliderSat + sliderHue; //COLOR?SAT=xxx&HUE=yyy
xhttp.open("GET", what, true);
xhttp.send(); 
}

function dimFunction() {
var xhttp = new XMLHttpRequest();
var sliderDim = "?level=" + document.getElementById('tuneDim').value;
var what = "/DIM" + sliderDim; //DIM?level=zzz
xhttp.open("GET", what, true);
xhttp.send(); 
}

// request data at onLoad and onEvent
function loadData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var antwoord = this.responseText;
      var obj = JSON.parse(antwoord);
      var sat = obj.sat;
      var hue = obj.hue; 
      var dim = obj.dim;     
      var onoff = obj.onoff;
      var lampState = obj.state;
      var devnr = obj.devnr;
      myLink.href = "/DEV?welke="+devnr;
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
      if (lampState == 2 && onoff == 1) { //warm is on
      document.getElementById("bt1").style.background="green";
      } else { 
        document.getElementById("bt1").style.background ="#b9b9c1";
      }        
      if (lampState == 3 && onoff == 1) { // neutral is on
      document.getElementById("bt2").style.background="green";
      } else { 
        document.getElementById("bt2").style.background ="#b9b9c1";
      }
      if (lampState == 4 && onoff == 1) { // cool is on
      document.getElementById("bt3").style.background="green";
      } else { 
        document.getElementById("bt3").style.background ="#b9b9c1";
      }
        
      if (lampState ==  5 && onoff == 1 ) { // color is on
      document.getElementById("bt4").style.background="green";
      } else { 
        document.getElementById("bt4").style.background ="#b9b9c1";
      }
      
      if (lampState == 6) { // color
      document.getElementById("bt5").style.background="green";        
      } else { document.getElementById("bt5").style.background ="#b9b9c1"; }    

      
      document.getElementById("tuneSat").value=sat;
      document.getElementById("tuneHue").value=hue;
      document.getElementById("tuneDim").value=dim;
    }
  };
  xhttp.open("GET", "get.Data?Slider=1", true);
  xhttp.send();
}

function loadSliders() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var antwoord = this.responseText;
      var obj = JSON.parse(antwoord);
      var bright = obj.h_state;
      document.getElementById("tuneSat").value=sat;
      var roodWaarde = obj.r_state;
      document.getElementById("tuneHu").value=hue;
     
    }
  };
  xhttp.open("GET", "get.Data?Sliders=1", true);
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
     loadData();
  }

 }, false);

}
</script>
  </body></html>
   )=====";