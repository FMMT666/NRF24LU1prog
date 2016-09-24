#ifndef __asspi
#define __asspi
  // moep, moep...
#endif

#include "SPI.h"

extern int spiWrite     ( uint16_t len, uint8_t *buf );
extern int spiWrite2    ( uint16_t len1, uint8_t *buf1, uint16_t len2, uint8_t *buf2 );
extern int spiRead      ( uint16_t len, uint8_t *buf );
extern int spiWriteRead ( uint16_t len, uint8_t *wrBuf );
extern int spiWriteRead2( uint16_t wrLen, uint8_t *wrBuf, uint16_t rdLen, uint8_t *rdBuf );
