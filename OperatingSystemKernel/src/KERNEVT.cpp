/*
 * KERNEVT.cpp
 *
 *  Created on: Aug 26, 2020
 *      Author: OS1
 */

#include "kernevt.h"
#include "intr.h"


static volatile KernelEv* kernEvents[256]={0}; //lista svih eventova
static char volatile happened[256]={0}; //lista koja govori da li je se desio ili ne event


KernelEv::KernelEv(Event *e, IVTNo ivtN){
	this->ivtNumber=ivtN;
	owner=PCB::running;
	parent=e;
	this->blocked=0;
}


KernelEv::~KernelEv(){
	//LOCK();
	happened[this->ivtNumber]=0;
	owner=(PCB*)0;
	parent=(Event*)0;
	//UNLOCK();
}


void KernelEv::waitKernEv (){
	LOCK();

	//cout<<"da wait dada"<<endl;
	//asm cli;

	if(this->owner->status==RUNNING){
		this->blocked=1;
		happened[this->ivtNumber]=1;
		kernEvents[this->ivtNumber]=this;
		this->owner->status=BLOCKED;
		UNLOCK();
		dispatch();
	}
	else UNLOCK();

}

void KernelEv::signalKernEv(){
	//LOCK();
	//cout<<"da signal dada"<<endl;
	//asm cli;
	if(blocked){
		blocked=0;
		happened[this->ivtNumber]=0;
		owner->status=READY;
		Scheduler::put((PCB*)owner);
		//UNLOCK();
		//dispatch();
	}
	//else {
		//UNLOCK();
	//}
}

void IVTEntry::execute(int ivtN, int callOld){
	//cout<<"execute "<<ivtN<<endl;
	//asm cli;
	if(callOld) IVTEntry::call_old_routine(ivtN);

	if(happened[ivtN]) ((KernelEv*)kernEvents[ivtN])->signalKernEv();
	else if(ivtN==9 && !callOld) IVTEntry::call_old_routine(ivtN);

	dispatch();
}

