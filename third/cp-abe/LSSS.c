#include "/usr/local/include/pbc/pbc.h"
#include "LSSS.h"
#include <stdio.h>
#include <stdlib.h>	
#include <string.h>
void mspInit(MSP *msp,int rows,int cols){
	msp -> matrix = malloc(rows * sizeof(int*));
	int i = 0;//the index of the following loop
	for(i; i < rows; i++){
		(msp -> matrix)[i] = calloc(cols, sizeof(int));
	}
	msp -> label = calloc(rows,sizeof(char));
	msp -> rows = rows;
	msp -> cols = cols;
	return ;
}//initialize the msp
void mspClear(MSP *msp){
	int i = 0;
	for(i = 0;i < msp->rows;i++){
		free((msp->matrix)[i]);
	}

	free(msp -> matrix);
	free(msp -> label);
	return;
}//clear the msp
void mspSetup(MSP *msp){
	FILE *fAttr = fopen("attribute.dat","r");
	int i,j = 0; //the index of the following loop
	int rows = 0;//the rows of the matrix
	int cols = 0;//the cols of the matrix
	fscanf(fAttr,"%d %d\n",&rows,&cols);
	mspInit(msp,rows,cols);
	for( i = 0; i < rows; i++){
		fscanf(fAttr,"%c\n",&msp->label[i]);
	}
	
	//label the row of matrix
	
	for( i = 0; i < rows; i++){
		for( j = 0; j < cols; j++){
			fscanf(fAttr,"%d ",&msp->matrix[i][j]);
		}
	}
	
	return;
}


