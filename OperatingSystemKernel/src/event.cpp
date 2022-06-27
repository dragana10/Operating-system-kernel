/*
 * event.cpp
 *
 *  Created on: Aug 26, 2020
 *      Author: OS1
 */
#ifndef _event_h_
#define _event_h_


#include "kernevt.h"


Event::Event (IVTNo ivtNo){
	this->myImpl=new KernelEv(this,ivtNo);
}


Event::~Event (){
	if(myImpl)delete this->myImpl;
	myImpl=(KernelEv*)0;
}


void Event::wait (){
	if(myImpl)this->myImpl->waitKernEv();
}

void Event::signal(){
	lockSt();
		this->myImpl->signalKernEv();
	unlockSt();
}



#endif

