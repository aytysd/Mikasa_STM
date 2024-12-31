///*
// * mycan.cpp
// *
// *  Created on: 2021/11/11
// *      Author: sora
// */
//
#include "mycan.hpp"
#include "drive.hpp"

uint32_t MyCAN::_rx_id = 0;
uint32_t MyCAN::_tx_id = 0;
uint32_t MyCAN::_pc_id = 0;
uint32_t MyCAN::_drive_md_id = 0;


MyCAN::MyCAN(CAN_HandleTypeDef *hcan, uint32_t pc_id):
_hcan(hcan),
_interrupt(0)
{}

uint8_t motor_id = 0;

uint32_t MyCAN::get_rxid()
{
	return _rx_id;
}

uint8_t MyCAN::data_get()
{
	return data[0];
}

void MyCAN::MyCAN_callback()
{

	data = getData();//can get target_angle
	received_target_angle = ((data[1] & 0b1) << 8)|((data[0] & 0b11111111) << 0);

	target_velo=(data[2] << 7)|((data[1] & 0b11111110) >> 1);
	data_check=true;


}

void MyCAN::setMotorID( uint8_t pc_id )
{

	_pc_id = pc_id;

	motor_id = 0;

    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0) motor_id = motor_id|(0b1<<3);
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0) motor_id = motor_id|(0b1<<2);
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == 0) motor_id = motor_id|(0b1<<1);
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == 0) motor_id = motor_id|0b1;

    _drive_md_id = motor_id + 4;

    _rx_id=((motor_id<<5)|_pc_id);
    _tx_id=((_drive_md_id<<5)|motor_id);
}

void MyCAN::init()
{
	HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	setMotorID( 31 );
	configFilter(&hcan);
}


void MyCAN::receive()
{
	CAN_RxHeaderTypeDef RxHeader;

	if (HAL_CAN_GetRxMessage(_hcan, CAN_RX_FIFO0, &RxHeader, _rx_data) == HAL_OK)
	{
		if (RxHeader.StdId == _rx_id)
		{
			MyCAN_callback();
		}

	}
}

void MyCAN::transmit(uint8_t *data, uint32_t size)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t TxMailbox;

	if (0 < HAL_CAN_GetTxMailboxesFreeLevel(_hcan))
	{
		TxHeader.StdId = _tx_id;
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

	uint32_t filter_id = (motor_id << 5) << 21;
	uint32_t filter_mask = ((motor_id << 5) << 21)|0x4;
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

//	   sFilterConfig.FilterIdHigh = 0;
//	   sFilterConfig.FilterIdLow = 0;
//	   sFilterConfig.FilterMaskIdHigh = 0;
//	   sFilterConfig.FilterMaskIdLow = 0;
//	   sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
//	   sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
//	   sFilterConfig.FilterBank = 0;
//	   sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
//	   sFilterConfig.SlaveStartFilterBank = 14;
//	   sFilterConfig.FilterActivation = ENABLE;
//	   HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
}

uint8_t *MyCAN::getData()
{
	return _rx_data;
}

