
/*
 * bno055.cpp
 *
 *  Created on: 2021/11/11
 *      Author: sora
 */
#define _USE_MATH_DEFINES
#include "../Inc/bno055.hpp"
#include "math.h"
#include "i2c.h"


namespace NITNC
{
BNO055::BNO055(I2C_HandleTypeDef *hi2c) :
_hi2c(hi2c),
_euler{0},
_init_euler{0}
{}

void BNO055::init()
{
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
	uint8_t opr_config_mode[2] = { BNO055_OPR_MODE, CONFIGMODE };
	HAL_I2C_Master_Transmit(_hi2c, BNO055_I2C_ADDR_LO<<1, opr_config_mode, sizeof(opr_config_mode), 10);
	HAL_Delay(10);

	// Select page 1 to configure sensors
	uint8_t conf_page1[2] = {BNO055_PAGE_ID, 0x01};
	HAL_I2C_Master_Transmit(_hi2c, BNO055_I2C_ADDR_LO<<1, conf_page1, sizeof(conf_page1), 10);
	HAL_Delay(10);

	// Configure ACC (Page 1; 0x08)
	uint8_t conf_acc[2] = {BNO055_ACC_CONFIG, (uint8_t)(APwrMode << 5 | Abw << 2 | Ascale)};
	HAL_I2C_Master_Transmit(_hi2c, BNO055_I2C_ADDR_LO<<1, conf_acc, sizeof(conf_acc), 10);
	HAL_Delay(10);

	// Configure GYR
	uint8_t conf_gyro[2] = {BNO055_GYRO_CONFIG_0, (uint8_t)(Gbw << 3 | Gscale)};
	HAL_I2C_Master_Transmit(_hi2c, BNO055_I2C_ADDR_LO<<1, conf_gyro, sizeof(conf_gyro), 10);
	HAL_Delay(10);

	uint8_t conf_gyro_pwr[2] = {BNO055_GYRO_CONFIG_1, GPwrMode};
	HAL_I2C_Master_Transmit(_hi2c, BNO055_I2C_ADDR_LO<<1, conf_gyro_pwr, sizeof(conf_gyro_pwr), 10);
	HAL_Delay(10);

	// Configure MAG
	uint8_t conf_mag_pwr[4] = {REG_WRITE, BNO055_MAG_CONFIG, 0x01, (uint8_t)((MPwrMode << 5) | (MOpMode << 3) | Modr)};
	HAL_I2C_Master_Transmit(_hi2c, BNO055_I2C_ADDR_LO<<1, conf_mag_pwr, sizeof(conf_mag_pwr), 10);
	HAL_Delay(10);

	// Select BNO055 gyro temperature source
	//PutHexString(START_BYTE, BNO055_TEMP_SOURCE, 0x01 );

	// Select page 0
	uint8_t conf_page0[2] = {BNO055_PAGE_ID, 0x00};
	HAL_I2C_Master_Transmit(_hi2c, BNO055_I2C_ADDR_LO<<1, conf_page0, sizeof(conf_page0), 10);
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
	uint8_t pwr_pwrmode[2] = {BNO055_PWR_MODE, PWRMode};
	HAL_I2C_Master_Transmit(_hi2c, BNO055_I2C_ADDR_LO<<1, pwr_pwrmode, sizeof(pwr_pwrmode), 10);
	HAL_Delay(10);

	// Select BNO055 system operation mode (Page 0; 0x3D)
	uint8_t opr_oprmode[2] = {BNO055_OPR_MODE, OPRMode};
	HAL_I2C_Master_Transmit(_hi2c, BNO055_I2C_ADDR_LO<<1, opr_oprmode, sizeof(opr_oprmode), 10);
	HAL_Delay(50);

}

double BNO055::getEuler(uint8_t axis)
{
	calcEuler();
	_euler[axis] = _euler[axis] - _init_euler[axis];
	if (round(_euler[axis]) < 0) 	_euler[axis] += 2*M_PI;
	if (_euler[axis]/(2.0*M_PI))	_euler[axis] -= round(_euler[axis]/(2*M_PI))*2*M_PI;

	if( _euler[ axis ] < 0 )
	{
		_euler[ axis ] += 2 * M_PI;
	}
	
        // HAL_UART_Transmit( &huart1, ( uint8_t* )&data, 1, 100 );
	return _euler[axis];
}


void BNO055::getQuaternion(double* quaternion)
{
	short quat[4] = {0};
	uint8_t data[8] = {0};

	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(_hi2c, BNO055_I2C_ADDR_LO<<1, BNO055_QUA_DATA_W_LSB, I2C_MEMADD_SIZE_8BIT, data, sizeof(data), 100);

	quat[0] = (data[1]<<8) | data[0];
	quat[1] = (data[3]<<8) | data[2];
	quat[2] = (data[5]<<8) | data[4];
	quat[3] = (data[7]<<8) | data[6];

	for(int i=0;i<4;i++)	quaternion[i] = (float)quat[i] / 16384;
}

double test = 0;

void BNO055::calcEuler()
{
	double quaternion[4] = {0};
	getQuaternion(quaternion);


	double sinr_cosp = 2.0*(quaternion[0]*quaternion[1] + quaternion[2]*quaternion[3]);
	double cosr_cosp = 1.0 - 2.0*(quaternion[1]*quaternion[1] + quaternion[2]*quaternion[2]);
	_euler[0] = atan2(sinr_cosp, cosr_cosp);

	double sinp = 2.0*(quaternion[0]*quaternion[2] - quaternion[3]*quaternion[1]);
	if (fabs(sinp) >= 1)	_euler[1] = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
	else					_euler[1] = asin(sinp);

	double siny_cosp = 2.0*(quaternion[0]*quaternion[3] + quaternion[1]*quaternion[2]);
	double cosy_cosp = 1.0 - 2.0*(quaternion[3]*quaternion[3] + quaternion[2]*quaternion[2]);

	_euler[2] = atan2(siny_cosp, cosy_cosp);

}


void BNO055::reset()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_Delay(50);
}


}


