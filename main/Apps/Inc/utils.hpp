/*
 * utils.hpp
 *
 *  Created on: Apr 2, 2022
 *      Author: ayato
 */

#ifndef INC_UTILS_HPP_
#define INC_UTILS_HPP_

#pragma once



#include "main.h"
#include "gpio.h"
#include "usart.h"

#include <vector>
#include <string>


#define CW 0
#define CCW 1
#define BRAKE 1

#define V_MAX 2000

#define L1( x ) ( HAL_GPIO_WritePin( GPIOC, GPIO_PIN_10, ( GPIO_PinState )x ) )
#define L2( x ) ( HAL_GPIO_WritePin( GPIOE, GPIO_PIN_7, ( GPIO_PinState )x ) )
#define L3( x ) ( HAL_GPIO_WritePin( GPIOC, GPIO_PIN_12, ( GPIO_PinState )x ) )
#define L4( x ) ( HAL_GPIO_WritePin( GPIOA, GPIO_PIN_15, ( GPIO_PinState )x ) )
#define L5( x ) ( HAL_GPIO_WritePin( GPIOE, GPIO_PIN_8, ( GPIO_PinState )x ) )
#define L6( x ) ( HAL_GPIO_WritePin( GPIOD, GPIO_PIN_9, ( GPIO_PinState )x ) )


bool GetWirelessOption( void );

std::string Dec2HexString( uint64_t Data, uint8_t Length );
void HexString2Dec( std::string Data, std::vector<uint8_t>& Result );

void PublishToPC( uint8_t* pData, uint8_t Size );

void drive(double velocity);

uint8_t GetMyID( uint32_t CANID );
uint8_t GetPartnerID( uint32_t CANID );
uint8_t GetDataID( uint32_t CANID );

uint8_t ReadMyChannelFromGPIO( void );

template< class type >
void xprint( type value, std::string str );
uint32_t time_calc( void );

void str_output( std::string str );

void Counter( uint8_t Node, uint8_t* Rxdata, uint8_t size );

double GetNarrowestRad( double Base, double Target );

bool compare(const std::pair<int, int>&a, const std::pair<int, int>&b);

extern UART_HandleTypeDef* PCUart;


#endif /* INC_UTILS_HPP_ */
