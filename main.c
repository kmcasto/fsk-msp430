#include "msp430g2231.h"

#define dsize 16
int data[1];

/* This two functions stop the main thread for a certain number of milli -or- microseconds.
 They are based on trial and error, but they work fine for the out-of-the-box Launchpad board.
 TI should really add this types of functions as default, just like Arduino does :) .
 */
void delay_ms(unsigned int ms) {
	unsigned int i;
	for (i = 0; i <= ms; i++)
		__delay_cycles(500); //Built-in function that suspends the execution for 500 cicles
}

void delay_us(unsigned int us) {
	unsigned int i;
	for (i = 0; i <= us / 2; i++)
		__delay_cycles(1);
}

//This function generates the square wave that makes the piezo speaker sound at a determinated frequency.
void beep(unsigned int note, unsigned int duration) {
	int i;
	long delay = (long) (10000 / note);  //This is the semiperiod of each note.
	long time = (long) ((duration * 100) / (delay * 2)); //This is how much time we need to spend on the note.
	for (i = 0; i < time; i++) {
		P1OUT |= BIT2;     //Set P1.2...
		delay_us(delay);   //...for a semiperiod...
		P1OUT &= ~BIT2;    //...then reset it...
		delay_us(delay);   //...for the other semiperiod.
	}
	delay_ms(0); //Add a little delay to separate the single notes
}

void setbit(int A[], int k) {
	A[k / 16] |= 1 << (k % 16);  // Set the bit at the k-th position in A[i]
}

void clearbit(int A[], int k) {
	A[k / 16] &= ~(1 << (k % 16));
}

int getbit(int A[], int k) {
	return ((A[k / 16] & (1 << (k % 16))) != 0);
}

// 2 bytes (16 bits) per int, 16 * 4 = 64 bits in serial #
void loadserial() {
	//int i;
	//for (i = 0; i < 1; i++ ) {
	data[0] = 168;
	//}

}


// Beep little speaker, beep
void play() {
	int i;
	for ( i = 0; i < dsize; i++ ) {
		if(getbit(data, i)) {
			beep(100, 10);
		}
		else {
			beep(50, 10);
		}
	}
}

int main(void) {
	WDTCTL = WDTPW + WDTHOLD; //Disable Watchdog Timer
	P1DIR |= BIT2;              // P1.2 output
	loadserial();
	while (1) {
		play();
		delay_ms(10000); //Add a 2 sec. delay to avoid replaying right after the end.
	}
}
