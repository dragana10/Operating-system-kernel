/*
 * KSEM.CPP
 *
 *  Created on: Aug 15, 2020
 *      Author: OS1
 */


#include "kernSem.h"
#include "SCHEDULE.H"


ListSemaphores* KernelSem::listOfAllSemaphoresInKernel=new ListSemaphores();
int KernelSem::gIdSem=0;


KernelSem::KernelSem (int init):value(init){
	this->queueBlockedOnSem=PCBqueue();

	this->id=++gIdSem;
	//cout<<id<<" "<<gIdSem<<endl;
	//asm cli;

	this->listOfAllSemaphoresInKernel->add(this);
	if(EXIST_SEM_LIST==0)EXIST_SEM_LIST=1;
}
KernelSem::~KernelSem (){
	LOCK();
	if(queueBlockedOnSem.sizeM() > 0){  //sve pcbove oznacim kao spremne
			PCBqueue::PCBqelem* curr=queueBlockedOnSem.firstQue;
			while( curr != ((PCBqueue::PCBqelem*)0) ){
				PCBqueue::PCBqelem* h=curr;
				PCB* free=h->pcb;
				free->status=READY;
				Scheduler::put(free);
				curr=curr->next;
				delete h;
			}
		}
	UNLOCK();
}


int KernelSem::waitKernSem (Time maxTimeToWait){
	LOCK();

	int x;
	value--;

	//cout<<PCB::running->get_id()<<" uradio wait, val je "<<value<<endl;
	//asm cli;

	if(maxTimeToWait==0){ //blokiram na neograniceno

		if(value<0){
			block(0);

			UNLOCK();
			dispatch();

			if(PCB::running->TIMEOUT==0 && PCB::running->deblockFrom==this->id)return 0;
			else return 1;
		}
		else{
			UNLOCK();

			if(PCB::running->TIMEOUT==0 && PCB::running->deblockFrom==this->id)return 0;
			else return 1;
		}
	}

	else{ //blokiram na ograniceno

		if(value<0){
			block(maxTimeToWait);

			UNLOCK();
			dispatch();

			if(PCB::running->TIMEOUT==0 && PCB::running->deblockFrom==this->id)return 0;
			else return 1;
		}
		else{
			UNLOCK();

			if(PCB::running->TIMEOUT==0 && PCB::running->deblockFrom==this->id)return 0;
			else return 1;
		}

	}
}
int KernelSem::signalKernSem(int n){
	LOCK();

	//cout<<this->id<<" signal, val je "<<value<<"\n";
	//asm cli;

	if(n<0){
		UNLOCK();
		return n;
	}

	else if(n==0){
		this->value+=1;

		if(value<=0){
			deblock(1);
			UNLOCK();
			//cout<<PCB::running->get_id()<<" uradio signal, val je "<<value<<endl;
			//asm cli;
			return 0;
		}
		else{
			UNLOCK();

			//cout<<" anlokujemo i vracamo 0 u sign  "<<value<<endl;
			//asm cli;
			return 0;
		}
	}
	else{
		value+=n;
		if(value<=0){ //odblokiram n niti
			deblock(n);
			UNLOCK();
			return n;
		}
		else{ //odblokiram n-val
			deblock(n-value);
			UNLOCK();
			return (n-value);
		}
	}
}



int KernelSem::valKernSem () const{
	return value;
}


void KernelSem::block(int n){

	//cout<<PCB::running->get_id()<<" se blokira "<<endl;
	//asm cli;

	if(n==0){ //blokiram 1 na neograniceno
		//cout<<this->id<<"----blokiram(neogr) nit broj "<<PCB::running->get_id()<<" val je "<<this->value<<endl;
		//asm cli;

		PCB::running->blockingTime=UNLIMITED;
		PCB::running->amountOfBlockingTime=0;
		PCB::running->status=BLOCKED;
		PCB::running->TIMEOUT=1;
		PCB::running->deblockFrom=0;
		this->queueBlockedOnSem.put((PCB*)PCB::running);

	}

	else{ //blokiram na ograniceno

		//cout<<this->id<<"----blokiram(ogr) nit broj "<<PCB::running->get_id()<<" val je "<<this->value<<" na "<<n<<endl;
		//asm cli;
		PCB::running->blockingTime=1;
		PCB::running->amountOfBlockingTime=n;
		PCB::running->status=BLOCKED;
		PCB::running->TIMEOUT=1;
		PCB::running->deblockFrom=0;
		this->queueBlockedOnSem.put((PCB*)PCB::running);
	}
}


void KernelSem::deblock(int n){

	//cout<<this->id<<"-------------deblokiram, runn je "<<PCB::running->get_id()<<" val je "<<this->value<<endl;
	//asm cli;
	int i=0;
	for(i;i<n;i++){
		PCB* h;
		//cout<<"popujem iz unlimited ";
		//asm cli;
		h=this->queueBlockedOnSem.pop();
		//cout<<PCB::running->get_id()<<" se deblokira "<<endl;
		//asm cli;
		//cout<<h->get_id()<<" je deblokirana sa sema "<<this->id<<endl;
		//asm cli;
		h->status=READY;
		PCB::running->deblockFrom=0;
		//cout<<"deblokiram "<<h->get_id()<<endl;
		//asm cli;
		Scheduler::put(h);
	}
}


void KernelSem::deblock(PCB* p){
	int i=0;

	for(i;i<this->queueBlockedOnSem.sizeM();i++){
		if(p->get_id()==this->queueBlockedOnSem.getPcbByIndex(i)->get_id()){
			PCB* h;
			h=this->queueBlockedOnSem.takeFromTheMiddle(this->queueBlockedOnSem.getPcbByIndex(i));
			h->blockingTime=UNLIMITED;
			h->amountOfBlockingTime=0;
			h->deblockFrom=this->id;
			//cout<<this->id<<"(ISTEK)---------deblokiram "<<h->get_id()<<" val je "<<this->value<<endl;
			//asm cli;
			h->status=READY;Scheduler::put(h);
			return;
		}
		else continue;
	}
}


void KernelSem::informPcbs() {
	PCBqueue::PCBqelem* curr=queueBlockedOnSem.firstQue;


	while( curr != ((PCBqueue::PCBqelem*)0) ){

		PCB* currPcb=curr->pcb;

		if(currPcb->blockingTime==1) {

				(currPcb->amountOfBlockingTime)-=1;

				//cout<<currPcb->get_id()<<" amount "<<currPcb->amountOfBlockingTime<<endl;
				//asm cli;

				if(currPcb->amountOfBlockingTime==0 && currPcb->get_id()==queueBlockedOnSem.firstQue->pcb->get_id()){
					if(curr->next)curr=curr->next;
					deblock(currPcb);
					currPcb->TIMEOUT=0;
					this->value+=1;
					continue;
				}
				else if(currPcb->amountOfBlockingTime==0){
					deblock(currPcb);
					currPcb->TIMEOUT=0;
					this->value+=1;
				}

		}
		curr=curr->next;
	}

}


int KernelSem::getId(){
	return this->id;
}
