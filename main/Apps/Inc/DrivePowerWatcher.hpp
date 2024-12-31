/*
 * DrivePowerWatcher.hpp
 *
 *  Created on: Sep 16, 2022
 *      Author: ayato
 */

#ifndef INC_DRIVEPOWERWATCHER_HPP_
#define INC_DRIVEPOWERWATCHER_HPP_


#include <array>

#include "main.h"
#include "gpio.h"
#include "can.h"

#include "mycan.hpp"
#include "Instances.hpp"

namespace NITNC
{

#define POWERED_ON true
#define POWERED_OFF false


class DrivePowerWatcher
{

private:


	void PowerOnFromGPIO( void ){ HAL_GPIO_WritePin( GPIOC, GPIO_PIN_12, GPIO_PIN_RESET ); };
	void PowerOffFromGPIO( void ){ HAL_GPIO_WritePin( GPIOC, GPIO_PIN_12, GPIO_PIN_SET ); };


public:

	void Init()
	{
		HAL_GPIO_WritePin( GPIOC, GPIO_PIN_12, GPIO_PIN_RESET );
	}

	bool CheckGPIOControl( void )
	{
		if( HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_12 ) == GPIO_PIN_SET )
		{
			return POWERED_OFF;
		}
		else
		{
			return POWERED_ON;
		}
	}

	bool CheckFinalPower( void )
	{
    	if( HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_10 ) == GPIO_PIN_SET )
    	{
    		return POWERED_OFF;
    	}
    	else
    	{
    		return POWERED_ON;
    	}

	}


	bool CANFilter( uint32_t CANID )
	{
		if( ( CANID >> 5 ) == 1 )
		{
			if( ( CANID & 0b11111 ) == 31 )
			{
				return true;
			}
		}
		return false;

	}

	void CANCallback( uint32_t CANID, uint8_t* data )
	{
		if( data[ 7 ] == 127 )
		{

			if( data[ 0 ] == 255 )
			{
				PowerOnFromGPIO();
			}
			else
			{
	    		PowerOffFromGPIO();
			}

		}

	}


};
};




#endif /* INC_DRIVEPOWERWATCHER_HPP_ */
