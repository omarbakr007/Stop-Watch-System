/*
 * Interfacing_Project.c
 *
 *  Created on: 12 Jan 2022
 *  Author: Omar Bakr
 *  Stop Watch Project,
 */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*unsigned char sec1 = 0;
unsigned char sec2 = 0;


unsigned char min1 = 0;
unsigned char min2 = 0;


unsigned char hour1 = 0;
unsigned char hour2 = 0;*/

unsigned char sec = 0;
unsigned char min = 0;
unsigned char hour = 0;

ISR(INT0_vect) //Stop Watch time should be reset.
{
	sec = 0;
	min = 0;
	hour = 0;
}

ISR(INT1_vect) //Stop Watch time should be paused.
{
	 TCCR1B &= ~(1<<CS10) & ~(1<<CS11) &  ~(1<<CS12);

}

ISR(INT2_vect) //Stop Watch time should be resumed.
{
	TCCR1B |= (1<<CS10) | (1<<CS12);


}




ISR(TIMER1_COMPA_vect)
{
	sec++;
	if (sec == 60)
	{
		sec = 0;
		min++;
	}
	if (min == 60)
	{
		min = 0;
		hour++;
	}
	if (hour == 99)
	{
		hour = 0;
		sec = 0;
		min = 0;
	}
}

/* External Interrupt INT0 with falling edge
 * Stop Watch time should be reset.
 *
 * */
void INT0_init_reset(void)
{
	DDRD &= ~(1<<PD2);  // Configure INT0/PD2 as input pin
	PORTD |=(1<<PD2);  // Enable the internal pull up resistor at PD2 pin
	GICR |= (1<<INT0);   // Enable external interrupt pin INT0
	MCUCR |= (1<<ISC01); // INT0 falling edge
	SREG |= (1<<7);     // Enable interrupts by setting I-bit
}

/* External Interrupt INT1 with raising edge
 * Stop Watch time should be paused.
 * */
void INT1_init_pause(void)
{
	DDRD &= ~(1<<PD3);  // Configure INT1/PD3 as input pin

	GICR |= (1<<INT1);   // Enable external interrupt pin INT1
	MCUCR |= (1<<ISC11) | (1<<ISC11); // INT1 raising edge
	SREG |= (1<<7);     // Enable interrupts by setting I-bit

}

/* External Interrupt INT2 with falling edge
 * Stop Watch time should be resumed.
 * */
void INT2_init_resume(void)
{
	DDRD &= ~(1<<PB2);  // Configure INT2/PB2 as input pin
	PORTD |=(1<<PB2);  // Enable the internal pull up resistor at PB2 pin
	GICR |= (1<<INT2);   // Enable external interrupt pin INT2
	MCUCSR |= (1<<ISC2); // INT2 falling edge
	SREG |= (1<<7);     // Enable interrupts by setting I-bit
}


void TIMER1_init(void)
{
	TCNT1=0;
	OCR1A = 1000;

	TCCR1A = (1<<FOC1A) ;
	TIMSK |= (1<<OCIE1A);
	TCCR1B = (1<<WGM12) | (1<<CS10) | (1<<CS12);

}


int main(void)

{
	DDRA = 0b11111111;
	PORTA = 0b00111111;
	DDRC |= 0b00001111;
	SREG |= (1 << 7);
	INT0_init_reset();
	INT1_init_pause();
	INT2_init_resume();

	TIMER1_init();

	while (1)
	{

		PORTA = (1 << PA0); // (PORTA = 0b00000001)
		PORTC = sec % 10;
		_delay_ms(5);

		PORTA = (1 << PA1);
		PORTC = sec / 10;
		_delay_ms(5);

		PORTA = (1 << PA2);
		PORTC = min % 10;
		_delay_ms(5);

		PORTA = (1 << PA3);
		PORTC = min / 10;
		_delay_ms(5);

		PORTA = (1 << PA4);
		PORTC = hour % 10;
		_delay_ms(5);

		PORTA = (1 << PA5);
		PORTC = hour / 10;
		_delay_ms(5);

	}

}





// another solution

/*
sec1++;
if (sec1 == 10 )
{
	sec1=0;
	sec2++;
}

if(sec2 == 6)
{
	sec1=0;
	sec2=0;
	min1++;
}


 if ((min1 == 9) & (sec2 == 5) & (sec1 == 9))
{
	min1=0;
	min2++;
}



 if(min2 == 6)
{
	min1=0;
	min2=0;
	hour1++;
}



 if ((hour1 == 9) & (min2 == 6) )
{
	min1=0;
	min2=0;
	hour2++;
}



 if((hour2 == 9) & (hour1 == 9) )
{
	 sec1 = 0;
	 	 sec2 = 0;



}
*/






