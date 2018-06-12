
#include "Arduino.h"

#include "config.h"
#include "spi_loc.h"
#include "nrf24lu1.h"


//*************************************************************************************************
//*** nrfReadFSR()
//***
//*************************************************************************************************
uint8_t nrfReadFSR()
{
	uint8_t buf[2];

	buf[0] = NRF_CMD_RDSR;
	buf[1] = 0xFF;

	spiWriteRead ( 2, buf );
	
	return buf[1];
}

