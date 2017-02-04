/*
 * BPM Generator
 * HV. 1.1
 * 
 * bpm_gen_timer1.h
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

#include <avr/pgmspace.h>
//#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>


// Timer1
// bpm counter delay
// from bpm 77 to bpm 190
// array from 0 to 113
// 114 x 2 = 228 bytes
#define bpm_min 77
//#define bpm_max 179
#define bpm_max 190

#define OUTPUT_PORT_1 PORTD
#define OUTPUT_PORT_2 PORTB


#define BINARY_1_BIT 2			// OUTPUT_PORT_1 (quarter note)
#define BINARY_2_BIT 3			// OUTPUT_PORT_1 (quarter note / 2)
#define BINARY_4_BIT 4			// OUTPUT_PORT_1 (quarter note / 4)
#define BINARY_8_BIT 5			// OUTPUT_PORT_1 (quarter note / 8)

#define TERNARY_3_BIT 6			// OUTPUT_PORT_1 (quarter note / 3)
#define TERNARY_6_BIT 7			// OUTPUT_PORT_1 (quarter note / 6)
#define TERNARY_12_BIT 0		// OUTPUT_PORT_2 (quarter note / 12)




//PROGMEM const prog_uint16_t bpm_display_digits[] = {


/*
 * bpm_timer1_delay[] contains Timer1 Compare Match
 * higher value -> lower frequency
 * 
 * 32467 = 77 bpm
 */

PROGMEM const uint16_t bpm_timer1_delay[] = {
  32467,
  32051,
  31646,
  31250,
  30864,
  30488,
  30120,
  29762,
  29412,
  29070,
  28736,
  28409,
  28090,
  27778,
  27472,
  27174,
  26882,
  26596,
  26316,
  26042,
  25773,
  25510,
  25252,
  25000,
  24752,
  24510,
  24272,
  24038,
  23809,
  23585,
  23364,
  23148,
  22936,
  22727,
  22522,
  22321,
  22124,
  21930,
  21739,
  21552,
  21367,
  21186,
  21008,
  20833,
  20661,
  20492,
  20325,
  20161,
  20000,
  19841,
  19685,
  19531,
  19380,
  19231,
  19084,
  18939,
  18797,
  18657,
  18518,
  18382,
  18248,
  18116,
  17986,
  17857,
  17730,
  17606,
  17482,
  17361,
  17241,
  17123,
  17007,
  16892,
  16778,
  16667,
  16556,
  16447,
  16340,
  16234,
  16129,
  16026,
  15924,
  15823,
  15723,
  15625,
  15528,
  15432,
  15337,
  15244,
  15151,
  15060,
  14970,
  14881,
  14793,
  14706,
  14620,
  14535,
  14451,
  14368,
  14286,
  14204,
  14124,
  14045,
  13966,
  13889,
  13812,
  13736,
  13661,
  13587,
  13513,
  13441,
  13369,
  13298,
  13227,
  13158
};

/*
 * PPQN : 24 Pulses Per Quarter Note
 * Counter incremented on each Timer1 Compare Match in ISR
 * From 0 to 23
 */
uint8_t ppqn_counter = 0;






void change_bpm(uint8_t bpm);
void setup_timer1(void);



/*
 * setup_timer1
 * prepare the 16 bit timer1 for Compare-match interrupt,
 * Clear timer on compare match (CTC) mode
 * with OC1A pin ticking on each Compare Match
 * 
 */

void setup_timer1()
{
  TCCR1A = (1 << COM1A0);   // toggle on OC1A pin
  TCCR1B = (1 << WGM12);    // CTC
  TCCR1B |= (1 << CS11);    // prescaler 8
//  OCR1A = bpm_timer1_delay[0] >> 1;  // test ok 30.8 Hz
//  OCR1A = bpm_timer1_delay[100] >> 1;  // test NOK index foireux ???
//  OCR1A = 13158;                       // test ok 76.08 Hz
//  OCR1A = 15625;                       // test ok 64.04 Hz
//  OCR1A = 20000;                         // test ok 50.05 Hz
//  OCR1A = pgm_read_word_near(bpm_timer1_delay + 0);  // test ok 30.83
//  OCR1A = pgm_read_word_near(bpm_timer1_delay + 113);  // test ok 76.08
//  OCR1A = pgm_read_word_near(bpm_timer1_delay + 70);  // test ok 58.87

  // Enable Compare-match register A interrupt for timer1
  TIMSK1 = (1 << OCIE1A);

  // enable global interrupts
  sei();


//  change_bpm(77);

	// nécessaire : ?
//  pinMode(9, OUTPUT);       // OC1A pin output

  
//  Serial.begin(9600);           // start serial for debug
//  Serial.println(OCR1A);
//  Serial.println(bpm_timer1_delay[100] >> 1);

  //~ for (uint8_t i = 0; i < 114; i++)
  //~ {
    //Serial.print(i);
    //Serial.print(" ");
    //Serial.println(bpm_timer1_delay[i]);  // montre qu'un certain nombre de valeurs du tableau sont fausses OU que le tableau n'a pas la bonne taille
    //uint16_t counter_top = pgm_read_word_near(bpm_timer1_delay + i);
    //Serial.println(counter_top);  // montre qu'un certain nombre de valeurs du tableau sont fausses OU que le tableau n'a pas la bonne taille
  //~ }
}

/*
 * constrain value between low and high
 */
/*
void constrain(value, low, high)
{
	if (value < low)
	{
		value = low;
	}
	if (value > high)
	{
		value = high;
	}
}
* */
/*
 * change_bpm
 * Load Timer1 Output Compare Register 1A OCR1A with correponding
 * bpm_timer1_delay, based on bpm value.
 */
void change_bpm(uint8_t bpm)
{
  //~ constrain(bpm, bpm_min, bpm_max);
	if (bpm < bpm_min)
	{
		bpm = bpm_min;
	}
	if (bpm > bpm_max)
	{
		bpm = bpm_max;
	}
	
  uint8_t index = bpm - bpm_min;	// find array index
  OCR1A = pgm_read_word_near(bpm_timer1_delay + index);
}

/*
 * ISR (TIMER1_COMPA_vect)
 * Interrupt Service Routine for Timer1 Compare match
 * 
 * Extracts steps from ppqn_counter and push results on PORTD
 */
ISR (TIMER1_COMPA_vect)
{
  PORTB ^= (1 << 4);  // toggle pin 13

  uint8_t binary_1_out = 0;  // noire
  uint8_t binary_2_out = 0;  // croche
  uint8_t binary_4_out = 0;  // double croche
  uint8_t binary_8_out = 0;  // triple croche

  uint8_t ternary_3_out = 0;  // noire / 3
  uint8_t ternary_6_out = 0;  // noire / 6
  uint8_t ternary_12_out = 0;  // noire / 12

	/*
	 * conditions for each output :
	 * 
	 * BINARY_1 (quarter note) active once per 24 PPQN (step 0)
	 * BINARY_2 (quarter note /2) active twice per 24 PPQN (steps 0 and 12)
	 * BINARY_4 (quarter note /4) active four times per 24 PPQN (steps 0, 6, 12 and 18)
	 * BINARY_8 (quarter note /8) active eight times per 24 PPQN (steps 0, 3, 6, 9, 12, 15, 18 and 21)
	 */
	
  binary_1_out = (ppqn_counter == 0);
  binary_2_out = (ppqn_counter == 0) || (ppqn_counter == 12);
  binary_4_out = (ppqn_counter == 0) || (ppqn_counter == 6) || (ppqn_counter == 12) || (ppqn_counter == 18);
  binary_8_out = (ppqn_counter == 0) || (ppqn_counter == 3) || (ppqn_counter == 6) || (ppqn_counter == 9) || (ppqn_counter == 12) || (ppqn_counter == 15) || (ppqn_counter == 18) || (ppqn_counter == 21);

  ternary_3_out = (ppqn_counter == 0) || (ppqn_counter == 8) || (ppqn_counter == 16);
  ternary_6_out = (ppqn_counter == 0) || (ppqn_counter == 4) || (ppqn_counter == 8) || (ppqn_counter == 12) || (ppqn_counter == 16) || (ppqn_counter == 20);
  ternary_12_out = ~(ppqn_counter & 0b00000001);


//  PORTD = (ppqn_counter << 2) & 0b01111100;
  OUTPUT_PORT_1 ^= ((binary_1_out << BINARY_1_BIT) | (binary_2_out << BINARY_2_BIT) | (binary_4_out << BINARY_4_BIT) | (binary_8_out << BINARY_8_BIT) | (ternary_3_out << TERNARY_3_BIT) | (ternary_6_out << TERNARY_6_BIT)) & 0b11111100;
  OUTPUT_PORT_2 ^= ternary_12_out & 0b00000001;		// TERNARY_12_BIT


  ppqn_counter++;
  if (ppqn_counter > 23)
  {
    ppqn_counter = 0;
  }

//	PORTC ^= (1 << PC3);
}


