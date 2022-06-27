/*
 * pcb.h
 *
 *  Created on: Jul 28, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "PCBqueue.h"
#include "defs.h"
#include "kernSem.h"


enum statusThread{NEW, READY, RUNNING, BLOCKED, TERMINATED, FAILED, IDLE, MAIN};


class ListSemaphores;


class PCB {
private:

	struct PCBelem {
		PCB *p;
		PCBelem *next;
		PCBelem *prev;
		PCBelem():p((PCBelem*)0),next(((PCBelem*)0)),prev((PCBelem*)0){}
	};
	static PCBelem *first, *last;

	static ID gId;
	ID id;
	ID parentID;


public:
	unsigned SP;
	unsigned SS;
	unsigned BP;
	unsigned *stack;


	statusThread status;
	PCBelem *holder;
	Time slice;
	Thread *thread;
	StackSize stackSize;

	PCBqueue* waiters;

	static PCB *running;
	static PCB* idle;
	static PCB* mainThr;
	static PCB* starting;

	friend class KernelSem; //da bi mogla da smanjuje ove parametre neke
	friend class PCBqueue;



	int blockingTime;
	int amountOfBlockingTime;
	int TIMEOUT; // wait vraca 0 ako je isteklo vreme tj ako je timeout==0, a inace 1
	int deblockFrom;



	PCB(Thread *t, StackSize sSize, Time timeslice, PCB *parent);
	void init();
	~PCB();
	Thread* getThread();
	static PCB* getById(ID id);
	ID get_id();
	void set_id(); //for idle thread je 1
	static PCB* getPCB(Thread *t);
	static void run_wrapper_pcb();
	void start_pcb();
	void wait_pcb();
	void set_status(statusThread s);

};




#endif /* PCB_H_ */
