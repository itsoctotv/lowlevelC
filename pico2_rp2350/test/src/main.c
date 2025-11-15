#include "pico/stdlib.h"
#include <stdio.h>

uint8_t leds[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int main(){

	stdio_init_all();

	for(int i = 0; i < sizeof(leds); i++){
		gpio_init(leds[i]);
		gpio_set_dir(leds[i],GPIO_OUT);

	}
	
	while(1){
		
		for(int i = 0; i < sizeof(leds); i++){
			gpio_put(leds[i], 1);
			sleep_ms(50);
			gpio_put(leds[i], 0);
		}
		for(int i = sizeof(leds); i > 0; i--){
			gpio_put(leds[i], 1);
			sleep_ms(50);
			gpio_put(leds[i], 0);
		}


	}
	return 0;
}
