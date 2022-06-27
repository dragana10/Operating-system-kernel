/*
 * PCBqueue.cpp
 *
 *  Created on: Jul 30, 2020
 *      Author: OS1
 */


#include "pcb.h"


PCBqueue::PCBqueue(){
	size=0;
	firstQue=lastQue=(PCBqelem*)0;
}

void PCBqueue::put(PCB* p){

	PCBqelem* h;
	safe(h=new PCBqelem());
	if(!h)return;
	if(firstQue){
		h->prev=lastQue;
		lastQue->next=h;
		lastQue=h;
	}
	else{
		h->prev=(PCBqelem*)0;
		firstQue=lastQue=h;
	}
	lastQue->next=(PCBqelem*)0;
	lastQue->pcb=p;
	size+=1;

}
PCB* PCBqueue::pop(){
	//cout<<"\n pop PCBqueue \n";
	//asm cli;

	if(firstQue){
		PCBqelem* he;
		he=firstQue;
		PCB* p=firstQue->pcb;
		firstQue=firstQue->next;
		firstQue->prev=(PCBqelem*)0;
		delete he;
		size--;

		return p;
	}
	else {

		return (PCB*)0;
	}

}

PCBqueue::~PCBqueue(){
	//cout<<"\n destr PCBqueue \n";
	//asm cli;
	while(firstQue){
		PCBqelem* h=firstQue;
		h->prev=(PCBqelem*)0;
		firstQue=firstQue->next;
		delete h;
		size-=1;
	}
}

int PCBqueue::isEmpty(){
	//cout<<"\n isEmpty PCBqueue \n";
	//asm cli;
	if(!firstQue)return 1;
	else return 0;
}

PCB* PCBqueue::getPcbByIndex(int i){
	PCBqelem* curr=firstQue;

	while(curr != ((PCBqelem*)0)){
		if(i==0) {
			return curr->pcb;
		}
		curr=curr->next;
		i--;
	}

}

PCB* PCBqueue::takeFromTheMiddle(PCB* p){

	//cout<<"takeFromTheMiddle \n";
	//asm cli;

	if(size<=0)return (PCB*)0;

	int i=p->get_id();

	PCBqelem* curr=firstQue;

	int fId=firstQue->pcb->get_id();
	int lId=lastQue->pcb->get_id();

	if(i==fId) return pop();
	if(i==lId){ //uzimam posl
		PCBqelem* h=lastQue;
		PCB* pp=h->pcb;
		lastQue=lastQue->prev;
		lastQue->next=(PCBqelem*)0;
		size--;
		delete h;

		//cout<<"oslobadjam pcb broj "<<pp->get_id()<<endl;
		//asm cli;
		return pp;
	}
	while(curr != ((PCBqelem*)0)){//uzimam iz sredine
		if(curr->pcb->get_id()==i){
			PCB* ppp=curr->pcb;
			PCBqelem* pr=curr->prev;
			PCBqelem* ne=curr->next;
			pr->next=ne;
			ne->prev=pr;
			size--;
			delete curr;

			//cout<<"oslobadjam pcb broj "<<ppp->get_id()<<endl;
			//asm cli;
			return ppp;
		}
		else {

			curr=curr->next;
		}
	}

}

void PCBqueue::bind_the_list(PCB* p){

	if(size<=0)return;

		int i=p->get_id();

		PCBqelem* curr=firstQue;

		int fId=firstQue->pcb->get_id();
		int lId=lastQue->pcb->get_id();

		if(i==fId) {
			if(firstQue){
				PCBqelem* he;
				he=firstQue;
				firstQue=firstQue->next;
				firstQue->prev=(PCBqelem*)0;
				size--;

				return;
			}
			else {

				return;
			}
		}
		if(i==lId){ //uzimam posl
			PCBqelem* h=lastQue;
			lastQue=lastQue->prev;
			lastQue->next=(PCBqelem*)0;
			size--;
			delete h;

			//cout<<"oslobadjam pcb broj "<<pp->get_id()<<endl;
			//asm cli;
			return;
		}
		while(curr != ((PCBqelem*)0)){//uzimam iz sredine
			if(curr->pcb->get_id()==i){
				PCBqelem* pr=curr->prev;
				PCBqelem* ne=curr->next;
				pr->next=ne;
				ne->prev=pr;
				size--;
				//cout<<"oslobadjam pcb broj "<<ppp->get_id()<<endl;
				//asm cli;

				return;
			}
			else curr=curr->next;

		}

}

int PCBqueue::sizeM() const {
	return (int)size;
}

void PCBqueue::ispisiListu(){
	PCBqelem* curr=firstQue;
	while(curr!= (PCBqelem*)0){
		//cout<<" "<<curr->pcb->get_id()<<" "<<" (moj prethodnik je "<<curr->prev->pcb->get_id()<<"), ";
		//asm cli;
		curr=curr->next;
	}
	//cout<<"\n";
	//asm cli;
}




