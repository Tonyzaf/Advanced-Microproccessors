#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define RAND_MAX 100		//Ορισμός της μέγιστης τιμής της Random για τον αισθητήρα στο 100
#define ped 585937500		//Ορισμός χρόνου αναμονής στα 30 Δευτερόλεπτα

void BIG_GREEN(){
	PORTD.OUTSET = PIN0_bm; //Θέτουμε το μεγάλο φανάρι(Pin 0) στο λογικό 1 για να ανάψει πράσινο
	PORTD.OUTCLR = PIN1_bm | PIN2_bm; //Θέτουμε το φανάρι των πεζών(Pin 1) και το μικρό φανάρι(Pin 2) στο λογικό 0 για να ανάψουν κόκκινα
} 

void BIG_RED(){
	PORTD.OUTCLR = PIN0_bm; //Θέτουμε το μεγάλο φανάρι(Pin 0) στο λογικό 0 για να ανάψει κόκκινο
	PORTD.OUTSET = PIN1_bm | PIN2_bm; //Θέτουμε το φανάρι των πεζών(Pin 1) και το μικρό φανάρι(Pin 2) στο λογικό 1 για να ανάψουν πράσινα
}

int sensor(){				//ορίζουμε μία συνάρτηση που υπολογίζει την τυχαία τιμή του αισθητήρα
	srand(time(0));
	int sen = rand();
	return (sen % 10);
}

int main(){
	PORTD.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm ;// Θέτουμε τα Pin 0-2 σαν εξόδους
	PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc; //θέτουμε το Pin 5 σαν είσοδο
	int interr = 0;
	sei(); //enable interrupts
	while(1){
		BIG_GREEN(); //καλούμε τη συνάρτηση για πράσινο φανάρι στο μεγάλο δρόμο	
		int res = sensor();
		if (res == 0 || res == 5 || res == 8){	//ελέγχουμε αν η τιμή απο τον αισθητήρα τελειώνει σε 0,5 ή 8
			interr = 1;
		}
		if (interr == 1){						//ελέγχουμε αν το interrupt εχει ενεργοποιηθεί για να αλλάξουμε τα φανάρια και να ξεκινήσει το timer
			interr = 0;
			BIG_RED();
			TCA0.SINGLE.CNT = 0; //clear counter
			TCA0.SINGLE.CTRLB = 0; //Normal Mode (TCA_SINGLE_WGMODE_NORMAL_gc σελ 207)
			TCA0.SINGLE.CMP0 = ped; //When reaches this value -> interrupt CLOCK FREQUENCY/1024δε
			TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc; //(= 0x7<<1 σελ 224 )
			TCA0.SINGLE.CTRLA |=1;//Enable
			TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm; //Interrupt Enable (=0x10)
		}
	}
	
	ISR(TCA0_CMP0_vect){				//Δημιουργία του timer interrupt
		interr = 1;
		TCA0.SINGLE.CTRLA = 0; //Disable
		//clear flag
		int intflags = TCA0.SINGLE.INTFLAGS;
		TCA0.SINGLE.INTFLAGS=intflags;
	}
	
	ISR(PORTF_PORT_vect){					//Δημιουργία του Interrupt που ελέγχει για το πάτημα του κουμπιού των πεζών
		interr = 1;
		//clear the interrupt flag
		int intflags = PORTF.INTFLAGS;
		PORTF.INTFLAGS=intflags;
	}
}