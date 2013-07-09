/*
 * adcLight.c
 *
 * Created: 11/16/2012 18:53:06
 *  Author: Andre Lucas
 
 Read voltage on ADC6 Toggle light on PD1
 
 */ 


#define F_CPU 2000000  //Define the clock rate of the uC, which is needed by some dependencies like delay.h.

#include <avr/io.h>		//Include Input Output pins library e.g. DDRB and PORTB and PINB registers.
#include <util/delay.h>	//Delay functions "_delay_ms(miliseconds)" and "_delay_(microseconds)" which puts the IC into an idle calculation for that specific amount of time. (It is basically a for loop counting nothing relevant).


void ioinit(void);

int main(void){
	char analog = 0; //Variable that will store the value read by the ADC. An 8 bit value that will indicate voltage on ADC6.
		
	ioinit();
	
    while(1){
		_delay_ms(500);
		
		PORTD &= ~(1<<1); //Set PD1 low. Remember, we are "and"ing whatever PORTD is to 0b11111101 which leaves every bit the same except for PD1!
		
		analog=ADCH; //Captures the analog reading and writes it to our variable analog. This is a destructive process for ADCH. Once it is read, its value gets cleared.
		
		if (analog < 100){ // The value 100 is the threshold for what level of the voltage we want to start toggling the pin. In our case it corresponds to ~1.9V (5V=255, 0V=0. 8-bit!);
			PORTD ^= (1<<1); //This is an exclusive OR operation. So it toggles the pin ON/OFF.
			
		}
    }
}

void ioinit(void){
	DDRD = 1<<1; //Set PD1 to Output
	PORTD = 1<<1; //Set PD1 high (Turn on LED)
	
	//You might want to check the values for the ADC in the ATmega datasheet. The ADC chapter explains the behavior of the ADC and at the end of the chapter there is a description for all the register values below.
	ADMUX |= (1<<REFS0); // Voltage Reference Selection: AVcc with External capacitor at Vin (This is how the Baby Orangutan board was built).
	ADMUX |= 1<<ADLAR; // Left Adjust the result so that we can read only one register to get the data. (We are simply reading 8bits off of the 10 bit ADC and losing some precision)
	ADMUX |= (1<<MUX1) | (1<<MUX2); //Select Channel for the ADC: ADC6.
	
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Select clock for the ADC: Main clock/1024 (Approx 20KHz)
	ADCSRA |= (1<<ADATE)|(1<<ADEN); //Select Auto Trigger mode: Free Running mode (Do conversions non-stop and update ADCH every ADC clock cycle) and Enable the ADC  
	
	ADCSRA |= 1<<ADSC; //Start Conversion: Starts the first conversion and keep converting because of the free running mode.
	
};