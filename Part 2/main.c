#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define RAND_MAX 100		//������� ��� �������� ����� ��� Random ��� ��� ��������� ��� 100
#define ped 585937500		//������� ������ �������� ��� 30 ������������

void BIG_GREEN(){
	PORTD.OUTSET = PIN0_bm; //������� �� ������ ������(Pin 0) ��� ������ 1 ��� �� ������ �������
	PORTD.OUTCLR = PIN1_bm | PIN2_bm; //������� �� ������ ��� �����(Pin 1) ��� �� ����� ������(Pin 2) ��� ������ 0 ��� �� ������� �������
} 

void BIG_RED(){
	PORTD.OUTCLR = PIN0_bm; //������� �� ������ ������(Pin 0) ��� ������ 0 ��� �� ������ �������
	PORTD.OUTSET = PIN1_bm | PIN2_bm; //������� �� ������ ��� �����(Pin 1) ��� �� ����� ������(Pin 2) ��� ������ 1 ��� �� ������� �������
}

int sensor(){				//�������� ��� ��������� ��� ���������� ��� ������ ���� ��� ���������
	srand(time(0));
	int sen = rand();
	return (sen % 10);
}

int main(){
	PORTD.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm ;// ������� �� Pin 0-2 ��� �������
	PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc; //������� �� Pin 5 ��� ������
	int interr = 0;
	sei(); //enable interrupts
	while(1){
		BIG_GREEN(); //������� �� ��������� ��� ������� ������ ��� ������ �����	
		int res = sensor();
		if (res == 0 || res == 5 || res == 8){	//��������� �� � ���� ��� ��� ��������� ��������� �� 0,5 � 8
			interr = 1;
		}
		if (interr == 1){						//��������� �� �� interrupt ���� ������������� ��� �� ��������� �� ������� ��� �� ��������� �� timer
			interr = 0;
			BIG_RED();
			TCA0.SINGLE.CNT = 0; //clear counter
			TCA0.SINGLE.CTRLB = 0; //Normal Mode (TCA_SINGLE_WGMODE_NORMAL_gc ��� 207)
			TCA0.SINGLE.CMP0 = ped; //When reaches this value -> interrupt CLOCK FREQUENCY/1024��
			TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc; //(= 0x7<<1 ��� 224 )
			TCA0.SINGLE.CTRLA |=1;//Enable
			TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm; //Interrupt Enable (=0x10)
		}
	}
	
	ISR(TCA0_CMP0_vect){				//���������� ��� timer interrupt
		interr = 1;
		TCA0.SINGLE.CTRLA = 0; //Disable
		//clear flag
		int intflags = TCA0.SINGLE.INTFLAGS;
		TCA0.SINGLE.INTFLAGS=intflags;
	}
	
	ISR(PORTF_PORT_vect){					//���������� ��� Interrupt ��� ������� ��� �� ������ ��� �������� ��� �����
		interr = 1;
		//clear the interrupt flag
		int intflags = PORTF.INTFLAGS;
		PORTF.INTFLAGS=intflags;
	}
}