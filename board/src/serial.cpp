
#include "Arduino.h"
#include <string.h>

#include "serial.h"


//*************************************************************************************************
//*** serDumpUsage()
//***
//*************************************************************************************************
void serDumpUsage()
{
	serPrintString("USAGE:\r\n");
	serPrintString("  ?  show help menu (this)\r\n");
	
	serPrintString("  +  increase mem page\r\n");
	serPrintString("  -  decrease mem page\r\n");
	serPrintString("  0  set mem page to zero\r\n");

	serPrintString("  S  show flash status (human readable)\r\n");
	serPrintString("  s  show flash status (FSR register value in HEX)\r\n");
	serPrintString("  W  enable flash write\r\n");
	serPrintString("  w  disable flash write\r\n");
	serPrintString("  E  erase all (requires W cmd to enable flash write\r\n");
	
	serPrintString("  r  read memory page from chip\r\n");
	serPrintString("  d  dump mem page in HEX\r\n");
		
}


//*************************************************************************************************
//*** serPrintString()
//***
//*************************************************************************************************
void serPrintString( char *pstr )
{
	if( pstr == NULL )
		return;
	
	while( *pstr != NULL )
		Serial.write( *pstr++ );
}


//*************************************************************************************************
//*** serPrintHex08()
//***
//*************************************************************************************************
void serPrintHex08( uint8_t value )
{
	uint8_t i = 0;
	char str[3];
	
	sprintf( (char *)&str[0], "%02X", value );
	serPrintString( (char *)&str );
}


//*************************************************************************************************
//*** serDumpBufHex()
//***
//*************************************************************************************************
void serDumpBufHex( uint16_t len, uint8_t *buf )
{
	uint8_t i = 0;
	char str[6];

	while( len-- )
	{
//		sprintf( (char *)&str[0], "%02X ", *buf++ );
//		serPrintString( (char *)&str );
		
		serPrintHex08( *buf++ );
		serPrintString(" ");
		
		if( !(++i % 16) )
			serPrintString("\r\n");
	}
	serPrintString("\r\n");
	
}

//*************************************************************************************************
//*** serDumpRegFSR()
//***
//*************************************************************************************************
void serDumpFSR( uint8_t value )
{
	char str[7];
	sprintf( (char *)&str[0], "0x%02X\r\n", value );
	
	serPrintString("FSR REGISTER: "); serPrintString( (char *)&str );
	serPrintString("  DBG    HW DBG:     "); value & 0x80 ? serPrintString("ON") : serPrintString("OFF"); serPrintString("\r\n");
	serPrintString("  STP    PRG START:  "); value & 0x40 ? serPrintString("PROTECTED") : serPrintString("0x0000"); serPrintString("\r\n");
	serPrintString("  WEN    FLASH WR/ER "); value & 0x20 ? serPrintString("ENABLED") : serPrintString("DISABLED"); serPrintString("\r\n");
	serPrintString("  RDYN   FLASH READY "); value & 0x10 ? serPrintString("BUSY") : serPrintString("READY"); serPrintString("\r\n");
	serPrintString("  INFEN  INFOPAGE    "); value & 0x08 ? serPrintString("ENABLED (*WARNING*)") : serPrintString("DISABLED"); serPrintString("\r\n");
	serPrintString("  RDISMB SPI RDLOCK  "); value & 0x04 ? serPrintString("LOCK ON") : serPrintString("LOCK OFF"); serPrintString("\r\n");
	serPrintString("  RDISIP SPI IPLOCK  "); value & 0x02 ? serPrintString("LOCK ON") : serPrintString("LOCK OFF"); serPrintString("\r\n");
	serPrintString("  RES    -\r\n");
}

