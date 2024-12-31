#include "main.h"
#include "gpio.h"

#include "IM920.hpp"
#include "utils.hpp"
#include "mycan.hpp"
#include "bno055.hpp"
#include "Odometry.hpp"
#include "Instances.hpp"




void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{

	using namespace NITNC;

    switch ( GPIO_Pin )
    {
    case GPIO_PIN_0:
    {
        break;
    }
    case GPIO_PIN_1:
    {
        break;
    }
    case GPIO_PIN_2:
    {
        break;
    }
    case GPIO_PIN_3:
    {

        break;
    }
    case GPIO_PIN_4:
    {
        break;
    }
    case GPIO_PIN_5:
    {
        break;
    }
    case GPIO_PIN_6:
    {
        break;
    }
    case GPIO_PIN_7:
    {
        break;
    }
    case GPIO_PIN_8:
    {
    	odom.OdomResetOnSW();
        break;
    }
    case GPIO_PIN_9:
    {
    	bno055.SetIsResetNeeded( true );
        break;
    }
    case GPIO_PIN_10:
    {
        break;
    }
    case GPIO_PIN_11:
    {
        break;
    }
    case GPIO_PIN_12:
    {
        break;
    }
    case GPIO_PIN_13:
    {
        break;
    }
    case GPIO_PIN_14:
    {
        break;
    }
    case GPIO_PIN_15:
    {
        break;
    }
    default:
        break;
    }
}

