/*
 * mycan.cpp
 *
 *  Created on: 2021/11/11
 *      Author: sora
 */


#include "main.h"
#include "can.h"

#include "IM920.hpp"
#include "mycan.hpp"
#include "utils.hpp"
#include "Odometry.hpp"
#include "Instances.hpp"



namespace NITNC
{
MyCAN::MyCAN(CAN_HandleTypeDef *hcan) :
_hcan(hcan)
{}

void MyCAN::init()
{
	HAL_CAN_Start(_hcan);

	if( _hcan -> Instance == CAN1 )
	{
		HAL_CAN_ActivateNotification(_hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	}
	else if( _hcan -> Instance == CAN2 )
	{
		HAL_CAN_ActivateNotification(_hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	}
	configFilter(_hcan);


	SetCallbackFunc
	(
		std::bind( &Odometry::OdomReset, &odom, std::placeholders::_1, std::placeholders::_2 ),
		std::bind( &Odometry::CANIDJudge, &odom, std::placeholders::_1 )
	);


	SetCallbackFunc
	(
		std::bind( &DrivePowerWatcher::CANCallback, &drive_power_watcher, std::placeholders::_1, std::placeholders::_2 ),
		std::bind( &DrivePowerWatcher::CANFilter, &drive_power_watcher, std::placeholders::_1 )
	);

}

void MyCAN::Receive()
{
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t buffer[8] = { 0 };

	if (HAL_CAN_GetRxMessage(_hcan, CAN_RX_FIFO0, &RxHeader, buffer) == HAL_OK)
	{
		for( size_t i = 0; i < CallbackFuncs_.JudgeFuncs.size(); i++ )
		{
			CANIDJudgeFunc JudgeFunc = CallbackFuncs_.JudgeFuncs[ i ];
			if ( JudgeFunc( RxHeader.StdId ) )
			{
				CANCallbackFunc CANCallbackFunc = CallbackFuncs_.CallbackFuncs[ i ];
				CANCallbackFunc( RxHeader.StdId, buffer );
			}

		}
	}
}

void MyCAN::Transmit(uint8_t DestinationID, uint8_t *data, uint32_t size)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t TxMailbox;

	uint16_t CANID = DestinationID << 5;
    CANID |= MYID;


	if (0 < HAL_CAN_GetTxMailboxesFreeLevel(_hcan))
	{
		TxHeader.StdId = CANID;
		TxHeader.RTR = CAN_RTR_DATA;
		TxHeader.IDE = CAN_ID_STD;
		TxHeader.DLC = size;
		TxHeader.TransmitGlobalTime = DISABLE;

		HAL_CAN_AddTxMessage(_hcan, &TxHeader, data, &TxMailbox);
	}

	HAL_CAN_Stop(_hcan);
	HAL_CAN_Start(_hcan);
}

void MyCAN::configFilter(CAN_HandleTypeDef *hcan)
{
	CAN_FilterTypeDef sFilterConfig;

	 uint32_t filter_id = (MYID << 5) << 21;
	 uint32_t filter_mask = ((MYID << 5) << 21)|0x4;
	 sFilterConfig.FilterIdHigh = filter_id >> 16;
	 sFilterConfig.FilterIdLow = filter_id;
	 sFilterConfig.FilterMaskIdHigh = filter_mask >> 16;
	 sFilterConfig.FilterMaskIdLow = filter_mask;
	 sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	 sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;

	 if( hcan -> Instance == CAN1 )
	 {
		 sFilterConfig.FilterBank = 0;
	 }
	 else if( hcan -> Instance == CAN2 )
	 {
		 sFilterConfig.FilterBank = 14;
	 }

	 sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	 sFilterConfig.SlaveStartFilterBank = 14;
	 sFilterConfig.FilterActivation = ENABLE;
	 HAL_CAN_ConfigFilter(hcan, &sFilterConfig);

}




void MyCAN::SetCallbackFunc( CANCallbackFunc CANCallbackFunc, CANIDJudgeFunc CANIDJudgeFunc )
{
	CallbackFuncs_.CallbackFuncs.push_back( CANCallbackFunc );
    CallbackFuncs_.JudgeFuncs.push_back( CANIDJudgeFunc );
}


};

void HAL_CAN_RxFifo0MsgPendingCallback( CAN_HandleTypeDef* hcan )
{
	using namespace NITNC;

	if( hcan -> Instance == CAN1 )
	{
		can1.Receive();
	}
	else if( hcan -> Instance == CAN2 )
	{
		can2.Receive();
	}
}


