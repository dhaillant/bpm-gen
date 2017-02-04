/*
 * BPM Generator
 * HV. 1.1
 * 
 * main.c
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


// ATMEL ATMEGA8 & 168 / ARDUINO
//
//                               +-\/-+
//                         PC6  1|    |28  PC5 (AI 5)
//                   (D 0) PD0  2|    |27  PC4 (AI 4)
//                   (D 1) PD1  3|    |26  PC3 (AI 3)
// BINARY_1	         (D 2) PD2  4|    |25  PC2 (AI 2)
// BINARY_2     PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
// BINARY_4          (D 4) PD4  6|    |23  PC0 (AI 0)
//                         VCC  7|    |22  GND
//                         GND  8|    |21  AREF
//                         PB6  9|    |20  AVCC
//                         PB7 10|    |19  PB5 (D 13)
// BINARY_8     PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// TERNARY_3    PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
// TERNARY_6         (D 7) PD7 13|    |16  PB2 (D 10) PWM  
// TERNARY_12        (D 8) PB0 14|    |15  PB1 (D 9)  PWM
//                               +----+
//
// (PWM+ indicates the additional PWM pins on the ATmega168.)




#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
//#include <Arduino.h>

//#define F_CPU 1000000UL  // 1 MHz
//#define F_CPU 16000000UL  // 16 MHz
#include <util/delay.h>

#include "spi_display.h"
#include "bpm_gen_timer1.h"


#define wait(ms) _delay_ms(ms)
#define DEBOUNCE_TIME_MS 500
#define debounce wait(DEBOUNCE_TIME_MS)
//#define read_startstop_button bla
#define startstop_button_pressed !(startstop_button_PIN & (1 << startstop_button_BIT))

//uint8_t bpm_output_enabled = false;

typedef enum
{
    false = 0,
    true = 1,
} boolean;

boolean bpm_output_enabled;

#define enable_bpm_output setup_timer1()
#define disable_bpm_output TCCR1B = 0
//enum bpm_output_state {start, stop};


void setup(void);
void loop(void);
long map(long, long, long, long, long);
void display_bpm(uint8_t bpm);

static inline void initADC0(void);



int main(void)
{
	setup();
	
	while (1)
	{
		loop();
	}
	
	return 0;
}

#define startstop_button_PORT PORTC
#define startstop_button_PIN  PINC
#define startstop_button_BIT  PC1
#define startstop_button_DDR  DDRC

#define analog_bpm_input_PORT PORTC
#define analog_bpm_input_PIN  PIN
#define analog_bpm_input_BIT  PC0
#define analog_bpm_input_DDR  DDRC

void setup ()
{
	ppqn_counter = 0;	// utile ici ?

	// set Pins Directions for OUTPUT
	// (1 means FORCE OUTPUT, 0 is unchanged state)
	DDRB |= 0b00000001;
	DDRD |= 0b11111100;

	// A FAIRE : déplacer les déclaration DDR dans chaque sous partie respective
	// ajouter/modifier la déclaration DDR pour spi_display depuis pinMode (setup_display)


	// setup of startstop button pin and so on
	startstop_button_DDR &= ~(1 << startstop_button_BIT); // makes double-sure we're in input mode
	startstop_button_PORT |= (1 << startstop_button_BIT); // enables pull-up resistor

	// setup of bpm potentiometer an CV input pin and so on
//	analog_bpm_input_DDR &= ~(1 << analog_bpm_input_BIT); // makes double-sure we're in input mode
//	analog_bpm_input_PORT |= (1 << analog_bpm_input_BIT); // enables pull-up resistor

	initADC0();

//	setup_timer1();
	bpm_output_enabled = false;
	setup_display();

	change_bpm(bpm_max);
	display_bpm(bpm_max);
}


void loop()
{
   //Serial.println(OCR1A);
//   Serial.println(TCNT1);

	/*
		read start/stop button state
		if button pressed,
	 		WAIT 20ms (debounce)
	 		update start/stop status :
	 		if bpm_output_state (start/stop status) is "stop" then
				bpm_output_state = start
	 			enable_bpm_output
	 			(enable Timer1)
	 		else
				bpm_output_state = stop
	 			disable_bpm_output
	 			(disable Timer1)
	 		end if
	 	end if
	*/
	//read_startstop_button;

	if (startstop_button_pressed)
	{
//		display_number(0x111);
		if (bpm_output_enabled == false)
		{
			bpm_output_enabled = true;
			enable_bpm_output;
		}
		else
		{
			bpm_output_enabled = false;
			disable_bpm_output;
		}
		debounce;
	}
	else
	{
		// nothing to do !
//		display_number(0x222);
	}

	ADCSRA |= (1 << ADSC);					// start ADC conversion
	loop_until_bit_is_clear(ADCSRA, ADSC);	// wait until done
	uint16_t pot = ADC;						// read ADC in

	// read from analog input (potentiometer or CV input)
//	uint16_t pot = analogRead(A0);
	uint8_t bpm = map(pot, 0, 1023, bpm_min, bpm_max);
	//sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 255);

	change_bpm(bpm);
	display_bpm(bpm);
	
	PORTC ^= (1 << PC3);
}

// 
static inline void initADC0(void) {
	ADMUX |= (1 << REFS0);					/* reference voltage on AVCC */
//	ADCSRA |= (1 << ADPS1) | (1 << ADPS0);	/* ADC clock prescaler /8 */
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	/* ADC clock prescaler /128 @ 16 MHz = 125 kHz */
	ADCSRA |= (1 << ADEN);					/* enable ADC */
}


long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void display_bpm(uint8_t bpm)
{
	if (bpm < bpm_min) { bpm = bpm_min; }
	if (bpm > bpm_max) { bpm = bpm_max; }

	display_number(led_numbers[bpm - bpm_min]);
//	display_number(led_numbers[0]);
}
