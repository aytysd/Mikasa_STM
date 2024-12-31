
#include "mainpp.h"
#include "Instances.hpp"

#include "i2c.h"


#define FPS 100.0
#define CTRL_RATE ( ( 1.0 / FPS ) * 1000 )

//double yaw = 0;


void mainpp( void )
{
	using namespace NITNC;

	HAL_Delay( 100 );

	bno055_uart -> reset();
	bno055_uart -> init();


	can1.init();
	can2.init();

	HAL_Delay( 500 );

	odom.Init();


	drive_power_watcher.Init();

	uint32_t prev_time = HAL_GetTick();

	while( 1 )
	{


		if( bno055_uart -> GetIsResetNeeded() )
		{
			HAL_Delay( 1000 );
			bno055_uart -> init();
			bno055_uart -> SetIsResetNeeded( false );
		}

		uint32_t now = HAL_GetTick();

		if( ( now - prev_time ) > CTRL_RATE )
		{
			odom.SetAttitudeAngle( bno055_uart -> getEuler( 2 ) );
			odom.PubSelfPos();
//			yaw = bno055_uart.getEuler( 2 );
			prev_time = now;
		}


	}





}

