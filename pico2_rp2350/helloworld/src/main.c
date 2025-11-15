#include "pico/stdlib.h"
#include <stdio.h>

int main(){
	stdio_init_all();
	gpio_init(22);
	gpio_set_dir(22, GPIO_OUT);
	while(1){
		printf("Hello World!\n");
		gpio_put(22,1);
		sleep_ms(500);
		gpio_put(22,0);
		sleep_ms(500);
		
	}
	return 0;
}
