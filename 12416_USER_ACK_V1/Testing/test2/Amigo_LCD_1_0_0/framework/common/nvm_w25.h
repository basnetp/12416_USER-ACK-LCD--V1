/**
 * Defines the SPI interface to the Winbond W25 FLASH chips
 *
 *	Refer to the device data sheet (eg https://www.winbond.com/resource-files/w25q128jv%20spi%20revc%2011162016.pdf)
 *	for details
 *
 */

#ifndef _EXT_FLASH_
#define _EXT_FLASH_
#include "drv_spi.h"
#include <stdbool.h>
#include <stdint.h>


#define SPI_C_ID_W25Q128JV 0XEF4018  // JEDEC INFO FOR FLASH CHIP
#define SPI_C_ID_W25M512JV 0XEF7119  


// Size of a device page in bytes (from datasheet) is 256
#define EXTFLASH_PAGE_SIZE 256

// Size of the memory device in bytes
#define EXTFLASH_SIZE 0x1000000

extern const DRV_SPI_INIT_DATA EXTFLASH_spiConfig;

extern uint16_t EXTFLASH_CS;

/**
 * Reserves the SPI bus
 *
 * Configures the SPI interface and reserves
 * it for use.
 */
void EXTFLASH_BeginTransaction( unsigned int chip );

/**
 * Releases the SPI bus when a transaction is finished.
 *
 *
 */
void EXTFLASH_EndTransaction( void );

// void EXTFLASH_Select( uint16_t chip );

uint8_t EXTFLASH_Erase( void );

bool EXTFLASH_WritePage( uint32_t address, uint8_t *p );

/**
 * @brief      Reads an array of bytes from the specified address.
 *
 *  This routine reads an array of bytes from the specified address location. The
 *  read array is saved to the location pointed to by p. The number of bytes
 *  to be read is specified by nCount.
 *  Before calling this, you need to have used _BeginTransaction with the correct
 *  flash device, so we know which device to read.
 *
 * @param[in]  address  starting address of the array to be read
 * @param      p        pointer to the destination of the read array
 * @param[in]  nCount   specifies the number of bytes to be read
 */
void EXTFLASH_Read( uint32_t address, uint8_t *p, uint16_t nCount );

uint16_t EXTFLASH_Checksum( void );

uint32_t EXTFLASH_JEDEC( void );

void EXTFLASH_Initialize( void );

#endif
