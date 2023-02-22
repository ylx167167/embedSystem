#include "/usr/local/include/pbc/pbc.h"
#include "/usr/local/include/pbc/pbc.h"
#include "encrypt.h"
#include "LSSS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void matrixMul(element_t *lambda, element_t *temp, element_t *y, MSP *msp)
{
	int i, j = 0; // the index for the for-loop
	for (i = 0; i < msp->rows; i++)
	{
		element_set0(lambda[i]);
		for (j = 0; j < msp->cols; j++)
		{
			element_set0(temp[j]);
			element_mul_si(temp[j], y[j], msp->matrix[i][j]);
			element_add(lambda[i], lambda[i], temp[j]);
		}
		// element_printf("lambda[%d] = %B\n",i,lambda[i]);
	}
}
void encrypt(element_t message, pairing_t pairing, MSP *msp)
{
	FILE *fEGG = fopen("publicKey/eGG.key", "r"); // open the file of publicKey/eGG.key
	FILE *fG = fopen("publicKey/g.key", "r");	  // open the file of publicKey/g.key
	FILE *fGA = fopen("publicKey/gA.key", "r");	  // open the file of publicKey/gA.key
	FILE *fH;									  // open the file of publicKey/h.key
	FILE *fMSK = fopen("MSK/msk.key", "r");		  // open the file of MSK/msk.key
	element_t eGG;
	element_t g;			// g
	element_t gA;			// g^a
	element_t h[msp->rows]; // attribute h
	int i, j = 0;			// the index of following for-loop
	// initialize the element
	element_init_GT(eGG, pairing);
	element_init_G2(g, pairing);
	element_init_G2(gA, pairing);
	char hCmd[100];			// the command line of attribute h
	char attrName[2];		// the name of the attributes
	memset(hCmd, 0, 100);	// initialize the hCmd
	memset(attrName, 0, 2); // initialize the attrName
	for (i = 0; i < msp->rows; i++)
	{
		strcpy(hCmd, "publicKey/h");
		sprintf(attrName, "%c", msp->label[i]);
		strcat(hCmd, attrName);
		strcat(hCmd, ".key");
		fH = fopen(hCmd, "r");
		element_init_G2(h[i], pairing);
		element_fread(fH, "%s %s\n", &h[i], 10);
		memset(attrName, 0, 2);
		fclose(fH);
		//	element_printf("h[%d] = %B\n",i,h[i]);
	}
	// read the value of element from file
	element_fread(fEGG, "%s %s", &eGG, 10);
	element_fread(fG, "%s %s", &g, 10);
	element_fread(fGA, "%s %s", &gA, 10);
	// close the file poitner
	fclose(fEGG);
	fclose(fG);
	fclose(fGA);
	// start to encrypt the message
	int rows = msp->rows;
	int cols = msp->cols;
	element_t s;			  // the secret of s
	element_t y[cols];		  // the rest of random number y[0]=s
	element_t eGGs;			  // eGGs = e(g,g)^(alpha*s)
	element_t gS;			  // C' = gS= g^s
	element_t lambda[rows];	  // lambda = y*(msp->matrix)
	element_t cipher;		  // C
	element_t cipher_r[rows]; // C_1...C_rows
	element_t d_r[rows];	  // D_1...D_rows
	element_t r[rows];		  // r_1...r_rows
	element_t r_neg[rows];	  //-r_1...-r_rows
	element_t temp[cols];	  // use temp to calculate the lambda = multiplication of v and msp->matrix
	element_t temp_2[rows];	  // use temp_2 to calculate the cipher_r
	element_t temp_3[rows];	  // use temp_3 to calculate the cipehr_r
	// initial the element
	// file pointer to save the ciphertext
	FILE *fC = fopen("cipher/C.cipher", "w");
	FILE *fC_0 = fopen("cipher/C_0.cipher", "w");
	FILE *fC_rows = fopen("cipher/C_rows.cipher", "w");
	FILE *fD_rows = fopen("cipher/D_rows.cipher", "w");
	// generate v = (s,y2..,yn)
	element_init_Zr(s, pairing);
	element_random(s);
	// element_printf("s = %B\n",s);
	for (i = 0; i < cols; i++)
	{
		element_init_Zr(y[i], pairing);
		element_init_Zr(temp[i], pairing);
		if (i == 0)
			element_set(y[0], s);
		else
			element_random(y[i]);
		// element_printf("y[%d] = %B\n",i,y[i]);
	}
	/*test
	element_t test;
	element_t testB;
	element_init_Zr(test,pairing);
	element_init_Zr(testB,pairing);
	element_neg(test,y[1]);
	element_add(testB,test,y[2]);
	element_printf("test: lambda[3] = %B\n",testB);
	test*/
	element_init_GT(eGGs, pairing);		// eGGs = e(g,g)^(alpha*s)
	element_init_G2(gS, pairing);		// gS = g^s
	element_init_GT(cipher, pairing);	// C = Me(g,g)^(alpha*s)
	element_pow_zn(eGGs, eGG, s);		// eGGs = e(g,g)^(alpha*s)
	element_pow_zn(gS, g, s);			// C' = g^s
	element_mul(cipher, eGGs, message); // C = M*e(g,g)^(alpha*s)
	// element_printf("C2 = %B\n", cipher); //解密+加密
	element_fprintf(fC, "%B", cipher);
	element_fprintf(fC_0, "%B", gS);

	// test
	element_t egg;
	element_t final;
	element_t test2;
	element_t test3;
	element_t test4;
	element_t test5;
	element_t test6;
	element_t msk;
	element_init_G2(test6, pairing);
	element_init_Zr(test5, pairing);
	element_init_Zr(test4, pairing);
	element_init_Zr(test3, pairing);
	element_init_Zr(final, pairing);
	element_init_GT(egg, pairing);
	element_init_GT(test2, pairing);
	element_init_G2(msk, pairing);
	element_fread(fMSK, "%s %s", &msk, 10);
	element_pow_zn(egg, eGG, s);
	// element_printf("eGGalphaS = %B\n",egg);

	// test
	for (i = 0; i < rows; i++)
	{
		element_init_Zr(lambda[i], pairing); // lambda_1...lambda_rows
		element_init_Zr(r[i], pairing);		 // r_1...r_rows
		element_init_Zr(r_neg[i], pairing);
		element_init_G2(d_r[i], pairing);	   // D_1...D_rows
		element_init_G2(cipher_r[i], pairing); // C_1...C_rows
		element_init_G2(temp_2[i], pairing);
		element_init_G2(temp_3[i], pairing);
		element_set0(lambda[i]);
		element_random(r[i]);
		element_pow_zn(d_r[i], g, r[i]);
		element_neg(r_neg[i], r[i]);
		element_fprintf(fD_rows, "%B\n", d_r[i]);
		element_set0(temp_2[i]);
		element_set0(temp_3[i]);
	}
	matrixMul(lambda, temp, y, msp); // generate lambda 实现线性秘密共享
	for (i = 0; i < rows; i++)
	{
		element_pow_zn(temp_2[i], gA, lambda[i]);
		element_pow_zn(temp_3[i], h[i], r_neg[i]);
		element_mul(cipher_r[i], temp_2[i], temp_3[i]);
		element_fprintf(fC_rows, "%B\n", cipher_r[i]);
	} // cipehr_r = (g^(a*lambda))*(h^-r)

	// close the file pointer
	fclose(fC);
	fclose(fC_0);
	fclose(fD_rows);
	fclose(fC_rows);
	// clear the element
	element_clear(eGG);
	element_clear(g);
	element_clear(gA);
	element_clear(eGGs);
	element_clear(gS);
	for (i = 0; i < rows; i++)
	{
		element_clear(h[i]);
		element_clear(lambda[i]);
		element_clear(r[i]);
		element_clear(r_neg[i]);
		element_clear(d_r[i]);
		element_clear(cipher_r[i]);
		element_clear(temp_2[i]);
	}

	for (i = 0; i < cols; i++)
	{
		element_clear(y[i]);
		element_clear(temp[i]);
	}

	return;
}