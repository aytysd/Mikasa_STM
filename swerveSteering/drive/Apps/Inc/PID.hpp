/*
 * PID.cpp
 *
 *  Created on: Jul 17, 2022
 *      Author: ayato
 */

#pragma once

#include "math.h"

namespace NITNC
{

#define P_MAX 200
#define I_MAX 50
#define D_MAX 10

class PID
{
private:

	double Kp = 0.1;
	double Ki = 0.8;
	double Kd = 0;

	double DeltaT = 0.01;

	const double IntegralMax = static_cast<double>( I_MAX ) / Ki;
	double integral;

	int diff[ 2 ] = { 0 };


public:

	PID(){}
	~PID(){}

	int PlusMinus( double x ){ return ( x < 0 )? -1 : 1; }
	int PIDForward( double CurrentSpeed, double TargetSpeed );
};
};
