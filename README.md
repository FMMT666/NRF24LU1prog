NRF24LU1prog
============

---
    (RE)WORK IN PROGRESS
    
    NOTHING FUNCTIONAL IN HERE (YET)
    ASkr, 18.09.2016

---
Program bricked, bootloader-less or code protected [NRF24LU1+][1] via SPI.  
Works with 16kB and 32kB versions of the chip.


---
## NEWS

### CHANGES 2016/09/XX:

    - initial upload


---
## REQUIREMENTS

  - a 3.3V Arduino compatible board with SPI functionality (see below)
  - [PlatformIO][2], [CLI][3] or [IDE][4]
  - [Python][5] interpreter
  - a handful of wires


### BOARDS

#### BUILT-IN

  NRF24LU1prog should [tm] run on a lot of 3.3V Arduino-compatible boards.  
  Currently implemented are:

  - [ChipKIT Max32][20]
  - [Teensy LC][21]

#### ADDING YOUR OWN BOARD

  ...


---
## COMPILE & INSTALL

...


---
## HW-SETUP

### CONNECTIONS

  To program an NRF24LU1+ via [SPI][6], at least 8 pins are required:

  - MOSI
  - MISO
  - SCLK
  - CS
  - PROG
  - 3V3
  - GND

  In very rare cases, it might be necessary to additionally wire the RESET pin, e.g.
  when the NRF24 is powered externally and can't be turned on or off.
  
  ...

---
## USAGE

### TERMINAL MODE

#### COMMANDS

  For the ease of it, all commands can be send/read via a serial UART/VCP terminal.  
  Serial settings are "8N1":


  - 115200 bits/s
  - 8 data bits
  - 1 stop bit


  Commands implemented (so far):

    ?      show help
    0      set NRF memory page address (n*512) to zero
    +      increase NRF memory page address; max is 63
    -      decrease NRF memory page address
    S      show flash memory status (in human readable form)
    s      show flash memory status (FSR register value in HEX)
    W      enable  write or erase of flash memory
    w      disable write or erase of flash memory
    r      dump memory content of current NRF memory page in HEX

  ...

### PROGARMMING

  ...

---
## LICENSE

  [CC BY 4.0, Attribution 4.0 International][7]

  You are free to:

  Share — copy and redistribute the material in any medium or format  
  Adapt — remix, transform, and build upon the material for any purpose, even commercially.
  
  The licensor cannot revoke these freedoms as long as you follow the license terms.


---
Have fun  
FMMT666(ASkr)  


[1]: https://www.nordicsemi.com/eng/Products/2.4GHz-RF/nRF24LU1P
[2]: http://platformio.org/
[3]: http://platformio.org/get-started/cli
[4]: http://platformio.org/platformio-ide
[5]: https://www.python.org/
[6]: https://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus
[7]: https://creativecommons.org/licenses/by/4.0/

[9]: https://en.wikipedia.org/wiki/WTFPL

[20]: https://reference.digilentinc.com/chipkit_max32/refmanual
[21]: https://www.pjrc.com/teensy/teensyLC.html
