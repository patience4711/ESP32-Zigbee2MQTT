# ESP32-Zigbee2MQTT
A zigbee coordinator like Zigbee2MQTT on the ESP32 platform
This software is basicly a zigbee coordinator that in theory can join any zigbee device. 
So far i was able to join and control a motion sensor, a temperature/Humidity sensor, 2 smartbulbs and a smart socket.

The harware is a 
 - ESP32
 - CC2530 zigbeemodule
 - an usb power supply
<br>
The ESP32 is connected to your wifi network using the captive portal.<br>
## Join a device
<br>If we have a device that we want to use, we have to add the device to the system via the webui.
When added the new device, we can join it. In the webui we click "join" and now the system awaits a message from the device for a certain time.
We put the device in pairing mode, usually just reset the device'. Now its led is flashing. When it sent the message, the system
analyzes this and add the address and ieee to the data of the device. And displays the state of the joining process to the webui.
## Control a device
In the frontpage we get a list of all devices. if it is a switch or a bulb there is a link to a control page. In case of a smart bulb we can put it on/off, set color temperature, a color by slide the hue and staturation and dim the bulb.
## read measures
In case of a sensor device, is sends messages when there is motion detected, or temp/hum readings
These nessages are analized and associated to a certain device and a mosquitto message is sent.
