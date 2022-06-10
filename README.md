to read UART on Raspberry Pi
```
minicom -b 9600 -o -D /dev/serial0
```

# Debuging (gdb simulation)

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

