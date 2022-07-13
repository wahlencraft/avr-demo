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

### Software
Install the packages: avr-gcc, avr-binutils, avr-libc, avrdude

### Hardware
- ATMEGA328P (No Arduino bootloader needed)
- Breadboard (or something else to attach things too)
- LED
- Resistors
- Wires
- AVR programmer (I'm using [AVR Pocket programmer](https://www.sparkfun.com/products/9825), others
  should work too).

### Programmer
Connect the programmer. Note that the AVR Pocket programmer has a built-in 5V
supply. Other programmers might need to connect that separately.

| Programmer | AVR |
|------------|-----|
| `MOSI` | `17` (`MOSI`) |
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

#### Raspberry Pi Pico UART-USB bridge + Linux computer
Download [Pico UART-USB bridge software](https://github.com/Noltari/pico-uart-bridge/releases/tag/v3.0) (uf2 file).

Hold the BOOTSEL button on the Raspberry Pi Pico and insert the USB cable.

Create a mount-point:
```
sudo mkdir -p /mnt/pico
```

Find the device name (`x` in `/dev/sdx`)
```
dmsg | tail
```

mount:
```
sudo mount /dev/sdx1 /mnt/pico
```

copy uf2 file, unmount:
```
sudo cp pico-uart-bridge-v3.0.uf2 /mnt/pico
sudo sync
sudo umount /mnt/pico
```

Now connect the device.

The `TX` (`3`) pin on atmega328p should be connected to the `RX` (`10`) pin on the
Raspberry Pi Pico. However the voltage needs to be dropped from 5 V to 3.3 V. That
can be done with a simple voltage slitter using 2 resistors.

```
                RX (3.3 V)
                 |
GND ---/\/\/\/---|---/\/\/\/--- TX (5 V)
        2 kΩ          1 kΩ
```

Also connect the two grounds.

Then open the terminal with:
```
sudo picocom /dev/ttyACM0 -b 9600 -l
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

