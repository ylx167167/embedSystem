#include "/usr/local/include/pbc/pbc.h"
#include "LSSS.h"
#ifndef _ABE_SETUP
#define _ABE_SETUP
void generatePrime(mpz_t *p,int sBit);
//generate prime number
//int generateOrdCurve(pbc_cm_t cm,void *data);
//generate ordinary curve
void setupSingularPairing(pairing_t *pairing);
void setupOrdinaryPairing(pairing_t *pairing);
//setup the pairing
void setup(int attrNo,pairing_t *pairing,MSP *msp);
//the first setup of cp-abe protocol -- setup
#endif
