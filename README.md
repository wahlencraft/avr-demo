# AVR demo
In this project I test low-level functionality of an AVR-microcontroller
(atmega328p). Everything is written from scratch in C and Assembly.
Functionality includes:
- Toggle LED
- Busy wait
- Sleep
- Measure time
- Print over USART

## Setup

### Hardware
- ATMEGA328P (No Arduino boatload needed)
- Breadboard (or something else to attach things too)
- LED
- Resistors
- Wires
- AVR programmer (I'm using [AVR Pocket programmer](https://www.sparkfun.com), others
  should work too).

### Programmer
Connect the programmer. Note that the AVR Pocket programmer has a built-in 5V
supply. Other programmers might need to connect that separately.

| Programmer | AVR |
|------------|-----|
| `MOSI` | `17` (`MISO`) |
| `MISO` | `18` (`MISO`) |
| `RST`  | `1` (`RESET`) |
| `SCK`  | `19` (`SCK`)  |
| `5V`   | `7` & `20` |
| `GND`  | `8` |

### LED
Connect the LED in series with a resistor from pin `23` (`PC0`) to ground.

### Read prints
The `printf` function is implemented to use USART as out-stream. However to
read the print it needs to be connected to a separate computer with some
wiring.

#### Raspberry Pi

The `TX` (`3`) pin on atmega328p should be connected to the `RX` (`10`) pin on the
Raspberry Pi. However the voltage needs to be dropped from 5 V to 3.3 V. That
can be done with a simple voltage slitter using 2 resistors.

```
                RX (3.3 V)
                 |
GND ---/\/\/\/---|---/\/\/\/--- TX (5 V)
        2 kΩ          1 kΩ
```

Also connect the two grounds.

Then from the Raspberry Pi using the program minicom the prints can be read
with the command: 
```
minicom -b 9600 -o -D /dev/serial0
```

### Installation and programming (Linux)

Install avrdude, avr-libc, avr-gcc and avr-binutils.

Clone this repository.

Verify good connections:
```
make verify
```

Set fuses (internal clock 8 MHz):
```
make fuse
```
This only has to be done once.

Build program and flash to processor:
```
make flash
```

## Debugging (gdb simulation)

Install `avr-gdb` and `simavr`.

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

