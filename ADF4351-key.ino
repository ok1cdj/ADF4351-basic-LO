/* Very simple software for PLL ADF4351 and ATTiny13
  can be compiled with arduino and microcore
  https://github.com/MCUdude/MicroCore
  v 0.1 Ondra OK1CDJ 5/2020 ondra@ok1cdj.com

  Please check the ADF4351 datasheet or use ADF4351 software
  to get PLL register values.

  PIN layout
  PB0 key input - keyed low
  PB4 ADF4531 clock (CLK)
  PB3 ADF4531 data (DATA)
  PB2 ADF4531 latch enable (LE)
*/





// PLL registers
long int r0, r1, r2, r3, r4, r5;
#define KEY 0
int val = 0;

////////////////////////////////////////////////////////
void write2PLL(uint32_t PLLword) {         // clocks 32 bits word  directly to the ADF4351
  // msb (b31) first, lsb (b0) last
  for (byte i = 32; i > 0; i--) { // PLL word 32 bits
    (PLLword & 0x80000000 ? PORTB |= 0b00001001 : PORTB &= 0b11110111);  // data on PB3
    PORTB |= 0b00010001;                   // clock in bit on rising edge of CLK (PB4 = 1)
    PORTB &= 0b11101111;                   // CLK (PB4 = 0)
    PLLword <<= 1;                         // rotate left for next bit
  }
  PORTB |= 0b00000101;                   // latch in PLL word on rising edge of LE (PB2 = 1)
  PORTB &= 0b11111011;  // LE (PB2 = 0)
  delayMicroseconds(1);
}

void setup () {

  DDRB  = 0xfe; // PB are all outputs PB0 input
  PORTB = 0x01; // make PB low and PB0 high

  // Fout= 2450.050 only from main port

  r0 = 0x300008;
  r1 = 0x8008FA1;


  r2 = 0x4E42;
  r3 = 0x4B3;
  r4 = 0x8C803C;
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
  val = digitalRead(KEY);
  // TX ON/OFF can be used for CW keying
  if (val == 1)
    write2PLL(r4 & 0xffffffdf); //txoff
  else
    write2PLL(r4); //txon


}




