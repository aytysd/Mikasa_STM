#pragma once

#include "main.h"
#include "usart.h"

#include <vector>
#include <functional>

#define EoF 13

typedef std::function<void( uint8_t, uint8_t*, uint8_t )> SBDBTCallbackFunc;

typedef struct StructSBDBTCallbackFuncs_t
{
	std::vector<SBDBTCallbackFunc> CallbackFuncs;
	std::vector<uint8_t> IDs;
} SBDBTCallbackFuncs_t;


class SBDBT
{
private:



	void SetCallbackFunc( SBDBTCallbackFunc CallbackFunc, uint8_t ID );

	SBDBTCallbackFuncs_t CallbackFuncs_;

public:

	SBDBT
	(
		UART_HandleTypeDef* huart
	):
	huart_( huart )
	{};


	void Init( void );

	void Transmit( uint8_t* pData, uint8_t Size );
	void ReceiveData( void );

	bool CANIDJudgeFunc( uint32_t CANID );
	void CANCallbackFunc( uint32_t CANID, uint8_t* Data );

	static uint8_t RxdataBuff_;
	std::vector<uint8_t> Rxdata_;

	UART_HandleTypeDef* huart_;

};


extern SBDBT sbdbt;
