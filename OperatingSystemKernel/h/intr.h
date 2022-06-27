/*
 * intr.h
 *
 *  Created on: Sep 16, 2020
 *      Author: OS1
 */

#ifndef INTR_H_
#define INTR_H_


typedef void interrupt (*ptrToIntrRout)(...);
typedef unsigned char IVTNo;


class IVTEntry{
public:
	IVTEntry(IVTNo ivtN, ptrToIntrRout intr);

	IVTNo numberOfEntry;
	ptrToIntrRout new_intr_routine;
	ptrToIntrRout old_intr_routine;

	static void execute(int ivtN, int callOld);
	static void call_old_routine(char ivtN);
	static void restore_the_old(char ivtN);


	friend class KernelEv;
};

int set_new_routine(int ivtN, ptrToIntrRout r);
int set_routine(int ivtN, ptrToIntrRout r);
#define PREPAREENTRY(ivtN,callOld)\
	class Entry##ivtN : IVTEntry{\
		private:\
			int is_interrupt_routine_set_##ivtN;\
		public:\
		static interrupt void generate_a_definition(...){\
			execute(ivtN,callOld);\
		}\
	};\
	int is_interrupt_routine_set_##ivtN=set_routine(ivtN,Entry##ivtN::generate_a_definition);




#endif /* INTR_H_ */
