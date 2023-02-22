#ifndef _LSSS_H
#define _LSSS_H
#include "/usr/local/include/pbc/pbc.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct MonotoneSpanProgram{
	int **matrix;	
	char *label;
	int rows;
	int cols;
}MSP;

int genSecret(int prime);
void mspInit(MSP *msp,int rows,int cols);//initialize the msp
void mspClear(MSP *msp);//clear the msp
void mspSetup(MSP *msp);//setup a msp大

#endif
