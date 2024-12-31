/*
 * mycan.cpp
 *
 *  Created on: 2021/11/11
 *      Author: sora
 */


#include "main.h"
#include "can.h"

#include "mycan.hpp"
#include "drive.hpp"
#include "Instances.hpp"

namespace NITNC
{
void MyCAN::init()
{
	_hcan = &hcan;
	HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	SetMotorID();
	configFilter(&hcan);

	SetCallbackFunc
	(
		std::bind( &Drive::CANCallback, &drive, std::placeholders::_1, std::placeholders::_2 ),
		std::bind( &Drive::CANIDJudge, &drive, std::placeholders::_1 )
	);


}

void MyCAN::SetMotorID()
{

	my_id = 0;

    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0) my_id = my_id|(0b1<<3);
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0) my_id = my_id|(0b1<<2);
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == 0) my_id = my_id|(0b1<<1);
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == 0) my_id = my_id|0b1;

    _rx_id = ( ( my_id << 5 ) | ( my_id - 4 ) );
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

			if( JudgeFunc( RxHeader.StdId ) )
			{
				CANCallbackFunc CANCallbackFunc = CallbackFuncs_.CallbackFuncs[ i ];
				CANCallbackFunc( RxHeader.StdId, buffer );
			}

		}
	}
}

void MyCAN::Transmit(uint8_t DestinationID, uint8_t *data, uint32_t size, uint8_t type)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t TxMailbox;

	uint16_t CANID = DestinationID << 5;
    CANID |= my_id;


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

	 uint32_t filter_id = (my_id << 5) << 21;
	 uint32_t filter_mask = ((my_id << 5) << 21)|0x4;
	 sFilterConfig.FilterIdHigh = filter_id >> 16;
	 sFilterConfig.FilterIdLow = filter_id;
	 sFilterConfig.FilterMaskIdHigh = filter_mask >> 16;
	 sFilterConfig.FilterMaskIdLow = filter_mask;
	 sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	 sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	 sFilterConfig.FilterBank = 0;
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


}

