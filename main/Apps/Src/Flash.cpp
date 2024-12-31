#include "main.h"

#include "Flash.hpp"

HAL_StatusTypeDef Flash::Erase( uint32_t sector )
{
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef erase;
    erase.TypeErase = FLASH_TYPEERASE_SECTORS;
    erase.Sector = sector;
    erase.NbSectors = 1;
    erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    
    uint32_t error_pointer;

    HAL_FLASHEx_Erase( &erase, &error_pointer );

    HAL_FLASH_Lock();

    if( error_pointer != 0xffffffff )
        return HAL_OK;
    else
        return HAL_ERROR;

}

template<>
void Flash::write( uint8_t value, uint32_t addr )
{
    HAL_FLASH_Unlock();
    HAL_FLASH_Program( FLASH_TYPEPROGRAM_BYTE, addr, value );
    HAL_FLASH_Lock();
}

template<>
void Flash::write( uint16_t value, uint32_t addr )
{
    HAL_FLASH_Unlock();
    HAL_FLASH_Program( FLASH_TYPEPROGRAM_HALFWORD, addr, value );
    HAL_FLASH_Lock();

}

template<>
void Flash::write( uint32_t value, uint32_t addr )
{
    HAL_FLASH_Unlock();
    HAL_FLASH_Program( FLASH_TYPEPROGRAM_WORD, addr, value );
    HAL_FLASH_Lock();

}

uint64_t Flash::read( int addr, uint32_t type )
{
    int* address = ( int* )addr;

    uint64_t data = *address;

    switch( type )
    {
    case FLASH_TYPEPROGRAM_BYTE:
        return ( uint8_t )data;
        break;
    case FLASH_TYPEPROGRAM_HALFWORD:
        return ( uint16_t )data;
        break;
    case FLASH_TYPEPROGRAM_WORD:
        return ( uint32_t )data;
        break;
    default:
        break;
    }
} 

