/*
 * mainpp.h
 *
 *  Created on: 2022/05/25
 *      Author: aoi
 */

#ifndef SRC_MAINPP_H_
#define SRC_MAINPP_H_

#ifdef __cplusplus
 extern "C"{
#endif

#include "tim.h"

void mainpp();
void ControlPeriodCallback( TIM_HandleTypeDef *htim );



#ifdef __cplusplus
}
#endif



#endif /* SRC_MAINPP_H_ */
