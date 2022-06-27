/*
 * kernel.cpp
 *
 *  Created on: Aug 4, 2020
 *      Author: OS1
 */
#include "kernel.h"
#include "defs.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "listSem.h"


static MainThread* mt;
static IdleThread* it;
static int val=-1;


class PCB;
class ListSemaphores;


ListSemaphores* ptrListSem=(ListSemaphores*)0;


extern void tick();


static volatile unsigned int tsp;
static volatile unsigned int tss;
static volatile unsigned int tbp;

volatile Time cntr = defaultTimeSlice;


// stara prekidna rutina
unsigned oldTimerOFFn, oldTimerSEGn;


// postavlja novu prekidnu rutinu
void initialize_ivt(){
	asm{
			cli
			push es
			push ax

			mov ax,0
			mov es,ax // es = 0

			// pamti staru rutinu
			mov ax, word ptr es:0022h
			mov word ptr oldTimerSEGn, ax
			mov ax, word ptr es:0020h
			mov word ptr oldTimerOFFn, ax

			// postavlja novu rutinu
			mov word ptr es:0022h, seg timer
			mov word ptr es:0020h, offset timer

			// postavlja staru rutinu na int 60h
			mov ax, oldTimerSEGn
			mov word ptr es:0182h, ax
			mov ax, oldTimerOFFn
			mov word ptr es:0180h, ax

			pop ax
			pop es
			sti
		}
}


// vraca staru prekidnu rutinu
void restore_ivt(){
	asm {
			cli
			push es
			push ax

			mov ax,0
			mov es,ax

			mov ax, word ptr oldTimerSEGn
			mov word ptr es:0022h, ax
			mov ax, word ptr oldTimerOFFn
			mov word ptr es:0020h, ax

			pop ax
			pop es
			sti
		}
}


void interrupt timer(){
	//cout<<"timer";
	//asm cli;

	if(context_switch_on_demand==0){
		asm int 60h;
		tick();
		cntr--;

	}


	if(EXIST_SEM_LIST==1 && context_switch_on_demand==0) {
		ptrListSem=KernelSem::listOfAllSemaphoresInKernel;
		ptrListSem->informSemaphore();
	}


	if ( cntr == 0 || context_switch_on_demand==1) {
		if(lockFlag==FLAG_NOT_LOCKED){

			//cout<<lockFlag<<" je "<<" lockFlag "<<PCB::running->get_id()<<"   menjam kont1  ";
			//asm cli;

			asm {// cuva sp
				mov tsp, SP
				mov tss, SS
				mov tbp, BP
			}

			PCB::running->SP = tsp;
			PCB::running->SS = tss;
			PCB::running->BP = tbp;

			//if(cntr==0 && PCB::running->blockingTime==LIMITED)PCB::running->TIMEOUT=0;


			if(PCB::running->status==RUNNING){
				PCB::running->status=READY;
				Scheduler::put((PCB*)PCB::running);
			}
			PCB::running=Scheduler::get();
			if(PCB::running==NULLPTR) (PCB*)PCB::running=(PCB*)PCB::idle;
			else PCB::running->status=RUNNING;


			/*if(PCB::running->get_id()==1){
				cout<<"\n  idle radi hehehe  \n"<<endl;
				asm cli;
			}*/
			//cout<<"  dobio kont2  "<<PCB::running->get_id()<<endl;
			//asm cli;

			tsp = PCB::running->SP;
			tss = PCB::running->SS;
			tbp = PCB::running->BP;

			cntr = PCB::running->slice;
			//PCB::running->TIMEOUT=1;

			asm {
				// restaurira sp
				mov SP, tsp
				mov SS, tss
				mov BP, tbp
			}
			context_switch_on_demand=0;
		}
		else context_switch_on_demand=1;

	}
}


void boot_the_system(int argc, char* argv[]){
	safe(mt=new MainThread(argc,argv));
	safe(it=new IdleThread());

	mt->start();

	PCB* i=PCB::getPCB(it);
	i->set_status(IDLE);
	PCB::idle=i;
	it->start();

	PCB* m=PCB::getPCB(mt);
	m->set_status(MAIN);
	//Scheduler::put(m);
	PCB::mainThr= m;

	initialize_ivt();
	initialize();

	mt->waitToComplete();
	delete PCB::mainThr;
	delete PCB::idle;
}


int get_main_res(){
	val=mt->get_return_code();
	restore_ivt();
	return val;
}


MainThread::MainThread(int argc, char** argv):Thread(){
	this->argc=argc;
	this->argv=argv;
	this->toReturn=-1;
}


void MainThread::run(){
	this->toReturn=userMain(this->argc,this->argv);
}


int MainThread::get_return_code(){
	return this->toReturn;
}


MainThread::~MainThread(){
	delete this->argv;
}


IdleThread::IdleThread():Thread(2048,1){
}


void IdleThread::run(){
	while(1);
}


IdleThread::~IdleThread(){
	delete PCB::idle;
}


void initialize(){
	safe(PCB::starting=PCB::running = new PCB((Thread*)0, 10,1, (void*)0));
	((PCB*)PCB::running)->status=RUNNING;
}

