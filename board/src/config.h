

// ----------------------------------------------------------------------------
// SPI PROGRAMMING PINS FOR NRF24LU1+


// ----------------------------------------------------------------------------
// PINS CHIPKIT MAX32
#ifdef BOARD_MAX32

#define PIN_PROG     77
#define PIN_RESET    76
#define PIN_CS       75

#endif

// ----------------------------------------------------------------------------
// PINS TEENSY LC
#ifdef BOARD_TEENSYLC

#define PIN_PROG      9
#define PIN_RESET     8
#define PIN_CS       10  // TOCHK: automatic control via SPI?

#endif

// ----------------------------------------------------------------------------
// SERIAL
#define SER_BAUD     115200
