## compile
- avr-gcc -mmcu=attiny85 -Os -o main.elf main.c

## convert from .elf to .hex
- avr-objcopy -O ihex main.elf main.hex

## flash to board
- avrdude -c avrisp -P /dev/ttyUSB0 -b 19200 -p t85 -U flash:w:main.hex:i
