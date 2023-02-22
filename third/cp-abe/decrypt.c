#include <pbc/pbc.h>
#include <gmp.h>
#include "decrypt.h"
#include "elementIO.h"
#include "LSSS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
void swap(int *A, int *B)
{
	int temp;
	temp = *A;
	*A = *B;
	*B = temp;
	return;
}
void valueToMessage(char *message, mpz_t message_mpz)
{
	char *c = NULL;
	c = (char *)message_mpz->_mp_d;
	unsigned int count = 0;

	while (*(c + count) != '\0')
	{
		count += 1;
	}
	message[count] = '\0';
	while (count > 0)
	{
		count -= 1;
		message[count] = *c;
		c += 1;
	}
}

void findOmega(MSP *msp, element_t *omega, int attrNo, char *attribute)
{

	int rows = attrNo;
	int cols = msp->cols;
	int sum = 0;
	int k, i, j; // the index of the for-loop
	int tempMatrix[rows][rows * 2];
	int inverseMatrix[rows][rows];
	int identity[rows];
	int result[rows];
	int middle[rows][rows];
	char tmpLabel[2];
	char tmpAttr[2];
	int x = 0;	   // the index of the tempMatrix
	int y = 0;	   // the index of the tempMatrix
	int count = 0; // the count for while-loop
	for (i = 0; i < rows; i++)
	{
		if (i == 0)
			identity[i] = 1;
		else
			identity[i] = 0;
		for (j = 0; j < msp->rows; j++)
		{
			memset(tmpLabel, 0, 2);
			memset(tmpAttr, 0, 2);
			sprintf(tmpLabel, "%c", msp->label[j]);
			sprintf(tmpAttr, "%c", attribute[i]);
			if (strcmp(tmpLabel, tmpAttr) == 0)
			{
				while (count != cols)
				{
					tempMatrix[x][y] = msp->matrix[j][count];
					count++;
					y++;
				}
				count = 0;
				y = 0;
				x++;
			}
		}
	}
	// initialize the target array
	// start to find the inverse matrix
	for (i = 0; i < rows; ++i)
		for (j = 0; j < rows; ++j)
			tempMatrix[i][rows + j] = 0;

	for (i = 0; i < rows; ++i)
		tempMatrix[i][rows + i] = 1;

	for (i = 0; i < rows; ++i)
	{
		if (tempMatrix[i][i] == 0)
			for (j = i + 1; j < rows; ++j)
				if (tempMatrix[j][i] != 0)
				{
					for (k = i; k < rows * 2; ++k)
						swap(&tempMatrix[i][k], &tempMatrix[j][k]);
					break;
				}

		if (tempMatrix[i][i] == 0)
			return;

		double t = tempMatrix[i][i];
		for (k = i; k < rows * 2; ++k)
			tempMatrix[i][k] /= t;

		for (j = 0; j < rows; ++j)
			if (i != j && tempMatrix[j][i] != 0)
			{
				double t = tempMatrix[j][i];
				for (k = i; k < rows * 2; ++k)
					tempMatrix[j][k] -= tempMatrix[i][k] * t;
			}
	}

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < rows; j++)
		{
			inverseMatrix[i][j] = tempMatrix[i][rows + j];
			//		printf("temp[%d][%d] = %d",i,j,inverseMatrix[i][j]);
		}
		//		printf("\n");
	}
	for (i = 0; i < rows; i++)
	{
		result[i] = 0;
		for (j = 0; j < rows; j++)
		{
			result[i] += identity[j] * inverseMatrix[j][i];
		}
		if (result[i] == 0)
		{
			element_set0(omega[i]);
		}
		else if (result[i] == 1)
		{
			element_set1(omega[i]);
		}
		else if (result[i] == -1)
		{
			element_set1(omega[i]);
			element_neg(omega[i], omega[i]);
		}
		// printf("result[%d] = %d\n",i,result[i]);
		// element_printf("omega[%d] = %B\n",i,omega[i]);
	}

	return;
}
void decrypt(pairing_t pairing, MSP *msp, element_t message, int attrNo, char *attribute, char *userName)
{
	// file pointer to read the ciphertext and private key
	FILE *fG = fopen("publicKey/g.key", "r");
	FILE *fGA = fopen("publicKey/gA.key", "r");
	FILE *fC = fopen("cipher/C.cipher", "r");
	FILE *fC_0 = fopen("cipher/C_0.cipher", "r");
	FILE *fC_rows = fopen("cipher/C_rows.cipher", "r");
	FILE *fD_rows = fopen("cipher/D_rows.cipher", "r");
	FILE *fK;
	FILE *fL;
	FILE *fKx;
	char kCmd[100];
	char lCmd[100];
	char kxCmd[100];
	memset(kCmd, 0, 100);
	memset(lCmd, 0, 100);
	memset(kxCmd, 0, 100);
	strcpy(kCmd, userName);
	strcat(kCmd, "/K.key");
	strcpy(lCmd, userName);
	strcat(lCmd, "/L.key");
	strcpy(kxCmd, userName);
	strcat(kxCmd, "/Kx.key");
	fK = fopen(kCmd, "r");
	fL = fopen(lCmd, "r");
	fKx = fopen(kxCmd, "r");
	int i = 0, j = 0;	  // the index for the following for-loop
	int rows = msp->rows; // the rows of msp
	element_t gA;		  // gA = g^a
	element_t g;		  // g
	element_t C;		  // C = M*e(g,g)^(aplha*s)
	element_t gS;		  // gS = g^s
	element_t cipher_r[rows];
	element_t d_r[rows];
	element_t K;		   // private key K
	element_t L;		   // private key L
	element_t Kx[attrNo];  // private key for attribute A user
	element_t omega[rows]; // omega *lambda = s
	element_init_G2(g, pairing);
	element_init_G2(gA, pairing);
	element_init_GT(C, pairing);
	element_init_G2(gS, pairing);
	element_init_G2(K, pairing);
	element_init_G2(L, pairing);
	element_fread(fG, "%s %s", &g, 10);
	element_fread(fGA, "%s %s", &gA, 10);
	element_fread(fC, "%s %s", &C, 10);
	element_fread(fC_0, "%s %s", &gS, 10); // 部分密文 g^s
	element_fread(fK, "%s %s", &K, 10);	   // e(C',K) 部分私钥
	element_fread(fL, "%s %s", &L, 10);

	for (i = 0; i < rows; i++)
	{
		element_init_G2(cipher_r[i], pairing);
		element_init_G2(d_r[i], pairing);
		element_init_Zr(omega[i], pairing);
		element_fread(fC_rows, "%s %s\n", &cipher_r[i], 10);
		element_fread(fD_rows, "%s %s\n", &d_r[i], 10);
	}
	for (i = 0; i < attrNo; i++)
	{
		element_init_G2(Kx[i], pairing);
		element_fread(fKx, "%s %s\n", &Kx[i], 10);
		//	element_printf("Kx[%d] = %B\n",i,Kx[i]);
	}
	///////finish reading the ciphertext and key from file////////
	//////close the file poitner
	fclose(fGA);
	fclose(fC);
	fclose(fC_0);
	fclose(fC_rows);
	fclose(fD_rows);
	fclose(fK);
	fclose(fL);
	fclose(fKx);

	findOmega(msp, omega, attrNo, attribute);
	/*test
	element_t mid;
	element_t eGGAST;
	element_t eGHRT;
	element_t eGGATL_eGHRT;
	element_t hTInverse;
	//element_init_Zr(t,pairing);
	element_init_GT(hTInverse,pairing);
	element_init_GT(mid,pairing);
	element_init_GT(eGGAST,pairing);
	element_init_GT(eGHRT,pairing);
	element_init_GT(eGGATL_eGHRT,pairing);
	element_pairing(mid,gA,gS);
	//element_pow_zn(eGGAST,mid,t);
	element_pairing(eGHRT,Kx[0],d_r[0]);
	element_invert(hTInverse,eGHRT);
	element_mul(eGGATL_eGHRT,eGGAST,hTInverse);
	//element_printf("t = %B\n",t);
	//element_printf("denominator1 = %B\n",eGGAST);
	//element_printf("eCL1 = %B\n",eGGATL_eGHRT);
	//test*/
	// calculate the time of pairing function
	FILE *fDecrypt = fopen("decryptTime.txt", "a+");
	float decryptTime = 0.0;
	clock_t dStart, dEnd;
	// start to decrypt the ciphertext
	element_t eGGAlphaS; // e(g,g)^alphaS
	element_t eGGgSK;	 // e(C',K)
	element_t eCL;		 // e(C_i,L)
	element_t eDKx;		 // e(D_i,Kx)
	element_t temp;
	element_t denominator; // eCL*eDKx
	element_t plaintext;
	char tmpLabel[2];
	char tmpAttr[2];
	// numerator
	element_init_GT(eGGAlphaS, pairing);
	element_init_GT(eGGgSK, pairing);
	// denominator
	element_init_GT(eCL, pairing);
	element_init_GT(eDKx, pairing);
	element_init_GT(temp, pairing);
	element_init_GT(denominator, pairing);
	// plaintext
	element_init_GT(plaintext, pairing);
	// start to calculate
	// numerator
	dStart = clock();
	element_pairing(eGGgSK, gS, K);
	// denominator
	element_set0(denominator);

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < attrNo; j++)
		{
			memset(tmpLabel, 0, 2);
			memset(tmpAttr, 0, 2);
			sprintf(tmpLabel, "%c", msp->label[i]);
			sprintf(tmpAttr, "%c", attribute[j]);
			if (!strcmp(tmpLabel, tmpAttr))
			{
				element_set0(temp);
				element_pairing(eCL, cipher_r[i], L);
				element_pairing(eDKx, d_r[i], Kx[j]);
				element_mul(temp, eCL, eDKx);
				element_pow_zn(temp, temp, omega[j]);
				element_mul(denominator, denominator, temp);
			}
		}
	}
	dEnd = clock();
	decryptTime = (float)(dEnd - dStart) / CLOCKS_PER_SEC;
	fprintf(fDecrypt, "%f\r\n", decryptTime);

	element_div(eGGAlphaS, eGGgSK, denominator);
	element_div(plaintext, C, eGGAlphaS);
	mpz_t message_mpz;
	mpz_init(message_mpz);
	char plaintext_dec[2048];
	element_to_mpz(message_mpz, plaintext);
	valueToMessage(plaintext_dec, message_mpz);
	element_printf("M2 = %B\n", plaintext);
	printf("解密密文：%s\n", plaintext_dec);
	fclose(fDecrypt);
}
