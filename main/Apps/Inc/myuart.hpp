#pragma once

#include "main.h"
#include "usart.h"


#ifdef __cplusplus
extern "C"
{
#endif

void IM920Callback( UART_HandleTypeDef* huart );
void SBDBTCallback( UART_HandleTypeDef* huart );

#ifdef __cplusplus
}
#endif