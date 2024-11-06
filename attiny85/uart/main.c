#define F_CPU	8000000UL // 8 MHz clock speed 
#include <avr/io.h>       // Defines macros for register addresses
#include <util/delay.h>    // Defines _delay_ms() for delay

/*
simple UART from scratch to have 
some sort of an output directly connected to the attiny85

//https://developer.electricimp.com/sites/default/files/attachments/images/uart/uart3.png
*/


#define BAUD_RATE		9600
#define BIT_DELAY		(1000000 / BAUD_RATE)

//pins
#define RX			PB1
#define TX			PB0

//helper funcs
#define SET_OUTPUT(pin)			DDRB |= (1 << pin)
#define SET_INPUT(pin)			DDRB &= ~(1 << pin)

#define SET_HIGH(pin)			PORTB |= (1 << pin)
#define SET_LOW(pin)			PORTB &= ~(1 << pin)


void initUART();
void sendByte(uint8_t byte);
void uartDelay();
void send(char* string);

int main() {

	initUART();
	while(1){
		
		//sendByte('A');
		send("Hello world from the ATTINY85 via UART!!!\r\n");
		_delay_ms(100);
	}
	
    
	return 0;
}

void initUART(){
	SET_OUTPUT(TX);
	SET_HIGH(TX); // inital state set HIGH 

}
//					 v-- place
//01000001  &  (1 << i)
void sendByte(uint8_t byte){
	SET_LOW(TX); // set tx low to initiate transmition
	uartDelay();
	for(int i = 0; i < 8; i++){ // transmit each bit 
		if(byte & (1 << i)){ 
			SET_HIGH(TX); 
		}
		else{
			SET_LOW(TX);
		}
		uartDelay();
	}
	SET_HIGH(TX); // set tx high to stop transmition
	uartDelay();
	
	
}
void send(char* string){
	int i = 0;

	while(string[i] != '\0'){
		sendByte(string[i]);
		i++;
	}
}
void uartDelay(){
	_delay_us(BIT_DELAY);
}
