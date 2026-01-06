# ESP32-Zigbee2MQTT
A zigbee coordinator like Zigbee2MQTT on the ESP32 platform. 
This software is basically a zigbee coordinator that, in theory, can join and control any zigbee device. 
Everything is done via a webinterface. Below is an example of the frontpage, showing joined devices.<br>
![frontPage](https://github.com/user-attachments/assets/5773e4ec-76cd-4acd-be89-c433684ed16b)

2026-01-06 download [ESP32-Zigbee2Mqtt](https://github.com/patience4711/ESP32-Zigbee2MQTT/blob/main/ESP32_ECU_v1_4.bin)

The very simple harware is: 
 - An ESP32
 - A CC2530 zigbeemodule
 - A usb power supply
<br>
The ESP32 is connected to your wifi network using the captive portal.<br>

## working
The coordinator starts a network. Within the network the coordinator can read and send messages to the devices, provided that they are joined. 
## Join a device
<br>If we have a device that we want to use, we can add the device to the system via the webui.
Once added, we can join it. In the webui we click "join" and now, for a predefined time, the system awaits a message from the device.
We put have to put the device in pairing mode, usually just reset the device'. Now its led is flashing. When it sent the message, the system
analyzes this and adds the address and ieee to the data of the device. And displays the state of the joining process to the webui.
## Control a device
In the frontpage we get a list of all devices. if it is a switch or a bulb there is a link to a control page. In case of a smart bulb we can put it on/off, set color temperature, a color by slide the hue and staturation and dim the bulb.
## read measures
In case of a sensor device, it sends messages when there is motion detected, or temp/hum readings.
These messages are analized and associated with a certain device and a adequate mosquitto message is sent.
