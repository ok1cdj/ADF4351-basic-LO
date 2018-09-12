/* Very simple software for PLL ADF4351 and ATTiny13 
 can be compiled with arduino and microcore
 https://github.com/MCUdude/MicroCore
 v 0.1 Ondra OK1CDJ 9/2018 ondra@ok1cdj.com
 
 Please check the ADF4351 datasheet or use ADF4351 software
 to get PLL register values.
 
 PIN layout
 PB4 ADF4531 clock (CLK)   
 PB3 ADF4531 data (DATA)
 PB2 ADF4531 latch enable (LE)
*/

// PLL registers
long int r0, r1, r2, r3, r4, r5;
 

////////////////////////////////////////////////////////
void write2PLL(uint32_t PLLword) {         // clocks 32 bits word  directly to the ADF4351
                                           // msb (b31) first, lsb (b0) last 
  for (byte i=32; i>0; i--) {     // PLL word 32 bits 
    (PLLword & 0x80000000? PORTB |= 0b00001000 : PORTB &= 0b11110111);   // data on PB3                                                                           
    PORTB |= 0b00010000;                   // clock in bit on rising edge of CLK (PB4 = 1)
    PORTB &= 0b11101111;                   // CLK (PB4 = 0)      
    PLLword <<= 1;                         // rotate left for next bit
    } 
    PORTB |= 0b00000100;                   // latch in PLL word on rising edge of LE (PB2 = 1)
    PORTB &= 0b11111011;  // LE (PB2 = 0)      
    delayMicroseconds(1);
} 

void setup () {   

DDRB  = 0xff; // PB are all outputs 
PORTB = 0x00; // make PB low

// Fout= 1965 MHz +5dBm LO= 2450 MHz - 435 MHz

 r0 = 0x4E8008;  
 r1 = 0x8008029;
 r2 = 0x4E42; 
 r3 = 0x4B3;
 r4 = 0x9C81FC;
 r5 = 0x580005; 

 // write from r5 to r0
     write2PLL(r5); 
     write2PLL(r4); 
     write2PLL(r3);
     write2PLL(r2);
     write2PLL(r1);
     write2PLL(r0);  
     
}

// main loop 
void loop() {                                 

    // TX ON/OFF can be used for CW keying
    //   write2PLL(r4 & 0xffffffdf);

    }





