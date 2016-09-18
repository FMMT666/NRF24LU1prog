
#include "Arduino.h"
#include <string.h>

#include "serial.h"


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
//*** serDumpBufHex()
//***
//*************************************************************************************************
void serDumpBufHex( uint16_t len, uint8_t *buf )
{
	uint8_t i = 0;
	char str[6];

	while( len-- )
	{
		sprintf( (char *)&str[0], "%02X ", *buf++ );
		serPrintString( (char *)&str );
		if( !(++i % 16) )
			serPrintString("\r\n");
	}
	serPrintString("\r\n");
	
}

