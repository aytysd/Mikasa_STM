/*
 * xprint.hpp
 *
 *  Created on: Jul 17, 2022
 *      Author: ayato
 */

#ifndef INC_XPRINT_HPP_
#define INC_XPRINT_HPP_


#include "main.h"
#include "usart.h"


#include <string>

#define GETVARNAME( var ) ( #var )

namespace NITNC
{
UART_HandleTypeDef* DebugUART = &huart1;

void str_output( std::string str )
{
	HAL_UART_Transmit( DebugUART, ( uint8_t* )str.c_str(), str.size(), 100 );
}


template< class type >
void xprint( type value, std::string str )
{
	str_output( str );

	std::string data = std::to_string( value );
	data += "\r\n";

	HAL_UART_Transmit( DebugUART, (uint8_t*)data.c_str(), data.size(), 100 );

}

template void xprint<int>( int value, std::string str );
template void xprint<double>( double value, std::string str );
template void xprint<uint8_t>( uint8_t value, std::string str );
template void xprint<int8_t>( int8_t value, std::string str );
template void xprint<uint32_t>( uint32_t value, std::string str );
template void xprint<uint64_t>( uint64_t value, std::string str );



}




#endif /* INC_XPRINT_HPP_ */
