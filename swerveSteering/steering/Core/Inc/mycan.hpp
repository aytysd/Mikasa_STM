/*
 * mycan.hpp
 *
 *  Created on: 2021/11/11
 *      Author: sora
 */

#ifndef NARALIB_MYCAN_HPP_
#define NARALIB_MYCAN_HPP_

#include "can.h"

class MyCAN
{
public:
	MyCAN(CAN_HandleTypeDef *hcan, uint32_t pc_id);

	static void init();

	static void setMotorID( uint8_t pc_id );

	void receive();

	void transmit(uint8_t *data, uint32_t size);

	void setInterrupt(bool state) { _interrupt = state; }

	bool getInterrupt(){ return _interrupt; }

	void resetData(){ for (uint8_t i = 0; i < 8; i++) _rx_data[i] = 0; }

	uint8_t *getData();

	uint32_t get_rxid();

	void MyCAN_callback();

	uint8_t data_get();

private:
	uint8_t _rx_data[8];
	CAN_HandleTypeDef *_hcan;
	static uint32_t _rx_id;
	static uint32_t _tx_id;
	static uint32_t _drive_md_id;
	static uint32_t _pc_id;
	bool _interrupt;
	uint8_t *data;

	static void configFilter(CAN_HandleTypeDef *hcan);
};



#endif /* NARALIB_MYCAN_HPP_ */
