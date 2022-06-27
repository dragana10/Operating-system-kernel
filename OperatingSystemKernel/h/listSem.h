/*
 * listSem.h
 *
 *  Created on: Aug 13, 2020
 *      Author: OS1
 */

#ifndef LISTSEM_H_
#define LISTSEM_H_

#include "semaphor.h"
#include "kernSem.h"

class KernelSem;

class ListSemaphores{


protected:

	struct ListSemElem{
		KernelSem* sem;
		ListSemElem* prev;
		ListSemElem* next;
		ListSemElem():sem((KernelSem*)0){};
	};



public:
	ListSemaphores();
	~ListSemaphores();


	void add(KernelSem* k);
	KernelSem* takeTheFirstOne();
	//KernelSem* takeById(int i);

	ListSemElem *firstListSemElem, *lastListSemElem;
	int size;

	void informSemaphore();

};


#endif /* LISTSEM_H_ */




