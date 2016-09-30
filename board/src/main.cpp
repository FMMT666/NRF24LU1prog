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



// length of a "write to buffer" HEX string (w/o a trailing 0!)
// "AA01020304..0F"
// 2 HEX digits for the memory page address (0..31 dec), followed by 32 HEX digits,
// representing 16 bytes of memory data.
#define BUFWRITELEN  34

// buffer (mostly) for flash memory operations
#define BUFSIZEMEM  520
uint8_t bufMem[BUFSIZEMEM];

// buffer for commands (keep an eye on the length :-)
#define BUFSIZECMD  (BUFWRITELEN + 2)
uint8_t bufCmd[BUFSIZECMD];


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
	memset( bufMem, 0x00, BUFSIZEMEM );
	memset( bufCmd, 0x00, BUFSIZECMD );
	
}




//*************************************************************************************************
//***
//*************************************************************************************************
void loop()
{
	static uint8_t nrfPageAddr =  0;
//	static int16_t bufMem16Ptr = -1;  // 512 bytes of buffer divided in 16 byte chunks (0..31)
	static uint8_t pendingCmd  =  0;
	static uint8_t pendingData =  0;
	uint8_t ch;

	// no error or plausibility checks; you have been warned...
	if( Serial.available() > 0 )
	{
		// read byte from serial
		ch = Serial.read();

		//-------------------------------------------------------------------------
		//--- check pending command 'b'
		if( pendingCmd == 'b' )
		{
			if(  ( pendingData > 0 )  &&  ( isHexadecimalDigit(ch) )  ) 
			{
				bufCmd[BUFWRITELEN - pendingData] = tolower( ch );
				serPrintChar( ch );
				
				if( --pendingData == 0 )
				{
					pendingCmd =  0;
					
					// add a 0, just in case we need to print that to the console...
					bufCmd[BUFWRITELEN] = 0;
					serPrintString( "\r\n" );

					// calculate the buffer address
					uint16_t bAddr = 16 * (  16 * ser1Hex2Dec( bufCmd[0] ) + ser1Hex2Dec( bufCmd[1] )  );
					if( bAddr > 16 * 31 )
						// nope, that would exceed the buffer size
						serPrintString( "ERR\r\n" );
					else
					{
						// return what was received
						serPrintString( (char *)&bufCmd );
						serPrintString( "\r\n" );

						// store the received memory chunk
						for( uint8_t i = 0; i < 16; i++ )
							bufMem[ bAddr + i ] = 16 * ser1Hex2Dec( bufCmd[2 + 2*i] ) + ser1Hex2Dec( bufCmd[2 + 2*i + 1] );
							
					}// END everything is fine

				}// END no pending data
				
			}// END pending data & valid char
			else
			{
				pendingCmd =  0;
				pendingData = 0;
				serPrintString( "ERR\r\n" );
			}

			return;
		}// END pending cmd 'b'
		

		//-------------------------------------------------------------------------
		//--- check other commands
		switch( ch )
		{
			//-----------------------------------------
			//--- write to buffer (multi byte command)
			case 'b':
				pendingCmd  = 'b';
				pendingData = BUFWRITELEN;  // 34 bytes to read (HEXstring: 2 bytes mem page addr and 32 bytes mem HEX digits)
				serPrintString( "BUF\r\n" );
				break;
				
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
			//--- show mem page
			case '=':
				Serial.println( nrfPageAddr );
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
				bufCmd[0] = NRF_CMD_WREN;

				spiWrite( 1, bufCmd );
				serPrintString( "WREN\r\n" );
				break;
				
			//-----------------------------------------
			//--- disable flash write/erase
			case 'w':
				bufCmd[0] = NRF_CMD_WRDIS;

				spiWrite( 1, bufCmd );
				serPrintString( "WRDIS\r\n" );
				break;
				
			//-----------------------------------------
			//--- erase all OR program page
			case 'E':
			case 'P':
				uint8_t regFSR;
				// read flash status before doing anything...
				regFSR = nrfReadFSR();
				
				// reading the reserved bit should result in a zero
				if( regFSR & NRF_FSR_RESERVED )
				{
					serPrintString( "ERR\r\n" );
					break;
				}

				// if INFEN is set, the ERASE_ALL command will delete the
				// InfoPage and kill the NRF24LU1.
				// That's not what we want...
				if( regFSR & NRF_FSR_INFEN )
				{
					serPrintString( "ERR INFEN\r\n" );
					break;
				}

				// is flash erase allowed?
				if( (regFSR & NRF_FSR_WEN) == 0 )
				{
					serPrintString( "ERR WEN\r\n" );
					break;
				}

				if( ch == 'E' )
				{
					// erase the chip...
					bufCmd[0] = NRF_CMD_ERASEALL;
					spiWrite ( 1, bufCmd );
					serPrintString( "ERASING" );
				}
				else
				{
					// program a memory page
					if( (regFSR & NRF_FSR_RDISMB) )
					{
						serPrintString( "ERR RDISMB\r\n" );
						break;
					}
					
					serPrintString( "PROGRAMMING" );
					bufCmd[0] = NRF_CMD_PROGRAM;
					bufCmd[1] = (nrfPageAddr * 512) >> 8;   // high
					bufCmd[2] = (nrfPageAddr * 512) & 0xff; // low

					// first 256 bytes
					spiWrite2( 3, bufCmd, 256, (uint8_t *)&bufMem[0] );
					// wait until finished
					while( nrfReadFSR() & NRF_FSR_RDYN )
						serPrintString( "." );

					// ok, as it seems, a 2nd WREN is required...
					bufCmd[0] = NRF_CMD_WREN;
					spiWrite( 1, bufCmd );

					// second 256 bytes
					bufCmd[0] = NRF_CMD_PROGRAM;
					bufCmd[1]++;
					spiWrite2( 3, bufCmd, 256, (uint8_t *)&bufMem[256] );
				}

				// wait until finished...
				while( nrfReadFSR() & NRF_FSR_RDYN )
					serPrintString( "." );

				serPrintString( "\r\n" );
				serPrintString( "DONE\r\n" );
				
				break;
				
			//-----------------------------------------
			//--- read memory from chip (to buffer)
			case 'r':
				bufCmd[0] = NRF_CMD_READ;
				bufCmd[1] = (nrfPageAddr * 512) >> 8;   // high
				bufCmd[2] = (nrfPageAddr * 512) & 0xff; // low

				spiWriteRead2( 3, bufCmd, 512, bufMem );
				serPrintHex08( nrfPageAddr );
				serPrintString( "\r\n" );
				break;
				
			//-----------------------------------------
			//--- dump buffer to console (HEX)
			case 'd':
				serDumpBufHex( 512, bufMem );
				break;

			//-----------------------------------------
			//--- RESET and PROG pin cycle
			case 'i':
				digitalWrite( PIN_RESET, LOW );
				delay(10);
				digitalWrite( PIN_RESET, HIGH );
				delay(10);
				digitalWrite( PIN_PROG,  LOW );
				delay(10);
				digitalWrite( PIN_PROG,  HIGH );
				serPrintString( "RESET\r\n" );
				break;
				
			//-----------------------------------------
			//--- return marker
			case '\n':
				serPrintString( "nop\r\n" );
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


