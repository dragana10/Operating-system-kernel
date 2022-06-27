/*
 * kernel.h
 *
 *  Created on: Aug 4, 2020
 *      Author: OS1
 */

#ifndef KERNEL_H_
#define KERNEL_H_
#include "thread.h"


// deklaracija nove prekidne rutine
void interrupt timer();


extern userMain(int argc, char* argv[]);


static void initialize_ivt();
static void restore_ivt();


class MainThread:public Thread{
private:
	int argc;
	char** argv;
public:
	int toReturn;
	MainThread(int argc, char** argv);

	~MainThread();
	virtual void run();
	int get_return_code();
};


class IdleThread: public Thread{
public:
	IdleThread();

	~IdleThread();
protected:
	virtual void run();
};


void boot_the_system(int argc, char* argv[]);
int get_main_res();
void startUp();
void initialize();


#endif /* KERNEL_H_ */
