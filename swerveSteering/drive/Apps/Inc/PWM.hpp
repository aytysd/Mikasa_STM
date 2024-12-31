/*
 * PWM.hpp
 *
 *  Created on: Jul 17, 2022
 *      Author: ayato
 */

#ifndef INC_PWM_HPP_
#define INC_PWM_HPP_

#include "math.h"

#include "tim.h"

namespace NITNC
{

class PWM
{
private:

public:

	void CW( double PWM );
	void CCW( double PWM );
	void STOP( void );
};
};



#endif /* INC_PWM_HPP_ */
