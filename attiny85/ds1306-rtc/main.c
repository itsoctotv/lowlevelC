#define F_CPU			8000000UL
#include <avr/io.h>
#include <util/delay.h>



//uart

#define BAUD_RATE		9600
#define BIT_DELAY		(1000000 / BAUD_RATE)

//pins

#define TX			PB4

//helper funcs
#define SET_OUTPUT(pin)			DDRB |= (1 << pin)
#define SET_INPUT(pin)			DDRB &= ~(1 << pin)

void initUART();
void sendByte(uint8_t byte);
void uartDelay();
void send(char* string);




//rtc to attiny85 connections
#define SCLK_PIN			PB2
#define IO_PIN				PB1
#define CE_PIN				PB0
#define SET_HIGH(x)			(PORTB |= (1 << x))
#define SET_LOW(x)			(PORTB &= ~(1 << x))

#define LED_PIN				PB3
#define SET_OUTPUT(pin)			DDRB |= (1 << pin)
#define SET_INPUT(pin)			DDRB &= ~(1 << pin)

void initPins();
void writeByte(uint8_t byte);
uint8_t readByte();
uint8_t readReg(uint8_t reg);

void toggleLED() {
    PORTB ^= (1 << LED_PIN); // Toggle LED state
}

int main() {
    initPins();
    initUART();

    uint8_t last_seconds = readReg(0x81); // Mask out CH bit for initial read
    while (1) {
        uint8_t current_seconds = readReg(0x81); // Read current seconds, mask CH

        
		sendByte(current_seconds);
		send("\r\n");
        _delay_ms(100); // Small delay to reduce read frequency
    }

    return 0;
}

void initPins(){
	//set clk and ce pins to output
	DDRB |= (1 << SCLK_PIN);
	DDRB |= (1 << CE_PIN);
	DDRB &= ~(1 << IO_PIN);// set io as input

	SET_LOW(CE_PIN);
	SET_LOW(SCLK_PIN);

	DDRB |= (1 << LED_PIN);
	SET_LOW(LED_PIN);
}

/* IO pin to output for writing
check bitwise if bit from byte is high -> set IO_PIN high else -> set low
-> give a clock pulse and shift to the next bit
*/
void writeByte(uint8_t byte){
	DDRB |= (1 << IO_PIN); // set io to output for writing to rtc
	for(uint8_t i = 0; i < 8; i++){
		if(byte & 0b00000001){
			SET_HIGH(IO_PIN);
		}
		else{
			SET_LOW(IO_PIN);
		}

		//clock pulse
		SET_HIGH(SCLK_PIN);
		_delay_us(1);
		SET_LOW(SCLK_PIN);
		_delay_us(1);
		byte >>= 1;
		
	}

}

uint8_t readByte(){
	uint8_t byte = 0x00;
	DDRB &= ~(1 << IO_PIN); // set io to input for reading from rtc
	//PINB -> read current state of pins that are on PORTB (all 5 on an attiny85)

	for(uint8_t i = 0; i < 8; i++){
		if(PINB & (1 << IO_PIN)){ // create a mask to read the IO_PIN  
			byte |= (1 << i); //read bit and write the information to a byte;
		}
	
		//clock pulse
		SET_HIGH(SCLK_PIN);
		_delay_us(1);
		SET_LOW(SCLK_PIN);
		_delay_us(1);
	}

	return byte;
	
}


uint8_t readReg(uint8_t reg){
	SET_HIGH(CE_PIN); //set ce pin high to enable communication
	writeByte(reg); // write instructions hexcode to the register
	uint8_t val = readByte(); // read from it on the next clock cycle
	SET_LOW(CE_PIN);
	return val;
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
