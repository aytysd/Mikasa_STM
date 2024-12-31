/*
 * utils.cpp
 *
 *  Created on: Apr 2, 2022
 *      Author: ayato
 */
#include "main.h"
#include "gpio.h"
#include "usart.h"

#include <unistd.h>
#include <algorithm>
#include <ios>
#include <sstream>
#include <iostream>
#include "math.h"

#include "utils.hpp"
#include "mycan.hpp"

UART_HandleTypeDef* PCUart = &huart1;
UART_HandleTypeDef* DebugUART = &huart1;

/**
 * @brief Get the Wireless Option( SBDBT or IM920 )
 *
 * @return true -> IM920
 * @return false -> SBDBT
 */

bool GetWirelessOption( void )
{
//    if( HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_4 ) == GPIO_PIN_SET )
//    {
//        return false;
//    }
//    else
//    {
//        return true;
//    }

	return true;
}

/**
 * @brief 十進数で送られてきたデータを，十六進の文字列に治す
 *
 * @param Data デーた
 * @param Length 文字列の長さ
 * @return std::string  十六進数の文字列
 */
std::string Dec2HexString( uint64_t Data, uint8_t Length )
{
    std::stringstream ss;
    ss << std::hex << Data;
    std::string string = ss.str();

    while( string.length() < Length )
        string.insert( 0, "0" );

    transform (string.begin(), string.end(), string.begin(), toupper);

    return string;
}


void HexString2Dec( std::string Data, std::vector<uint8_t>& Result )
{

    uint64_t number = ( uint64_t )strtoll( Data.c_str(), NULL, 16 );


    for( size_t i = 0; i < Data.size() / 2; i++ )
    {
    	uint8_t OneByte = ( uint8_t )( number & 0xFF );
        Result.insert( Result.begin(), OneByte );

        uint64_t numbuff = ( 0xFFFFFFFFFFFFFF00 & number ) >> 8;
        number = numbuff;
    }


}

void PublishToPC( uint8_t* pData, uint8_t Size )
{

	std::string msg;

	for( int i = 0; i < Size; i++ )
	{
		msg += Dec2HexString( pData[ i ], 2 );
	}

	msg += '\r';

	HAL_UART_Transmit( PCUart, ( uint8_t* )msg.c_str(), msg.size(), 10 );

}

uint8_t GetMyID( uint32_t CANID )
{
	uint8_t MyID = ( CANID & 0b11110000000 ) >> 7;
	return MyID;
}

uint8_t GetPartnerID( uint32_t CANID )
{
	uint8_t PartnerID = ( CANID & 0b00001111000 ) >> 3;
	return PartnerID;
}

uint8_t GetDataID( uint32_t CANID )
{
	uint8_t DataID = ( CANID & 0b00000000111 );
	return DataID;
}

uint8_t ReadMyChannelFromGPIO()
{
	uint8_t TensPlace = 0;
	if( HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_9 ) == GPIO_PIN_SET ) TensPlace |= 0b1000;
	if( HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_15 ) == GPIO_PIN_SET ) TensPlace |= 0b0100;
	if( HAL_GPIO_ReadPin( GPIOD, GPIO_PIN_8 ) == GPIO_PIN_SET ) TensPlace |= 0b0010;
	if( HAL_GPIO_ReadPin( GPIOE, GPIO_PIN_13 ) == GPIO_PIN_SET ) TensPlace |= 0b0001;

	uint8_t OnesPlace = 0;
	if( HAL_GPIO_ReadPin( GPIOE, GPIO_PIN_15 ) == GPIO_PIN_SET ) OnesPlace |= 0b1000;
	if( HAL_GPIO_ReadPin( GPIOE, GPIO_PIN_12 ) == GPIO_PIN_SET ) OnesPlace |= 0b0100;
	if( HAL_GPIO_ReadPin( GPIOE, GPIO_PIN_14 ) == GPIO_PIN_SET ) OnesPlace |= 0b0010;
	if( HAL_GPIO_ReadPin( GPIOE, GPIO_PIN_10 ) == GPIO_PIN_SET ) OnesPlace |= 0b0001;

	uint8_t Channel = TensPlace * 10 + OnesPlace;

	return Channel;
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

uint32_t time_calc( void )
{
	static int i;
	static uint32_t start_time;
	static uint32_t end_time;

	i++;
	switch( i % 2 )
	{
	case 1:
	{
		start_time = HAL_GetTick();

		break;
	}
	case 0:
	{
		end_time = HAL_GetTick();
		uint32_t diff = end_time - start_time;

		return diff;
//		xprint( diff, "diff:" );
		break;
	}

	}


}

void str_output( std::string str )
{
	HAL_UART_Transmit( DebugUART, ( uint8_t* )str.c_str(), str.size(), 100 );
}

void Counter( uint8_t Node, uint8_t* Rxdata, uint8_t size )
{
	xprint( Rxdata[ 0 ], "data" );
//	xprint( Node, "node:" );

	static bool Received[ 100 ] = { false };

	static uint8_t count = 0;
	count++;

	xprint( count, "count:" );

}

double GetNarrowestRad( double Base, double Target )
{
    double TargetSubBase = Target - Base;

    while( TargetSubBase < 0 )
    {
        TargetSubBase += 2 * M_PI;
    }

    if( TargetSubBase < M_PI )
    {
        return abs( TargetSubBase );
    }
    else
    {
        return abs( TargetSubBase ) - 2 * M_PI;
    }
}



bool compare(const std::pair<int, int>&a, const std::pair<int, int>&b)
{
   return a.second<b.second;
}





