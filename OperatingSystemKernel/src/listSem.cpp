/*
 * listSem.cpp
 *
 *  Created on: Aug 13, 2020
 *      Author: OS1
 */

#include "listSem.h"
#include "defs.h"


ListSemaphores::ListSemaphores(){
	size=0;
	firstListSemElem=lastListSemElem=(ListSemElem*)0;
}

void ListSemaphores::add(KernelSem* k){
	ListSemElem* h;
	safe(h=new ListSemElem());
	if(!h)return;
	if(firstListSemElem){
		//ListSemElem* p=lastListSemElem; prethodni je poslednji
		h->prev=lastListSemElem;
		lastListSemElem->next=h; //h je poslednji, uvezem
		lastListSemElem=h;
		//cout<<"sl sem koji dodajem ima id "<<k->getId()<<endl;
		//asm cli;
	}
	else{
		firstListSemElem->prev=(ListSemElem*)0;
		firstListSemElem=lastListSemElem=h;
		//cout<<"prvi sem koji dodajem ima id "<<k->getId()<<endl;
		//asm cli;
	}
	lastListSemElem->next=(ListSemElem*)0;
	lastListSemElem->sem=k;
	//cout<<"dodao sam id  "<<lastListSemElem->sem->getId()<<endl;
	//asm cli;
	size++;
}
KernelSem* ListSemaphores::takeTheFirstOne(){
	if(firstListSemElem){
		ListSemElem* he;
		he=firstListSemElem;
		KernelSem* k=firstListSemElem->sem;
		firstListSemElem=firstListSemElem->next;
		firstListSemElem->prev=(ListSemElem*)0;
		delete he;
		size--;
		return k;
	}
	else return (KernelSem*)0;
}


/*KernelSem* ListSemaphores::takeById(int i){
	if(firstListSemElem){
		int fId=(int)firstListSemElem->sem->getId();
		//cout<<"fid je"<<firstListSemElem->sem->getId()<<endl;
		//sasm cli;
		int lId=lastListSemElem->sem->id;
		cout<<"lId je"<<this->lastListSemElem->sem->getId()<<endl;
		//cout<<"lId je"<<lId<<endl;

		asm cli;
		ListSemElem* curr=firstListSemElem;
		while(curr){
			if(curr->sem->id==i){
				if(curr->sem->id==fId){ //prvi se uzima
					this->takeTheFirstOne();
				}
				else if(curr->sem->id==lId){ //posl se uzima
					ListSemElem* he;
					he=lastListSemElem;
					KernelSem* k=lastListSemElem->sem;
					lastListSemElem=lastListSemElem->prev;
					lastListSemElem->next=(ListSemElem*)0;
					delete he;
					size--;
					return k;
				}
				else { //uzimamo iz sredine
					KernelSem* k=curr->sem;
					ListSemElem* p;
					ListSemElem* n;
					p=curr->prev;
					n=curr->next;
					p->next=n;
					n->prev=p;
					delete curr;
					size--;
					return k;
				}
			}
			else curr=curr->next;
		}
	}
	else return (KernelSem*)0;
}*/


void ListSemaphores::informSemaphore(){
	LOCK();


	int it=0;
	int g=KernelSem::gIdSem;
	ListSemElem* curr=firstListSemElem;

	//cout<<" informSemaphore ";
	//asm cli;

	while(it<g){
		it++;
		//cout<<it<<" je it, a sem id je  "<<curr->sem->getId()<<"g je "<<g<<endl;
	    //asm cli;
		curr->sem->informPcbs();
		curr=curr->next;
	}

	//cout<<"\n zavrseno informisanje \n";
	//asm cli;
	UNLOCK();
}

ListSemaphores::~ListSemaphores(){
	while(firstListSemElem){
		ListSemElem* curr=firstListSemElem;
		curr->prev=(ListSemElem**)0;
		firstListSemElem=firstListSemElem->next;
		delete curr;
		size-=1;
	}
}





