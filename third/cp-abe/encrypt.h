#ifndef _ENCRYPT_H
#define _ENCRYPT_H
#include "/usr/local/include/pbc/pbc.h"
#include "LSSS.h"
void encrypt(element_t message,pairing_t pairing,MSP *msp);//the function to encrypt the message
void matrixMul(element_t *lambda,element_t *temp,element_t *y,MSP *msp);//the multiplication of matrix
#endif