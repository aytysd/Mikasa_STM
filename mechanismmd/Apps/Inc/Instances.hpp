/*
 * Instances.hpp
 *
 *  Created on: Jul 17, 2022
 *      Author: ayato
 */

#ifndef INC_INSTANCES_HPP_
#define INC_INSTANCES_HPP_


#include "Drive.hpp"
#include "mycan.hpp"
#include "Velocity.hpp"

namespace NITNC
{
extern Drive drive;
extern MyCAN can;
extern Velocity velocity;
};


#endif /* INC_INSTANCES_HPP_ */
