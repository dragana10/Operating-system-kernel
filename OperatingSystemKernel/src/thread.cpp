/*
 * thread.cpp
 *
 *  Created on: Jul 30, 2020
 *      Author: OS1
 */
#include "pcb.h"

extern interrupt timer();

Thread::Thread (StackSize stackSize, Time timeSlice){
	safe(myPCB= new PCB(this,stackSize, timeSlice, PCB::running));
	if(!myPCB || myPCB->status==FAILED){
		safe(delete myPCB);
		myPCB=(PCB*)0;
	}
	//cout<<"kreiram nit broj "<<this->getId()<<endl;
	//asm cli;
}


void Thread::start(){
	if(!myPCB)return;
	myPCB->start_pcb();
}


void Thread::waitToComplete(){
	//cout<<"waitToComplete \n";
	//asm cli;
	if(myPCB)myPCB->wait_pcb();
	//return;
}


Thread::~Thread(){

	//if(!myPCB)return;
	waitToComplete();
	safe(delete myPCB);
	myPCB=(PCB*)0;

}


ID Thread::getId(){
	if(!myPCB)return -1;
	return myPCB->get_id();
}


ID Thread::getRunningId(){
	return PCB::running->get_id();
}


Thread * Thread::getThreadById(ID id){
	return PCB::getById(id)->getThread();
}

void dispatch (){
	lockSt()
	context_switch_on_demand=EXPLICIT;
	timer();
	unlockSt()
}
