#include "/usr/local/include/pbc/pbc.h"
#include"abeSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(){
	//define the parameters
	pairing_t pairing;//the pairing
	float pairingTime,pairingPowerTime,pointPowerTime,mulTime,divTime;//the duration time of each operation
	clock_t start,end;//the start and end time of the operations
	element_t g,h,eGH,n,gN;
	//initialize the parameters
	setupSingularPairing(&pairing);
	element_init_G1(g,pairing);
	element_init_G1(gN,pairing);
	element_init_G2(h,pairing);
	element_init_GT(eGH,pairing);
	element_init_Zr(n,pairing);
	element_random(g);
	element_random(h);
	element_random(n);
	int tempA= 100,tempB= 100,tempC = 100;
	//start to record the time duration
	// pairing time
	start = clock();
	element_pairing(eGH,g,h);
	end = clock();
	pairingTime = (float)(end-start)/CLOCKS_PER_SEC;
	//pairing power time
	start = clock();
	element_pow_zn(eGH,eGH,n);
	end = clock();
	pairingPowerTime = (float)(end-start)/CLOCKS_PER_SEC;
	//point power Time
	start = clock();
	element_pow_zn(gN,g,n);
	end = clock();
	pointPowerTime = (float)(end - start)/CLOCKS_PER_SEC;
	//multiplication time
	start = clock();
	//element_mul(gN,g,g);
	tempC = tempA*tempB;
	end = clock();
	mulTime = (float)(end - start)/CLOCKS_PER_SEC;
	//division time
	start = clock();
	//element_div(gN,g,g);
	tempC = tempA/tempB;
	end = clock();
	divTime = (float)(end - start)/CLOCKS_PER_SEC;
	//show the result
	printf("The time of pairing is %f\n",pairingTime);
	printf("The time of pairing poweris %f\n",pairingPowerTime);
	printf("The time of point power is %f\n",pointPowerTime);
	printf("The time of multiplication power is %f\n",mulTime);
	printf("The time of division power is %f\n",divTime);
	
}