/*
 * defs.h
 *
 *  Created on: Jul 28, 2020
 *      Author: OS1
 */

#ifndef DEFS_H_
#define DEFS_H_

#include"thread.h"

#define FLAG_LOCKED 0
#define FLAG_NOT_LOCKED	1

#define EXPLICIT 1
#define NOT_EXPLICIT 0

#define UNLIMITED 0
#define LIMITED 1

extern volatile unsigned int lockFlag;
extern volatile int context_switch_on_demand;
extern volatile int EXIST_SEM_LIST;
//extern volatile int EXIST_KERNEL_EV;

#define NULLPTR (void*)0

#define lockSt() {\
		asm pushf;\
		asm cli;\
}
#define unlockSt() {\
		asm popf;\
}

#define safe(K){\
	asm pushf;\
	asm cli;\
	K;\
	asm popf;\
}

#define LOCK() { \
	lockFlag=FLAG_LOCKED;\
}

#define UNLOCK() {\
	lockFlag=FLAG_NOT_LOCKED;\
}

#endif /* DEFS_H_ */
