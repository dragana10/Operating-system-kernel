/*
 * main.cpp
 *
 *  Created on: Aug 2, 2020
 *      Author: OS1
 */


#include "kernel.h"


int main(int argc, char* argv[]){

	//cout<<"Krenuo main."<<endl;

	int val;
	val=-1;

	asm cli;
	boot_the_system(argc, argv);
	val=get_main_res();
	asm sti;
	return val;



}
