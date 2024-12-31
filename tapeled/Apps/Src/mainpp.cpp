/*
 * mainpp.cpp
 *
 *  Created on: Sep 4, 2022
 *      Author: ayato
 */


#include "can.h"

#include "Instances.hpp"
#include "mainpp.h"

void mainpp()
{
	using namespace NITNC;

	can1.init( &hcan );

	while( 1 )
	{
		tape_led.Drive();
		HAL_Delay( 100 );
	}
}




