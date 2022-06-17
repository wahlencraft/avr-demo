# AVR demo
In this project I test low-level functionality of an AVR-microcontroller
(atmega328p). Everything is written from scratch in C and Assembly.
Functionality includes:
- Toggle LED
- Busy wait
- Sleep
- Measure time
- Print over USART

## Read prints
The `printf` function is implemented to use USART as out-stream. However to
read the print it needs to be connected to a separate computer with some
wiring.

### Raspberry Pi

The TX (3) pin on atmega328p should be connected to the RX (10) pin on the
Raspberry Pi. However the voltage needs to be dropped from 5 V to 3.3 V. That
can be done with a simple voltage splitter using 2 resistors (2 kΩ + 1 kΩ
works).

Also connect a ground cable.

Then using the program minicom the prints can be read with the command:
```
minicom -b 9600 -o -D /dev/serial0
```

## Debugging (gdb simulation)

Make sure `avr-gdb` and `simavr` is installed.

Replace `-Os` with `-Og` in Makefile `COMPILE` variable.
```
make
```

Start simavr
```
simavr -m atmega328p -f 1000000 main.elf -g
```
Connect with gdb
```
avr-gdb
file main.elf
target remote :1234
load
```

