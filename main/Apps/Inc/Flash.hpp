/*
 * Flash.hpp
 *
 *  Created on: Apr 2, 2022
 *      Author: ayato
 */

#ifndef INC_FLASH_HPP_
#define INC_FLASH_HPP_


#include "main.h"

class Flash
{
public:
    static HAL_StatusTypeDef Erase( uint32_t sector );

    template< typename type >
    static void write( type value, uint32_t addr );

    static uint64_t read( int addr, uint32_t type );
};



#endif /* INC_FLASH_HPP_ */
