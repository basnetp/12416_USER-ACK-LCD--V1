/**
 * Driver for Winbond W25 serial FLASH device
 *
 * See https://www.pjrc.com/teensy/W25Q128FV.pdf for device reference
 *
 * This file uses a SPI driver to communicate with the device.
 *
 */

#include "nvm_w25.h"
#include "drv_spi.h"
#include "system.h"
//#include <stdio.h>
#include <xc.h>

//------------------------------//----------------------------------------------
// Definitions

#ifndef FLASH_CS1
#error "FLASH_CS1 is not defined. Please define in system_config.h."
#endif


// SPI commands to use with external FLASH memory devices
#define SPI_C_CMD_EVSR 0X50          // ENABLE VOLATILE STATUS REGISTER
#define SPI_C_CMD_WSR1_1 0X01        // WRITE STATUS REGISTER
#define SPI_C_CMD_WSR1_2 0           // DISABLE ALL WRITE PROTECT (WPS=0)
#define SPI_C_CMD_WSR1_3 0X1C        // DISABLE ALL WRITE PROTECT (WPS=1)
#define SPI_C_CMD_WSR2 0X31          // WRITE STATUS REGISTER 2
#define SPI_C_CMD_WSR3 0X11          // WRITE STATUS REGISTER 3
#define SPI_C_CMD_WEL 0X06           // WRITE ENABLE
#define SPI_C_CMD_EC 0XC7            // ERASE CHIP
#define SPI_C_CMD_RSR1 0X05          // READ SR1
#define SPI_C_CMD_RSR2 0X35          // READ SR2
#define SPI_C_CMD_RSR3 0X15          // READ SR3
#define SPI_C_CMD_PP 0X02            // PAGE PROGRAM
#define SPI_C_CMD_READ 0X03          // READ BYTES
#define SPI_C_CMD_FASTREAD 0x0B      // Fast read
#define SPI_C_CMD_JEDEC 0X9F         // READ JEDEC INFO
#define SPI_M_SR1_BUSY 0X01          // BUSY
#define SPI_M_SR3_WPS 0X04           // WPS

//------------------------------//----------------------------------------------
// Global variables

// Holds the SPI driver configuration used to communicate with the device
const DRV_SPI_INIT_DATA EXTFLASH_spiConfig = {
    SPI_CHANNEL_EXTFLASH,   // channel
    0b11,                   // primaryPrescale
    0b111,                  // secondaryPrescale
    SPI_CKE_IDLE_ACT,       // cke
    SPI_BUS_MODE_0,         // spi mode
    SPI_TRANSFER_MODE_8BIT  // transfer mode
};

//------------------------------//----------------------------------------------
// Local variables

// Keep track of which chip select we are supposed to use
static uint16_t _selectedDevice;

//------------------------------//----------------------------------------------
// Local functions

/**
 * @brief      Activates the current chip select pin
 */
static void _EnableChipSelect( void )
{
        FLASH_CS1 = 0;
}

/**
 * @brief      Deactivate the current chip select line
 */
static void _DisableChipSelect( void )
{
        FLASH_CS1 = 1;
}

/**
 * @brief      Reads the device Status Register 1
 *
 * @return     byte containing status bits
 */
static inline __attribute__( ( __always_inline__ ) ) uint8_t _ReadSR( uint8_t cmd )
{
    uint8_t temp;

    _EnableChipSelect();

    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, cmd );
    temp = DRV_SPI_Get( SPI_CHANNEL_EXTFLASH );

    _DisableChipSelect();

    return temp;
}

/**
 * @brief      Send the Write Enable command to the device
 */
static void _WriteEnable( void )
{
    _EnableChipSelect();
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, SPI_C_CMD_WEL );
    _DisableChipSelect();
    return;
};

//------------------------------//----------------------------------------------
// Public functions

/**
 * @brief      Designates which chip (chip select) line we should use
 *
 * @param[in]  chip  Index of the chip (i.e. 1 or 2)
 */
void EXTFLASH_Select( uint16_t chip )
{
    _selectedDevice = chip;
}

/**
 * CHECK FLASH CHIP'S JEDEC INFO
 *
 * Sends a SPI command to the flash device to verify its identity
 *
 * @return JDEC Value
 */
uint32_t EXTFLASH_JEDEC( void )
{
    uint32_t Response = 0;

    _EnableChipSelect();
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, SPI_C_CMD_JEDEC );

    Response |= (uint32_t)DRV_SPI_Get( SPI_CHANNEL_EXTFLASH ) << 16;
    Response |= (uint32_t)DRV_SPI_Get( SPI_CHANNEL_EXTFLASH ) << 8;
    Response |= (uint32_t)DRV_SPI_Get( SPI_CHANNEL_EXTFLASH );
    _DisableChipSelect();

    return Response;
}

/**
 * @brief      Calculates the CRC of the entire flash device
 *
 * We read the flash in pages (not because we have to; just so we can read
 * from the chip in blocks)
 *
 * @return     CRC-16
 */
#if 0
uint16_t EXTFLASH_Checksum( void )
{
    uint32_t crc = CRC16_INIT;

    uint32_t pages = EXTFLASH_SIZE / EXTFLASH_PAGE_SIZE;
    uint8_t buf[ EXTFLASH_PAGE_SIZE ];

    _EnableChipSelect();
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, SPI_C_CMD_READ );
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, 0 );
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, 0 );
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, 0 );

    while( pages > 0 )
    {
        uint16_t i;

        DRV_SPI_GetBuffer( SPI_CHANNEL_EXTFLASH, buf, EXTFLASH_PAGE_SIZE );

        for( i = 0; i < EXTFLASH_PAGE_SIZE; i++ )
        {
            crc = CRC16_Update( crc, buf[ i ] );
        }
        pages--;
    }

    _DisableChipSelect();

    return crc;
}
#endif

/**
 * @brief      Write a single page to the device
 *
 * @param[in]  address  starting address of the page
 * @param      p        pointer to byte buffer
 *
 * @return     true if no error
 */
bool EXTFLASH_WritePage( uint32_t address, uint8_t *p )
{
    // wait till the chip is not busy
    while( _ReadSR( SPI_C_CMD_RSR1 ) & SPI_M_SR1_BUSY )
        ;

    _WriteEnable();

    _EnableChipSelect();
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, SPI_C_CMD_PP );
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, address >> 16 );
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, address >> 8 );
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, address );

    DRV_SPI_PutBuffer( SPI_CHANNEL_EXTFLASH, p, EXTFLASH_PAGE_SIZE );

    _DisableChipSelect();

    return true;
}

/**
 * @brief      Read a block of memory from flash
 *
 * Note: correct device (chip select) needs to be chosen before calling this.
 *
 */
void EXTFLASH_Read( uint32_t address, uint8_t *p, uint16_t nCount )
{
    _EnableChipSelect();

    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, SPI_C_CMD_READ );
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, ( address >> 16 ) & 0xff );
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, ( address >> 8 ) & 0xff );
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, (address)&0xff );

    DRV_SPI_GetBuffer( SPI_CHANNEL_EXTFLASH, p, nCount );

    _DisableChipSelect();
}

/**
 *  ERASE W25Q128JV CHIP.
 *
 *   1. WRITE ENABLE VOLATILE STATUS REG(0X50)
 *   2. TURN OFF WRITE PROTECT(0X01 0X07)
 *   3. WEL(0X06)
 *   3. WRITE ERASE CHIP COMMAND(0X60)
 *   4. WAIT UNTIL STATUS REGISTER INDICATES COMPLETE (0X05 0X00)
 *
 */
uint8_t EXTFLASH_Erase( void )
{
    uint8_t ProtectOff;

    // ENABLE VOLATILE STATUS REG
    _EnableChipSelect();
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, SPI_C_CMD_EVSR );
    _DisableChipSelect();

    // Figure out what write protection is on
    ProtectOff = ( _ReadSR( SPI_C_CMD_RSR3 ) & SPI_M_SR3_WPS ) ? SPI_C_CMD_WSR1_3 : SPI_C_CMD_WSR1_2;
    // DISABLE WRITE PROTECT
    _EnableChipSelect();
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, SPI_C_CMD_WSR1_1 );
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, ProtectOff );
    _DisableChipSelect();

    // this is probably unnecessary
    _ReadSR( SPI_C_CMD_RSR1 );

    _WriteEnable();

    // this is probably unnecessary
    _ReadSR( SPI_C_CMD_RSR1 );

    // ERASE CHIP
    _EnableChipSelect();
    DRV_SPI_Put( SPI_CHANNEL_EXTFLASH, SPI_C_CMD_EC );
    _DisableChipSelect();

    // WAIT FOR COMPLETION
    while( _ReadSR( SPI_C_CMD_RSR1 ) & SPI_M_SR1_BUSY )
        ;
    return true;
};

/**
 * Reserves the SPI bus
 *
 * Configures the SPI interface and reserves
 * it for use.
 */
void EXTFLASH_BeginTransaction( unsigned int chip )
{
    while( !DRV_SPI_Lock( SPI_CHANNEL_EXTFLASH ) )
        ;

    _selectedDevice = chip;

    DRV_SPI_Initialize( &EXTFLASH_spiConfig );
}

/**
 * Releases the SPI bus when a transaction is finished.
 *
 *
 */
void EXTFLASH_EndTransaction( void )
{
    DRV_SPI_Unlock( SPI_CHANNEL_EXTFLASH );
}

/**
 * @brief      Module initialization
 *
 * This function should be called once at powerup
 */
void EXTFLASH_Initialize()
{
    // Make sure the flash chip selects are initialized properly
    EXTFLASH_Select( 1 );
    _DisableChipSelect();
    EXTFLASH_Select( 2 );
    _DisableChipSelect();
}