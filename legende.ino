/*
🔹 System / Firmware info

SYS_VERSION (41 02)

UTIL_GET_DEVICE_INFO (27 00)

sendZB("2700");
returns FE1067 0000 FFFF CCDBA7169980000007090173DB6F
→ Returns: Short (NWK) address, IEEE (EUI-64) address, Device type (coordinator/router/end device) State

🔹 Network info

ZB_GET_DEVICE_INFO (26 06) → Returns device info table (PAN ID, NWK address, channel, etc.).

ZB_READ_CONFIGURATION (26 04)
For example, to read PAN ID: sendZB("26048300");
the zigbee message = 250015DB8E6C7838C1A4
sendZB FE08250015DB8E6C7838C1A424
       FE082500846523BD7238C1A47D
FE0A2500A4C138786C8EDB15000026
eadZB FE0165000064

readZB FE0C 45CA B211 846523BD7238C1A4 000070 rc=17

pairing bulb
Device announce received (45CA)
sending pairing sequence
sendZB FE0A2505BBB9A4C1388CCCB1DE9BC1
readZB FE0165050061 rc=6
sendSeuence: no answer on request
Short Address: b9bb
IEEE Address: 9bdeb1cc8c38c1a4

pairing temperature sensor
readZB FE0C45CA0487846523BD7238C1A4000050 rc=17
Device announce received (45CA)
sending pairing sequence
sendZB FE0A25050487A4C13872BD236584F9
readZB FE0165050061 rc=6
sendSeuence: no answer on request
Short Address: 8704
IEEE Address: 846523bd7238c1a4
incoming frames from the temperature sensor
FE1C44810000 0204 0487 010100 730075FA4500000808B10A000029520804871DB3
FE1C44810000 0204 0487 010100 7B0007034600000808B10A000029520804871D33
FE1C44810000 0504 0487 010100 7B00BC1C4600000808B20A000021E01504871D34
FE1C44810000 0504 0487 010100 7B004E254600000808B20A000021E01504871DFF
FE1F44810000 0100 0487 010100 7B0093E2F100000B08B50A2000201E210020C8 0487 1D56
FE1F44810000 0100 0487 010100 7E0026EBF100000B08B50A2000201E210020C8 0487 1DEF 

register an endpoint with cluster 0006
sendZB FE0C240001010001000600000F03010022                                                                                                                                                                                               
readZB FE0164000065  rc=6
loopbackframe ( won't work without register the endpoint )
sendZB FE0D240100000101000600000F0301020322
readZB FE0164010064FE034480000100C6FE174481000000060000010100FF0000000000000301020300000028  rc=42


https://github.com/Nerivec/zigbee-on-host/blob/main/src/zigbee/zigbee-nwkgp.ts

*/
/* what means intreview
discovers active endpoints   2505
Discovers device descriptors (profile, device ID) 2504
Discovers input/output clusters 2502
Reads basic attributes (manufacturer, model, version) 2401
Optionally binds clusters and sets up reporting

example device with address 92e6
active endpoint request 2505E692E692
simple descripter 2504E692E69201 (assume endpoind 01)
mode descripter    2502E692E692
power descriptor   2503E682E692 optional but z2m does it
basic cluster attributes

clusters
on/off 0021
temperature 0402
*/