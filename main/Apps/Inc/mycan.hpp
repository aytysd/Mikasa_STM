/*
 * mycan.hpp
 *
 *  Created on: 2021/11/11
 *      Author: sora
 */

#ifndef NARALIB_MYCAN_HPP_
#define NARALIB_MYCAN_HPP_

#include <stdbool.h>
#include <vector>
#include <functional>

#include "can.h"
#include "main.h"

namespace NITNC
{
typedef std::function<void( uint32_t, uint8_t* )> CANCallbackFunc;
typedef std::function<bool( uint32_t )> CANIDJudgeFunc;

typedef struct StructCallbackFuncs
{
    std::vector<CANCallbackFunc> CallbackFuncs;
    std::vector<CANIDJudgeFunc> JudgeFuncs;
} CANCallbackFuncs_t;


#define MYID 1

class MyCAN
{
public:
	MyCAN(CAN_HandleTypeDef *hcan);

	void init();

	void Receive();

	void Transmit(uint8_t DestinationID, uint8_t *data, uint32_t size);
	

private:
	uint8_t _rx_data[8];
	CAN_HandleTypeDef *_hcan;
	CANCallbackFuncs_t CallbackFuncs_;

    void SetCallbackFunc( CANCallbackFunc CANCallbackFunc, CANIDJudgeFunc CANIDJudgeFunc );
	void configFilter(CAN_HandleTypeDef *hcan);
};

};




#endif /* NARALIB_MYCAN_HPP_ */
