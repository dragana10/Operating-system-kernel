/*
 * defs.cpp
 *
 *  Created on: Jul 31, 2020
 *      Author: OS1
 */

#include"defs.h"

volatile unsigned int lockFlag=FLAG_NOT_LOCKED;

volatile int context_switch_on_demand = NOT_EXPLICIT;

volatile int EXIST_SEM_LIST=0;

//volatile int EXIST_KERNEL_EV=0;

