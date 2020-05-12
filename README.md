# ADF4351-basic-LO
Very simple software for PLL ADF4351 and ATTiny13  can be compiled with arduino and microcore
https://github.com/MCUdude/MicroCore

v 0.1 Ondra OK1CDJ 9/2018 ondra@ok1cdj.com
 
ADF4351 eval board from eBay or Ali for aprox. 17 USD. 
 
Please check the ADF4351 datasheet or use ADF4351 software
to get proper PLL register values.

# ADF4351-Beacon
Simple CW beacon. SW is to large for ATTiny13. You can use ATTINY45/85 on same PCB.
Beacon send string in morse code like - OK1CDJ JO70VA and then continous tone for 5s.

# ADF4351-Key
Version with external keying on PB0 of ATTiny13. Can be used as simple CW transmitter.

 
## PIN layout of ATTINY13/45/85
MCU is powered  by 3.3V directly from PLL board.

 pin 5 - PB0 external keying - optional

 pin 3 - PB4  to ADF4531 clock (CLK) 
 
 pin 2 - PB3 to ADF4531 data (DATA) 
 
 pin 7 - PB2 to ADF4531 latch enable (LE) 

![board](https://raw.githubusercontent.com/ok1cdj/ADF4351-basic-LO/master/PLL-board.jpg)

Kit is available here:

https://www.hamshop.cz/pll-adf4351-c51/adf4351-ovladani-s-attiny13-stavebnice-i351/
    
