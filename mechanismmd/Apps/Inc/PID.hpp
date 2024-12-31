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

#define P_MAX 600
#define I_MAX 300
#define D_MAX 200


class PID
{
private:

	const double Kp = 0.45;
	const double Ki = 0.7;
	const double Kd = 0.001;

	double DeltaT = 0.01;

	double integral;
	int diff[ 2 ] = { 0 };

	int PlusMinus( double x ){ return ( x < 0 )? -1 : 1; }

public:

	PID(){}
	~PID(){}

	int PIDForward( double CurrentSpeed, double TargetSpeed );
};
};
