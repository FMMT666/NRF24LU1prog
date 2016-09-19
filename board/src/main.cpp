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
			//--- help
			case '?':
				serDumpUsage();
				break;
			//-----------------------------------------
			//--- increase mem page
			case '+':
				if( nrfPageAddr < 63 )
					nrfPageAddr++;
				break;
			//-----------------------------------------
			//--- decrease mem page
			case '-':
				if( nrfPageAddr > 0 )
					nrfPageAddr--;
				break;
			//-----------------------------------------
			//--- set mem page to zero
			case '0':
				nrfPageAddr = 0;
				break;
			//-----------------------------------------
			//--- flash status (human readable)
			case 'S':
				serDumpFSR( nrfReadFSR() );
				break;
			//-----------------------------------------
			//--- flash status (single hex output)
			case 's':
				serPrintHex08( nrfReadFSR() );
				serPrintString( "\r\n" );
				break;
			//-----------------------------------------
			//--- enable flash write/erase
			case 'W':
				bufDbg[0] = NRF_CMD_WREN;

				spiWriteRead ( 1,  (uint8_t *)&bufDbg );
				serPrintString( "WREN\r\n" );
				break;
			//-----------------------------------------
			//--- disable flash write/erase
			case 'w':
				bufDbg[0] = NRF_CMD_WRDIS;

				spiWriteRead ( 1,  (uint8_t *)&bufDbg );
				serPrintString( "WRDIS\r\n" );
				break;
			//-----------------------------------------
			//--- erase all (but never the infopage)
			case 'E':
				// read flash status before doing anything...
				bufDbg[1] = nrfReadFSR();
				
				// reading the reserved bit should result in a zero
				if( bufDbg[1] & NRF_FSR_RESERVED )
				{
					serPrintString( "ERR\r\n" );
					break;
				}

				// if INFEN is set, the ERASE_ALL command will delete the
				// InfoPage and kill the NRF24LU1.
				// That's not what we want...
				if( bufDbg[1] & NRF_FSR_INFEN )
				{
					serPrintString( "ERR INFEN\r\n" );
					break;
				}

				// is flash erase allowed?
				if( (bufDbg[1] & NRF_FSR_WEN) == 0 )
				{
					serPrintString( "ERR WEN\r\n" );
					break;
				}
				
				// now, it's safe [tm] to erase the chip...
				bufDbg[0] = NRF_CMD_ERASEALL;
				spiWriteRead ( 1,  (uint8_t *)&bufDbg );
				serPrintString( "ERASING" );

				// wait until finished...
				while( nrfReadFSR() & NRF_FSR_RDYN )
					serPrintString( "." );
				serPrintString( "DONE\r\n" );
				
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


