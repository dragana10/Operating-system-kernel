/*
 * kernSem.h
 *
 *  Created on: Aug 11, 2020
 *      Author: OS1
 */

#ifndef KERNSEM_H_
#define KERNSEM_H_

#include "PCBqueue.h"
#include "semaphor.h"
#include "defs.h"
#include "PCB.h"
#include "listSem.h"


class ListSemaphores;


class KernelSem{
	friend class PCB;
public:
	KernelSem (int init=1);
	virtual ~KernelSem ();

	virtual int waitKernSem (Time maxTimeToWait);
	virtual int signalKernSem(int n=0);

	void block(int n);
	void deblock(int n);
	void deblock(PCB* p);

	void informPcbs();

	int valKernSem () const;

	KernelSem* getSemById(int i);
	int getId();



	static ListSemaphores* listOfAllSemaphoresInKernel; //postoji samo jedna lista svih semafora, zajednicka za sve


	static int gIdSem;
	int id;


private:

	int value;
	PCBqueue queueBlockedOnSem; //lista blokiranih pcbova na ovom semaforu; svaki semafor ima poseban ovaj red pcbova

};




#endif /* KERNSEM_H_ */
