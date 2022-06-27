/*
 * kernevt.h
 *
 *  Created on: Aug 26, 2020
 *      Author: OS1
 */

#ifndef KERNEVT_H_
#define KERNEVT_H_

#include "event.h"
#include "pcb.h"
#include "defs.h"
#include "SCHEDULE.H"
#include <dos.h>

class KernelEv{
public:
	KernelEv(Event *e, IVTNo ivtN);
	~KernelEv();

	void waitKernEv();
	void signalKernEv();

	PCB* owner;
	IVTNo ivtNumber;
	Event* parent;
	int volatile blocked;


	friend class IVTEntry;
};



#endif /* KERNEVT_H_ */
