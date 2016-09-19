
// NRF24LU1+

// SPI COMMANDS
#define NRF_CMD_WREN      (0x06)
#define NRF_CMD_WRDIS     (0x04)
#define NRF_CMD_RDSR      (0x05)
#define NRF_CMD_WRSR      (0x01)
#define NRF_CMD_READ      (0x03)
#define NRF_CMD_ERASEPAGE (0x52)
#define NRF_CMD_ERASEALL  (0x62)

// FSR register bits
#define NRF_FSR_DBG       (0x80)
#define NRF_FSR_STP       (0x40)
#define NRF_FSR_WEN       (0x20)
#define NRF_FSR_RDYN      (0x10)
#define NRF_FSR_INFEN     (0x08)
#define NRF_FSR_RDISMB    (0x04)
#define NRF_FSR_RDISIP    (0x02)
#define NRF_FSR_RESERVED  (0x01) // will read as 0


extern uint8_t nrfReadFSR(void);

