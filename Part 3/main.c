#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Variable Initialization
#define del 5
int  count = 0, interr =0 , y = 0, route = 0;

int main(){
	PORTD.DIR |= PIN1_bm; //PIN1 Straight Movement
	PORTD.DIR |= PIN2_bm; //PIN2 Left Turn
	PORTD.DIR |= PIN3_bm; //PIN3 Right Turn
	
	// Enable Switches For Turning and Reversing
	PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc; // Right Turn
	PORTC.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc; // Reverse Mode
	
	//initialize the ADC for Free-Running mode
	ADC0.CTRLA |= ADC_RESSEL_10BIT_gc; //10-bit resolution
	ADC0.CTRLA |= ADC_FREERUN_bm; //Free-Running mode enabled
	ADC0.CTRLA |= ADC_ENABLE_bm; //Enable ADC
	ADC0.MUXPOS |= ADC_MUXPOS_AIN7_gc; //The bit
	//Enable Debug Mode
	ADC0.DBGCTRL |= ADC_DBGRUN_bm;
	//Window Comparator Mode
	ADC0.WINLT |= 100; //Set threshold
	ADC0.INTCTRL |= ADC_WCMP_bm; //Enable Interrupts for WCM
	ADC0.CTRLE |= ADC_WINCM0_bm; //Interrupt when RESULT < WINLT
	sei();
	ADC0.COMMAND |= ADC_STCONV_bm; //Start Conversion
	
	while (1){
		
		if (y == 0){
			PORTD.OUT= PIN1_bm; //  LED1 On for Straight Movement
			_delay_ms(del);			// Movement Delay
			if(count==8||count==0)
			break;
		}
		else if (y == 1 || count != -1){
			// timer
			interr = 0;
			TCA0.SINGLE.CNT = 0; // Coubter Reset
			TCA0.SINGLE.CTRLB = 0; //Normal Mode
			TCA0.SINGLE.CMP0 = del; // Value Comparison
			TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc; // Clock Frequency Set
			TCA0.SINGLE.CTRLA |=1;// Timer Enable
			TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm; // Interrupt Enable
			while (interr == 0) {	}
			PORTD.OUT= PIN1_bm; //  LED1 On for Straight Movement
		}
		
		
	}
	cli();
}
ISR(ADC0_WCOMP_vect){
	//Turn if Value is Lower
	int intflags = ADC0.INTFLAGS;
		ADC0.INTFLAGS = intflags;
	if (route == 0){     // 
		PORTD.OUT= PIN2_bm; //  LED2 On For Left Turn
				count+=1; // Adjust Corner Counter
	}
	else if(route == 1){
		PORTD.OUT= PIN3_bm; //  LED3 On For Right Turn
		count-=1; // Adjust Corner Counter
	}
	y = 1;
}

ISR(PORTF_PORT_vect){
	//Reverse Switch
	int intflags = PORTF.INTFLAGS;
	PORTF.INTFLAGS=intflags;
	if (route == 0){
		PORTD.OUT= PIN3_bm; //  LED3 on for Right Turn
		count+=1; // Adjust Corner Counter
	}
	else if(route == 1){
		PORTD.OUT= PIN2_bm; //  LED2 on for Left Turn
		count-=1; // Adjust Corner Counter
	}
	y = 1;
}

ISR(PORTC_PORT_vect){
	//180 Degree Turn
	int intflags = PORTC.INTFLAGS;
	PORTC.INTFLAGS=intflags;
	PORTD.OUT= 0x0e; //  All 3 LEDs On
	route = 1;
	y = 1;
	count+=1;
}


ISR(TCA0_CMP0_vect){
	TCA0.SINGLE.CTRLA = 0; //Disable
	//clear flag
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS=intflags;
	y = 0;
	interr=1;
}