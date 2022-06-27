/*
 * PCBqueue.h
 *
 *  Created on: Jul 29, 2020
 *      Author: OS1
 */

#ifndef PCBQUEUE_H_
#define PCBQUEUE_H_

class PCB;

class PCBqueue{
protected:
	struct PCBqelem{
		PCB *pcb;
		PCBqelem* next;
		PCBqelem* prev;
		PCBqelem():pcb((PCB*)0){}
	};

public:
	PCBqelem *firstQue, *lastQue;

	PCBqueue();
	void put(PCB* p);
	PCB* pop();
	PCB* takeFromTheMiddle(PCB* p);
	void bind_the_list(PCB* p);
	~PCBqueue();
	int isEmpty();
	int sizeM() const;
	PCB* getPcbByIndex(int i);
	void ispisiListu();

private:
	int size;


};


#endif /* PCBQUEUE_H_ */
