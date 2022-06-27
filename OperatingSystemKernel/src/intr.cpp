/*
 * intr.cpp
 *
 *  Created on: Sep 16, 2020
 *      Author: OS1
 */

#include "intr.h"
#include "defs.h"
#include <dos.h>
#include "kernevt.h"

static ptrToIntrRout old_interrupt_routines[256];
static char exists_old_value[256]={0};


IVTEntry::IVTEntry(IVTNo ivtN, ptrToIntrRout intr){
	this->numberOfEntry=ivtN;
	this->new_intr_routine=intr;
}


void IVTEntry::call_old_routine(char ivtN){
	//cout<<"call_old_routine "<<ivtN<<endl;
	//asm cli;
	old_interrupt_routines[ivtN]();
}


void IVTEntry::restore_the_old(char ivtN){
	if(exists_old_value[ivtN]) {
#ifndef BCC_BLOCK_IGNORE
		setvect(ivtN, old_interrupt_routines[ivtN]);
#endif
	}
}


int /*IVTEntry::*/set_new_routine(int ivtN, ptrToIntrRout r){
	lockSt();
	if((int)ivtN==8){
		unlockSt();
		return 0;
	}
#ifndef BCC_BLOCK_IGNORE
	old_interrupt_routines[ivtN]=getvect(ivtN);
#endif
	exists_old_value[ivtN]=1;
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtN, r);
#endif
	unlockSt();
	return 1;
}


int set_routine(int ivtN, ptrToIntrRout r){
	return set_new_routine(ivtN,r);
}
