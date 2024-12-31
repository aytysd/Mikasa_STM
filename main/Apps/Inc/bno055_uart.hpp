/*
 * bno055_uart.hpp
 *
 *  Created on: Oct 26, 2022
 *      Author: ayato
 */

#ifndef INC_BNO055_UART_HPP_
#define INC_BNO055_UART_HPP_

#include "usart.h"
#include "bno055.hpp"

#include <vector>
#include <array>
#include <queue>


namespace NITNC
{

enum RxStatus
{
	WAITING_RESPONSE = 0x00,
	WAITING_BUFFER_SIZE,
	COLLECTING_DATA,
};

class BNO055UART : public BNO055
{
public:

	static uint8_t m_p_RxBuffer[ 10 ];


	BNO055UART
	(
		UART_HandleTypeDef* huart,
		GPIO_TypeDef* hport,
		uint16_t pin
	) : BNO055( nullptr )
	{
		m_p_huart = huart;

		m_p_hport = hport;
		m_pin = pin;
	}

	void reset( void )
	{
		HAL_GPIO_WritePin( m_p_hport, m_pin, GPIO_PIN_RESET);
		HAL_Delay(50);
		HAL_GPIO_WritePin( m_p_hport, m_pin, GPIO_PIN_SET );
		HAL_Delay(50);
	}


	void init( void )
	{

//		HAL_UART_Receive_IT( m_p_huart, ( uint8_t* )m_p_RxBuffer, COUNTOF( m_p_RxBuffer ) );

		uint8_t GPwrMode 	= NormalG;   		// Gyro power mode
		uint8_t Gscale 		= GFS_2000DPS; 	// Gyro full scale
		//uint8_t Godr	 	= GODR_250Hz;  	// Gyro sample rate
		uint8_t Gbw 		= GBW_230Hz;    // Gyro bandwidth
		//
		uint8_t Ascale 		= AFS_16G;      // Accel full scale
		uint8_t APwrMode 	= NormalA;   		// Accel power mode
		uint8_t Abw 		= ABW_250Hz;    // Accel bandwidth, accel sample rate divided by ABW_divx
		//
		//uint8_t Mscale 	= MFS_4Gauss;		// Select magnetometer full-scale resolution
		uint8_t MOpMode 	= EnhancedRegular;    	// Select magnetometer perfomance mode
		uint8_t MPwrMode 	= Normal;    		// Select magnetometer power mode
		uint8_t Modr 			= MODR_30Hz;    // Select magnetometer ODR when in BNO055 bypass mode

		uint8_t PWRMode 	= Normalpwr;  	// Select BNO055 power mode
		uint8_t OPRMode 	= IMU;    	// specify operation mode for sensors [ACCONLY|MAGONLY|GYROONLY|ACCMAG|ACCGYRO|MAGGYRO|AMG|NDOF|NDOF_FMC_OFF]

	//	reset();
		// Select BNO055 config mode
		uint8_t opr_config_mode[5] = { START_BYTE, REG_WRITE, BNO055_OPR_MODE, 1, CONFIGMODE };
		HAL_UART_Transmit( m_p_huart, opr_config_mode, COUNTOF( opr_config_mode ), 100 );
		HAL_Delay(10);

		// Select page 1 to configure sensors
		uint8_t conf_page1[5] = { START_BYTE, REG_WRITE, BNO055_PAGE_ID, 1, 0x01 };
		HAL_UART_Transmit( m_p_huart, conf_page1, COUNTOF( conf_page1 ), 100 );
		HAL_Delay(10);

		// Configure ACC (Page 1; 0x08)
		uint8_t conf_acc[5] = { START_BYTE, REG_WRITE, BNO055_ACC_CONFIG, 1, static_cast<uint8_t>(APwrMode << 5 | Abw << 2 | Ascale)};
		HAL_UART_Transmit( m_p_huart, conf_acc, COUNTOF( conf_acc ), 100 );
		HAL_Delay(10);

		// Configure GYR
		uint8_t conf_gyro[5] = { START_BYTE, REG_WRITE, BNO055_GYRO_CONFIG_0, 1, static_cast<uint8_t>(Gbw << 3 | Gscale)};
		HAL_UART_Transmit( m_p_huart, conf_gyro, COUNTOF( conf_gyro ), 100 );
		HAL_Delay(10);

		uint8_t conf_gyro_pwr[5] = { START_BYTE, REG_WRITE, BNO055_GYRO_CONFIG_1, 1, GPwrMode};
		HAL_UART_Transmit( m_p_huart, conf_gyro_pwr, COUNTOF( conf_gyro_pwr ), 100 );
		HAL_Delay(10);

		// Configure MAG
		uint8_t conf_mag_pwr[5] = { START_BYTE, REG_WRITE, BNO055_MAG_CONFIG, 0x01, static_cast<uint8_t>((MPwrMode << 5) | (MOpMode << 3) | Modr)};
		HAL_UART_Transmit( m_p_huart, conf_mag_pwr, COUNTOF( conf_mag_pwr ), 100 );
		HAL_Delay(10);


		// Select BNO055 gyro temperature source
		//PutHexString(START_BYTE, BNO055_TEMP_SOURCE, 0x01 );


		// Select page 0
		uint8_t conf_page0[5] = { START_BYTE, REG_WRITE, BNO055_PAGE_ID, 1, 0x00 };
		HAL_UART_Transmit( m_p_huart, conf_page0, COUNTOF( conf_page0 ), 100 );
		HAL_Delay(10);

		// Select BNO055 sensor units (Page 0; 0x3B, default value 0x80)
		/*- ORIENTATION_MODE		 - Android					(default)
			- VECTOR_ACCELEROMETER - m/s^2  					(default)
			- VECTOR_MAGNETOMETER  - uT								(default)
			- VECTOR_GYROSCOPE     - rad/s        v		(must be configured)
			- VECTOR_EULER         - degrees					(default)
			- VECTOR_LINEARACCEL   - m/s^2        v		(default)
			- VECTOR_GRAVITY       - m/s^2						(default)
		*/
		//const char conf_units[4] = {REG_WRITE, BNO055_UNIT_SEL, 0x01, 0x82};
		//SendAccelData(USART1, (uint8_t*)conf_units);
		//HAL_Delay(50);

		// Select BNO055 system power mode (Page 0; 0x3E)
		uint8_t pwr_pwrmode[5] = { START_BYTE, REG_WRITE, BNO055_PWR_MODE, 1, PWRMode };
		HAL_UART_Transmit( m_p_huart, pwr_pwrmode, COUNTOF( pwr_pwrmode ), 100 );
		HAL_Delay(10);

		// Select BNO055 system operation mode (Page 0; 0x3D)
		uint8_t opr_oprmode[ 5 ] = { START_BYTE, REG_WRITE, BNO055_OPR_MODE, 1, OPRMode };
		HAL_UART_Transmit( m_p_huart, opr_oprmode, COUNTOF( opr_oprmode ), 100 );
		HAL_Delay(50);

	}

	void sendQuatReadOutRequest( void )
	{
		uint8_t quatReadOutRequest[ 4 ] = { START_BYTE, REG_READ, BNO055_QUA_DATA_W_LSB, 8 };
		HAL_StatusTypeDef status = HAL_UART_Transmit( m_p_huart, quatReadOutRequest, COUNTOF( quatReadOutRequest ), 100 );
		__HAL_UART_CLEAR_OREFLAG( m_p_huart );
		__HAL_UART_CLEAR_NEFLAG( m_p_huart );
		HAL_StatusTypeDef rx_status = HAL_UART_Receive( m_p_huart, ( uint8_t* )m_p_RxBuffer, COUNTOF( m_p_RxBuffer ), 5 );

		if( rx_status == HAL_BUSY )
		{

		}
		RxInterrupt( m_p_huart );

	}

	double getEuler(uint8_t axis)
	{
		calcEuler();
		_euler[axis] = _euler[axis] - _init_euler[axis];
		if (round(_euler[axis]) < 0) 	_euler[axis] += 2*M_PI;
		if (_euler[axis]/(2.0*M_PI))	_euler[axis] -= round(_euler[axis]/(2*M_PI))*2*M_PI;

		if( _euler[ axis ] < 0 )
		{
			_euler[ axis ] += 2 * M_PI;
		}

		return _euler[axis];
	}




	void RxInterrupt( UART_HandleTypeDef* huart )
	{

//		HAL_UART_Receive_IT( m_p_huart, m_p_RxBuffer, COUNTOF( m_p_RxBuffer ) );

		for( size_t i = 0; i < COUNTOF( m_p_RxBuffer ); i++ )
		{
			switch( m_Flag )
			{
			case WAITING_BUFFER_SIZE:
			{
				if( m_p_RxBuffer[ i ] == 8 )
				{
					m_Flag = COLLECTING_DATA;
					m_ElementCounter = 0;

					m_arr_CurrentBuffer.fill( 0 );
				}
				else
				{
					m_Flag = WAITING_RESPONSE;
				}

				break;
			}
			case WAITING_RESPONSE:
			{
				if( m_p_RxBuffer[ i ] == RESPONSE_BYTE )
				{
					m_Flag = WAITING_BUFFER_SIZE;
				}

				break;
			}
			case COLLECTING_DATA:
			{
				if( m_ElementCounter != 8 )
				{
					m_arr_CurrentBuffer[ m_ElementCounter ] = m_p_RxBuffer[ i ];
					m_ElementCounter++;
				}
				else
				{
					m_q_RxAllignedBuffers.push( m_arr_CurrentBuffer );
					m_Flag = WAITING_RESPONSE;
				}

				break;
			}
			};


		}

		getQuaternionAndEulerFromAllignedBuffer();
	}




private:

	std::queue<std::array<uint8_t, 8>> m_q_RxAllignedBuffers;
	std::array<uint8_t, 8> m_arr_CurrentBuffer;

	size_t m_ElementCounter = 0;

	enum RxStatus m_Flag = WAITING_RESPONSE;

	UART_HandleTypeDef* m_p_huart;

	GPIO_TypeDef* m_p_hport;
	uint16_t m_pin;

	double m_p_quaternion[ 4 ] = { 0 };


	void getQuaternionAndEulerFromAllignedBuffer( void )
	{

		if( m_q_RxAllignedBuffers.empty() )
		{

		}
		else
		{
			short quat[4] = {0};

			std::array<uint8_t, 8> data = m_q_RxAllignedBuffers.front();
			m_q_RxAllignedBuffers.pop();

			quat[0] = (data[1]<<8) | data[0];
			quat[1] = (data[3]<<8) | data[2];
			quat[2] = (data[5]<<8) | data[4];
			quat[3] = (data[7]<<8) | data[6];


			for(int i=0;i<4;i++)	m_p_quaternion[i] = (float)quat[i] / 16384;

		}


	}



	void calcEuler( void )
	{
		sendQuatReadOutRequest();

		double sinr_cosp = 2.0*(m_p_quaternion[0]*m_p_quaternion[1] + m_p_quaternion[2]*m_p_quaternion[3]);
		double cosr_cosp = 1.0 - 2.0*(m_p_quaternion[1]*m_p_quaternion[1] + m_p_quaternion[2]*m_p_quaternion[2]);
		_euler[0] = atan2(sinr_cosp, cosr_cosp);

		double sinp = 2.0*(m_p_quaternion[0]*m_p_quaternion[2] - m_p_quaternion[3]*m_p_quaternion[1]);
		if (fabs(sinp) >= 1)	_euler[1] = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
		else					_euler[1] = asin(sinp);

		double siny_cosp = 2.0*(m_p_quaternion[0]*m_p_quaternion[3] + m_p_quaternion[1]*m_p_quaternion[2]);
		double cosy_cosp = 1.0 - 2.0*(m_p_quaternion[3]*m_p_quaternion[3] + m_p_quaternion[2]*m_p_quaternion[2]);

		_euler[2] = atan2(siny_cosp, cosy_cosp);

	}


};

};


#endif /* INC_BNO055_UART_HPP_ */
