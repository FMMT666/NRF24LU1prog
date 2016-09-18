
#include "Arduino.h"

#include "config.h"
#include "spi.h"



//*************************************************************************************************
//*** spiWrite()
//***
//*************************************************************************************************
int spiWrite( uint16_t len, uint8_t *buf )
{
	digitalWrite( PIN_CS, LOW );
	
	for( uint16_t i = 0; i < len; i++ )
		SPI.transfer( *buf++ );

	digitalWrite( PIN_CS, HIGH );

	return 0;
}


//*************************************************************************************************
//*** spiRead()
//***
//*************************************************************************************************
int spiRead( uint16_t len, uint8_t *buf )
{
	digitalWrite( PIN_CS, LOW );
	
	for( uint16_t i = 0; i < len; i++ )
		*buf++ = SPI.transfer( 0x00 );

	digitalWrite( PIN_CS, HIGH );

	return 0;
}


//*************************************************************************************************
//*** spiWriteRead()
//***
//*************************************************************************************************
int spiWriteRead( uint16_t len, uint8_t *wrBuf )
{
	uint8_t *rdBuf = wrBuf;
	
	digitalWrite( PIN_CS, LOW );
	
	for( uint16_t i = 0; i < len; i++ )
		*rdBuf++ = SPI.transfer( *wrBuf++ );

	digitalWrite( PIN_CS, HIGH );

	return 0;
}

