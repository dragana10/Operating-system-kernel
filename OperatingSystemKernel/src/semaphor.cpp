/*
 * semaphor.cpp
 *
 *  Created on: Aug 11, 2020
 *      Author: OS1
 */


#include "semaphor.h"
#include "kernSem.h"
#include "defs.h"

Semaphore::Semaphore (int init){
	LOCK();
	this->myImpl=new KernelSem(init);
	UNLOCK();
}
Semaphore::~Semaphore (){
	//LOCK();
	delete this->myImpl;
	//UNLOCK();
}

int Semaphore::wait (Time maxTimeToWait){
	return this->myImpl->waitKernSem(maxTimeToWait);
}
int Semaphore::signal(int n){
	return this->myImpl->signalKernSem(n);
}

int Semaphore::val () const{
	return this->myImpl->valKernSem();
}

