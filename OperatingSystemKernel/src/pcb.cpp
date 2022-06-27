/*
 * pcb.cpp
 *
 *  Created on: Jul 29, 2020
 *      Author: OS1
 */
#include <dos.h>
#include"pcb.h"
#include"defs.h"
#include "SCHEDULE.H"
#include "PCBqueue.h"


ID PCB::gId=0; //main thread==0; idle thread==1
PCB::PCBelem* PCB::first=(PCBelem*)0;
PCB::PCBelem* PCB::last=(PCBelem*)0;
PCB* PCB::running=(PCB*)0;
PCB* PCB::idle=(PCB*)0;
PCB* PCB::mainThr=(PCB*)0;
PCB* PCB::starting=(PCB*)0;


class KernelSem;
class PCBqueue;


PCB::PCB(Thread *t, StackSize sSize, Time tslice, PCB *parent){
	this->id=-1;
	this->thread=t;
	this->slice=tslice;
	if(parent)this->parentID=parent->id;
	else this->parentID=-1;

	if(sSize>65536)this->stackSize=65536;
	else if(sSize<1024)this->stackSize=1024;
	else this->stackSize=sSize;
	this->stackSize=(this->stackSize)/sizeof(unsigned);
	safe(stack=new unsigned[stackSize]);
	if(!stack){
		this->status=FAILED;
		return;
	}
	stack[stackSize-1]=0x200;
#ifndef BCC_BLOCK_IGNORE
	this->stack[stackSize-2]=FP_SEG(run_wrapper_pcb);
	this->stack[stackSize-3]=FP_OFF(run_wrapper_pcb);
	this->SS=FP_SEG(this->stack+this->stackSize-12);
	this->SP=FP_OFF(this->stack+this->stackSize-12);
	this->BP=this->SP;
#endif

	this->status=NEW;
	this->holder=NULLPTR;
	this->TIMEOUT=1;
	this->blockingTime=UNLIMITED;
	this->amountOfBlockingTime=0;
	this->deblockFrom=0;
	this->waiters=new PCBqueue();
	init();
}


void PCB::init(){
	id=gId++;


	safe(holder=new PCBelem());

	if(!holder){
		status=FAILED;
		return;
	}

	holder->p=this;

	if(first){
		holder->prev=last;
		last=last->next=holder;
		last->next=((PCBelem*)0);
	}
	else{
		first=last=holder;
	}
}

PCB::~PCB(){
	LOCK();


	if(!holder)return;
	if(stack)safe(delete[] stack);
	stack=NULLPTR;
	thread->myPCB=(PCB*)0;
	PCBelem* p=(PCBelem*)0;
	PCBelem* n=(PCBelem*)0;
	if(holder->prev)p=holder->prev;
	if(holder->next)n=holder->next;
	if(p)p->next=holder->next;
	if(n)n->prev=holder->prev;
	if(holder==first)first=first->next;
	if(holder==last)last=last->prev;
	safe(delete holder);


	//cout<<"brisem nit broj "<<this->get_id()<<endl;

	UNLOCK();
}


Thread* PCB::getThread(){return this->thread;}


PCB* PCB::getById(ID id){

	PCBelem *curr=first;
	while(curr){
		if(curr->p->id==id)return curr->p;
		curr=curr->next;
	}
	return NULLPTR;

}


ID PCB::get_id(){
	return this->id;
}


PCB* PCB::getPCB(Thread *t){return t->myPCB;}


void PCB::run_wrapper_pcb(){
	running->thread->run();

	LOCK();

	running->status=TERMINATED;

	//sve niti koje su cekale u redu da se zavrsi ova se oslobode i stave u red spremnih

	while(PCB::running->waiters->sizeM()>0){
		PCB* p=PCB::running->waiters->pop();
		p->status=READY;
		Scheduler::put(p);

	}


	UNLOCK();
	dispatch();
}




void PCB::start_pcb(){
	safe(
			if(this->status==NEW && this->status!= IDLE){
				this->status=READY;
				Scheduler::put(this);
			}
	);
}


void PCB::wait_pcb(){
	LOCK();


	//ako je nit zavrsila svoje izvrsavanje-->NE TREBA DA SE CEKA
	//NE TREBA NIT DA CEKA SAMA SEBE
	if(this->status==TERMINATED || PCB::running==this || this==PCB::starting || this->status==IDLE ){
		UNLOCK();
		return;
	}

	PCB::running->status=BLOCKED;
	waiters->put(PCB::running);
	//cout<<"stavio sam u waiters";
	//asm cli;

	UNLOCK();

	dispatch();
}

void PCB::set_status(statusThread s){
	this->status=s;
}



