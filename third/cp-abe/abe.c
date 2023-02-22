#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pbc/pbc.h>
#include <gmp.h>
#include "abeSetup.h"
#include "keyGen.h"
#include "LSSS.h"
#include "encrypt.h"
#include "decrypt.h"
#define LOOP 1
void messageToValue(void *message, mpz_t message_mpz, char *m)
{
	char *c = NULL;
	unsigned int value = 0, size = 0;
	c = message;

	while (*c != '\0')
	{
		value = (unsigned int)*c;
		mpz_mul_ui(message_mpz, message_mpz, 256);
		mpz_add_ui(message_mpz, message_mpz, value);
		c += 1;
	}

	mpz_get_str(m, 10, message_mpz);
}

void printf_element_str(element_t e, unsigned char *name)
{
	uint32_t e_len = element_length_in_bytes(e);
	unsigned char e_str[e_len];

	element_to_bytes(e_str, e);
	printf("%s\n", name);
	for (size_t i = 0; i < e_len; i++)
	{
		printf("%02x ", e_str[i]);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	// test2();
	// test();
}
// int main(int argc, char *argv[])
// {
// 	// test();
// 	//初始化参数
// 	// pairing_t pairing;
// 	// setupSingularPairing(&pairing); // setup pairing first
// 	// printf("Use supersingular curve...\n");
// 	//采用之前的参数
// 	pairing_t pairing;
// 	char s[16384];
// 	FILE *fp = fopen("prime.data", "r");
// 	size_t count = fread(s, 1, 16384, fp);
// 	fclose(fp);
// 	pairing_init_set_buf(pairing, s, count);

// 	float difftime = 0.0;
// 	int i, j, k = 0; // the index of the following loop
// 	clock_t tStart, tEnd;
// 	FILE *fTime = fopen("timeTate.txt", "w+");
// 	int loopNum = 100;
// 	while (LOOP && loopNum)
// 	{

// 		tStart = clock();
// 		MSP msp; // the monotone spanning program
// 		mspSetup(&msp);
// 		int rows = msp.rows;
// 		FILE *fUser = fopen("user.file", "r"); // the pointer to read the user files
// 		int userNo = 0;						   // the number of users
// 		int *attrNo;						   // the number of attributes
// 		char **userName;
// 		char **attribute;
// 		fscanf(fUser, "%d\n", &userNo);
// 		attrNo = (int *)malloc(sizeof(int) * userNo);
// 		userName = (char **)malloc(sizeof(char *) * userNo);
// 		attribute = (char **)malloc(sizeof(char *) * userNo);
// 		for (i = 0; i < userNo; i++)
// 		{
// 			userName[i] = (char *)malloc(sizeof(char) * 100);
// 			fscanf(fUser, "%s\n", userName[i]);
// 			fscanf(fUser, "%d\n", &attrNo[i]);
// 			attribute[i] = (char *)malloc(sizeof(char) * attrNo[i]);
// 			j = 0; // initialize the index of j
// 			k = attrNo[i];
// 			while (k != 0)
// 			{
// 				fscanf(fUser, "%c\n", &attribute[i][j]);
// 				j++;
// 				k--;
// 			}
// 		}
// 		// // 1. Setup
// 		// setup(rows, &pairing, &msp); // the first step to set up the public key and master key
// 		// // // 2. KeyGen
// 		// for (i = 0; i < userNo; i++)
// 		// {
// 		// 	keyGen(pairing, attrNo[i], attribute[i], userName[i]); // genereate the private key according to user's attribute
// 		// }
// 		// 加密密文
// 		element_t message; // the plaintext message;
// 		element_init_GT(message, pairing);

// 		// QmRjZtG91EcBhbXhUYDDeDEQpnbJ6dorok9z6CGu4eCT3d ipfs 返回地址
// 		//
// 		// FILE *messagefile = fopen("txt/gA.key","r");
// 		// element_fread(messagefile,"%s %s",&message,10);
// 		char plaintext[2048] = "ddd\0"; // value形式的密文转成int形式;
// 		char plaintext_enc[2048], M[2048];
// 		mpz_t message_mpz;
// 		mpz_init(message_mpz);
// 		messageToValue(plaintext, message_mpz, plaintext_enc);
// 		strcpy(M, "[");
// 		strcat(M, plaintext_enc);
// 		strcat(M, ",2]");
// 		element_set_str(message, M, 10);
// 		element_printf("M1 = %B\n", message);
// 		// 3.Encrypt
// 		encrypt(message, pairing, &msp);
// 		// 4.Decrypt
// 		decrypt(pairing, &msp, message, attrNo[0], attribute[0], userName[0]);
// 		tEnd = clock();
// 		// 5.Time calculation presents
// 		difftime = (float)(tEnd - tStart) / CLOCKS_PER_SEC;
// 		printf("The cost time of tate pairing: %fs\n", difftime);
// 		fprintf(fTime, "%f\r\n", difftime);
// 		loopNum--;
// 	} // end of while-loop
// 	fclose(fTime);
// 	return 0;
// }

void test()
{
	pairing_t pairing;
	char s[16384];
	FILE *fp = fopen("prime.data", "r");
	size_t count = fread(s, 1, 16384, fp);
	fclose(fp);
	pairing_init_set_buf(pairing, s, count);
	element_t g; // generator g
	element_init_G2(g, pairing);
	FILE *fG = fopen("publicKey/g.key", "r"); // fG to read the public key -genterator g
	element_fread(fG, "%s %s", &g, 10);
	// element_printf("g = %B\n", g);
	element_printf("g = %d\n", g);
}

void test2()
{
	pairing_t pairing;
	char s[16384];
	FILE *fp = fopen("prime.data", "r");
	size_t count = fread(s, 1, 16384, fp);
	fclose(fp);
	pairing_init_set_buf(pairing, s, count);
	element_t d;
	element_t z;
	element_init_Zr(d, pairing);
	element_init_Zr(z, pairing);
	element_random(d);
	element_random(z);
	element_printf("g= %d\n", d);
	element_printf("g= %d\n", z);
}
