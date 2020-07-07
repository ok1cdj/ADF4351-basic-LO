/* CW beacon software for PLL ADF4351 and ATTiny45 
 can be compiled with arduino 
 v 0.1 Ondra OK1CDJ 9/2018 ondra@ok1cdj.com
 
 Parts of this code is based on routines written by PA3FYM

 Morse Based on Simple Arduino Morse Beacon  
 Written by Mark VandeWettering K6HX
 
 Please check the ADF4351 datasheet or use ADF4351 software
 to get PLL register values.
 
 PIN layout
 PB4 ADF4531 clock (CLK)   
 PB3 ADF4531 data (DATA)
 PB2 ADF4531 latch enable (LE)
*/


struct t_mtab {
  char c, pat;
} ;

struct t_mtab morsetab[] = {
  {'.', 106},
  {',', 115},
  {'?', 76},
  {'/', 41},
  {'A', 6},
  {'B', 17},
  {'C', 21},
  {'D', 9},
  {'E', 2},
  {'F', 20},
  {'G', 11},
  {'H', 16},
  {'I', 4},
  {'J', 30},
  {'K', 13},
  {'L', 18},
  {'M', 7},
  {'N', 5},
  {'O', 15},
  {'P', 22},
  {'Q', 27},
  {'R', 10},
  {'S', 8},
  {'T', 3},
  {'U', 12},
  {'V', 24},
  {'W', 14},
  {'X', 25},
  {'Y', 29},
  {'Z', 19},
  {'1', 62},
  {'2', 60},
  {'3', 56},
  {'4', 48},
  {'5', 32},
  {'6', 33},
  {'7', 35},
  {'8', 39},
  {'9', 47},
  {'0', 63}
} ;

#define N_MORSE  (sizeof(morsetab)/sizeof(morsetab[0]))

int dotlen;
int dashlen;
int speed = 20;

char *str;


// Morse code tokens. 0 = dit, 1 = dah any other character will generate 8 dit error.

// NOTE: These need to be in ASCII code sequence to work

char tokens [][7] = { "110011", "100001", // ,,-
                      "010101", "10010", // .,/
                      "11111", "01111", "00111", "00011", "00001", "00000", // 0,1,2,3,4,5
                      "10000", "11000", "11100", "11110", // 6,7,8,9
                      "111000", "10101", "<", // :,;,<"=",">","001100", // =,>,?
                      "@", // @,
                      "01", "1000", "1010", // A,B,C
                      "100", "0", "0010", // D,E,F
                      "110", "0000", "00", // G,H,I
                      "0111", "101", "0100", // J,K,L
                      "11", "10", "111", // M,N,O
                      "0110", "1101", "010", // P,Q,R
                      "000", "1", "001", // S,T,U
                      "0001", "011", "1001", // V,W,X
                      "1011", "1100"
                    }; // Y,Z }
;

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


// Routine to output a dit
void dit()
{
  write2PLL(r4);
  delay(dotlen);
  write2PLL(r4 & 0xffffffdf);
  delay(dotlen);
}

// Routine to output a dah
void dash()
{
  write2PLL(r4);
  delay(dashlen);
  write2PLL(r4 & 0xffffffdf);
  delay(dotlen);
}


// Look up a character in the tokens array and send it
void
send(char c)
{
  int i ;
  if (c == ' ') {

    delay(7 * dotlen) ;
    return ;
  }
  for (i = 0; i < N_MORSE; i++) {
    if (morsetab[i].c == c) {
      unsigned char p = morsetab[i].pat ;

      while (p != 1) {
        if (p & 1)
          dash() ;
        else
          dit() ;
        p = p / 2 ;
      }
      delay(2 * dotlen) ;
      return ;
    }
  }
 //if we drop off the end, then we send a space

}

void
sendmsg(char *str)
{
  while (*str)
    send(*str++) ;

}


void setup () {   

DDRB  = 0xff; // PB are all outputs 
PORTB = 0x00; // make PB low

// Fout= 432.910 MHz +5dBm  only RF port
 r0 = 0x450A60;  
 r1 = 0x8009389;
 r2 = 0x4E42; 
 r3 = 0x4B3;
 r4 = 0xBC80FC;
 r5 = 0x580005; 

 // write from r5 to r0
     write2PLL(r5); 
     write2PLL(r4); 
     write2PLL(r3);
     write2PLL(r2);
     write2PLL(r1);
     write2PLL(r0);  
// set speed of morse in WPM
     
     speed = 20;

     dotlen = (1200 / speed);
     dashlen = (3 * (1200 / speed));
     str = "OK1CDJ/B JO70VA";
     write2PLL(r4 & 0xffffffdf);        // TX off 
     
}

// main loop 
void loop() {                                 
      delay(800);                         
      sendmsg(str);
      delay(350);                         // pause after beacon text
      write2PLL(r4);                     // then TX on 
      delay(5000);                       // 5 seconds TX
      write2PLL(r4 & 0xffffffdf);        // TX off   

}




