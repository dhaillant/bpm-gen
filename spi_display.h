/*
 * BPM Generator
 * HV. 1.1
 * 
 * spi_display.h
 * 
 * Copyright 2014 david haillant <david_haillant@yahoo.fr>
 * http://dhaillant.free.fr
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */


//~ #include <Arduino.h>



#define DISP_REG1 DDRB
#define DISP_PORT1 PORTB

#define DISP_REG2 DDRC
#define DISP_PORT2 PORTC


#define DATA (1<<PB3)	//MOSI (SI)
#define LATCH (1<<PB2)	//SS   (RCK)
#define CLOCK (1<<PB5)	//SCK  (SCK)

#define DIMM (1<<PC3)	// A3 dimmer


// PORT B :

//Pin connected to ST_CP of 74HC595
//int latchPin = 10;
//#define latchBit 2 
//Pin connected to SH_CP of 74HC595
//int clockPin = 13;
//#define clockBit 5
//Pin connected to DS of 74HC595
//int dataPin = 11;
//#define dataBit 3

// PORT C :

//Pin connected to 
//int enPin = "A3";


// take the latchPin low so 
// the LEDs don't change while you're sending in bits:
//#define disable_display_update digitalWrite(latchPin, LOW)
#define init_transfert DISP_PORT1 &= ~LATCH

//take the latch pin high so the LEDs will light up:
//#define enable_display_update digitalWrite(latchPin, HIGH)
#define end_transfert DISP_PORT1 |= LATCH



/*
 * Digits wiring :
 * 
 *     A
 *   F   B
 *     G
 *   E   C
 *     D
 *         dp
 * 
 *     2
 *   3   1
 *     0
 *   4   6
 *     5
 *         7
 * 
*/

/*
 * Digits array. Each segment is driven by a bit (0 or 1)
 * 0 means segment is ON
 * 1 means segment is OFF
 * 
 * led_digits[0] is character "0"
 * led_digits[9] is character "9"
 * led_digits[10] is character " " ("empty" or "space")
 */

const uint8_t led_digits[] = {
  //dCDEFABG
  //76543210
  0b10000001,  // 0
  0b10111101,  // 1
  0b11001000,  // 2
  0b10011000,  // 3
  0b10110100,  // 4
  0b10010010,  // 5
  //76543210
  0b10000010,  // 6
  0b10111001,  // 7
  0b10000000,  // 8
  0b10010000,  // 9
  0b11111111   // all off
  //dCDEFABG
};



const uint16_t led_numbers[] = {
	0x77,  // 0
	0x78,
	0x79,
	0x80,
	0x81,
	0x82,
	0x83,
	0x84,
	0x85,
	0x86,
	0x87,
	0x88,
	0x89,
	0x90,
	0x91,
	0x92,
	0x93,
	0x94,
	0x95,
	0x96,
	0x97,
	0x98,
	0x99,
	0x100,
	0x101,
	0x102,    // 25
	0x103,
	0x104,
	0x105,
	0x106,
	0x107,
	0x108,
	0x109,
	0x110,
	0x111,
	0x112,
	0x113,
	0x114,
	0x115,
	0x116,
	0x117,
	0x118,
	0x119,
	0x120,
	0x121,
	0x122,
	0x123,
	0x124,
	0x125,
	0x126,
	0x127,
	0x128,
	0x129,
	0x130,
	0x131,
	0x132,
	0x133,
	0x134,
	0x135,
	0x136,
	0x137,
	0x138,
	0x139,
	0x140,
	0x141,
	0x142,
	0x143,
	0x144,
	0x145,
	0x146,
	0x147,
	0x148,
	0x149,
	0x150,
	0x151,
	0x152,
	0x153,
	0x154,
	0x155,
	0x156,
	0x157,
	0x158,
	0x159,
	0x160,
	0x161,
	0x162,
	0x163,
	0x164,
	0x165,
	0x166,
	0x167,
	0x168,
	0x169,
	0x170,
	0x171,
	0x172,
	0x173,
	0x174,
	0x175,
	0x176,
	0x177,
	0x178,
	0x179,
	0x180,
	0x181,
	0x182,
	0x183,
	0x184,
	0x185,
	0x186,
	0x187,
	0x188,
	0x189,
	0x190
};




void display_number(uint16_t numberToDisplay);
void setup_display(void);
int shift_data(uint8_t data);


void setup_display(void)
{
  //set pins to output so you can control the shift register
  //pinMode(latchPin, OUTPUT);
  //pinMode(clockPin, OUTPUT);
  //pinMode(dataPin, OUTPUT);

  //pinMode(pwmPin, OUTPUT);

  	// set Pins Directions for OUTPUT
	// (1 means FORCE OUTPUT, 0 is unchanged state)
//	DDRB |= (1 << latchPin) | (1 << clockPin) | (1 << dataPin);
//	DDRC |= (1 << pwmPin);
	DISP_REG1 |= (DATA | LATCH | CLOCK);		//Set control pins as outputs
	DISP_PORT1 &= ~(DATA | LATCH | CLOCK);		//Set control pins low

	DISP_REG2 |= (DIMM);						//Set DIMM as output
	DISP_PORT2 &= ~(DIMM);						//Set DIMM pin low

	//Setup SPI
	SPCR = (1<<SPE) | (1<<MSTR);  //Start SPI as Master

}

int shift_data(uint8_t data)
{
	SPDR = data;        //This should light alternating LEDs
	//Wait for SPI process to finish
	while(!(SPSR & (1<<SPIF)));

	return 0;
}


/*
 * display_number : use SPI to display numbers on 3 digits 7 segments
 * display
 * ---------------------------------------------------------------------
 * uint16_t numberToDisplay : BCD value (value 0x123 will display "123")
*/
void display_number(uint16_t numberToDisplay)
{
  //uint8_t digit = 0;				// 1 digit
  uint8_t digits[] = {0, 0, 0};		// 3 digits

	// extract digits
  digits[0] = (numberToDisplay >> 8) & 0b00001111;  // MSB
  digits[1] = (numberToDisplay >> 4) & 0b00001111;
  digits[2] = (numberToDisplay >> 0) & 0b00001111;  // LSB

	if (digits[0] == 0)		// if MSB is "0" (eg. "077")
	{
		digits[0] = 10;		// then display nothing instead of "0" (eg. "77")
	}

  init_transfert;

	// shift out the bits from digits:
	for (uint8_t digit = 0; digit < 3; digit++)
	{
		//shiftOut(dataPin, clockPin, MSBFIRST, led_digits[digits[digit]]);  
//    shiftOut(dataPin, clockPin, MSBFIRST, led_digits[digits[1]]); 
		shift_data(led_digits[digits[digit]]);
	}

  end_transfert;
}

