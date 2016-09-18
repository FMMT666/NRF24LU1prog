//
// NRF24LU1prog
//
//
// FMMT666(ASkr)
// www.askrprojects.net
//

#include "Arduino.h"

#include "config.h"
#include "serial.h"
#include "spi.h"
#include "nrf24lu1.h"



// TODO
#define BUFSIZE (520)
uint8_t bufDbg[BUFSIZE];


//*************************************************************************************************
//***
//*************************************************************************************************
void setup()
{
	// Could be nicer, but just let's do all the stuff in here, for now...
	
	// config serial
	Serial.begin( SER_BAUD );

	// config SPI
	SPI.begin();
	SPI.setBitOrder( MSBFIRST );
	SPI.setDataMode( SPI_MODE0 );
	SPI.setClockDivider( SPI_CLOCK_DIV8 ); // TODO (default is _DIV4, I guess)

  pinMode( PIN_PROG,  OUTPUT );
  pinMode( PIN_RESET, OUTPUT );
  pinMode( PIN_CS,    OUTPUT );

	digitalWrite( PIN_CS,    HIGH );
	digitalWrite( PIN_PROG,  HIGH );
	digitalWrite( PIN_RESET, HIGH );

	delay( 100 );

	// try a reset
	digitalWrite( PIN_RESET, LOW );
	digitalWrite( PIN_RESET, HIGH );
	digitalWrite( PIN_PROG,  LOW );
	digitalWrite( PIN_PROG,  HIGH );

	// nice mem
	memset( bufDbg, 0x00, BUFSIZE );
	
}




//*************************************************************************************************
//***
//*************************************************************************************************
void loop()
{
	static uint8_t nrfPageAddr = 0;
	uint8_t ch;
	
	// no error or plausibility checks; you have been warned...
	if( Serial.available() > 0 )
	{
		switch( (ch = Serial.read()) )
		{
			//-----------------------------------------
			case '?':
				serDumpUsage();
				break;
			//-----------------------------------------
			case '+':
				if( nrfPageAddr < 63 )
					nrfPageAddr++;
				break;
			//-----------------------------------------
			case '-':
				if( nrfPageAddr > 0 )
					nrfPageAddr--;
				break;
			//-----------------------------------------
			case '0':
				nrfPageAddr = 0;
				break;
			//-----------------------------------------
			case 'S':
				bufDbg[0] = NRF_CMD_RDSR;
				bufDbg[1] = 0x00;         // result will be in here

				spiWriteRead ( 2,  (uint8_t *)&bufDbg );
				serDumpFSR( bufDbg[1] );
				break;
			//-----------------------------------------
			case 's':
				bufDbg[0] = NRF_CMD_RDSR;
				bufDbg[1] = 0x00;         // result will be in here

				spiWriteRead ( 2,  (uint8_t *)&bufDbg );
				serPrintHex08( bufDbg[1] );
				serPrintString( "\r\n" );
				break;
			//-----------------------------------------
			case 'W':
				bufDbg[0] = NRF_CMD_WREN;

				spiWriteRead ( 1,  (uint8_t *)&bufDbg );
				serPrintString( "WREN\r\n" );
				break;
			//-----------------------------------------
			case 'w':
				bufDbg[0] = NRF_CMD_WRDIS;

				spiWriteRead ( 1,  (uint8_t *)&bufDbg );
				serPrintString( "WRDIS\r\n" );
				break;
			//-----------------------------------------
			case 'r':
				bufDbg[0] = NRF_CMD_READ;
				bufDbg[1] = (nrfPageAddr * 512) >> 8;   // high
				bufDbg[2] = (nrfPageAddr * 512) & 0xff; // low
				bufDbg[3] = 0x00;                       // results from here on
				
				spiWriteRead ( 512 + 3, (uint8_t *)&bufDbg[0] );
				serDumpBufHex( 512,     (uint8_t *)&bufDbg[3] );
				break;
			//-----------------------------------------
			default:
				break;
		}// END switch
		
		// print page address (if changed)
		if( ch == '+' || ch == '-' || ch == '0' )
			Serial.println( nrfPageAddr );
	}
	
	
}


