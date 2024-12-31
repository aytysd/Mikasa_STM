#pragma once

#include <vector>
#include <functional>

#include "can.h"
#include "main.h"

#include "CallbackBase.hpp"

namespace NITNC
{
#define MYID 15

typedef std::function<void( uint32_t, uint8_t* )> CANCallbackFunc;
typedef std::function<bool( uint32_t )> CANActivator;


class MyCAN : CallbackBase<CANCallbackFunc, CANActivator>
{
public:

	void init( CAN_HandleTypeDef *hcan );

	void Receive();
	void Transmit(uint8_t DestinationID, uint8_t *data, uint32_t size, uint8_t type);


private:
	uint8_t _rx_data[8];
	CAN_HandleTypeDef *_hcan;

	static void CreateActivatorLamda( CANActivator& activator, uint8_t partner_id );
	static void configFilter(CAN_HandleTypeDef *hcan);
};

}



