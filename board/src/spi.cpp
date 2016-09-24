
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
//*** spiWrite2()
//***
//*************************************************************************************************
int spiWrite2( uint16_t len1, uint8_t *buf1, uint16_t len2, uint8_t *buf2 )
{
	uint16_t i;
	
	digitalWrite( PIN_CS, LOW );
	
	for( i = 0; i < len1; i++ )
		SPI.transfer( *buf1++ );

	for( i = 0; i < len2; i++ )
		SPI.transfer( *buf2++ );

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
//*** I somehow begin to dislike this brilliant idea of doing READ and WRITE
//*** with the same buffer :-)
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


//*************************************************************************************************
//*** spiWriteRead2()
//***
//*************************************************************************************************
int spiWriteRead2( uint16_t wrLen, uint8_t *wrBuf, uint16_t rdLen, uint8_t *rdBuf )
{
	digitalWrite( PIN_CS, LOW );
	
	for( ; wrLen; wrLen-- )
		SPI.transfer( *wrBuf++ );		

	for( ; rdLen; rdLen-- )
		*rdBuf++ = SPI.transfer( 0xff );		
	

	digitalWrite( PIN_CS, HIGH );

	return 0;
}


