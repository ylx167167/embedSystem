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
	// build the pairing function
	pairing_t pairing;
	// if (argc < 2)
	// {
	// 	fprintf(stderr, "Wrong input arguments!\n");
	// 	fprintf(stderr, "Please input <./abe><supersinuglar> or <./abe><ordinary>\n");
	// }
	// else
	// {
	// if (!strcmp(argv[1], "ordinary"))
	// {
	// 	setupOrdinaryPairing(&pairing);
	// 	printf("Use ordinary curve...\n");
	// }
	// else if (!strcmp(argv[1], "supersingular"))
	// {
	// 	setupSingularPairing(&pairing); // setup pairing first
	// 	printf("Use supersingular curve...\n");
	// }
	// else
	// {
	// 	fprintf(stderr, "Wrong input arguments!");
	// 	fprintf(stderr, "Please input <./abe><sinuglar> or <./abe><ordinary>\n");
	// }
	setupSingularPairing(&pairing); // setup pairing first
	printf("Use supersingular curve...\n");
	// }
	// end of building the pairing funcion
	// construct a CP-ABE scheme
	// Pre-computation -> read the file of users
	float difftime = 0.0;
	int i, j, k = 0; // the index of the following loop
	clock_t tStart, tEnd;
	FILE *fTime = fopen("timeTate.txt", "w+");
	int loopNum = 100;
	while (LOOP && loopNum)
	{

		tStart = clock();
		MSP msp; // the monotone spanning program
		mspSetup(&msp);
		int rows = msp.rows;
		FILE *fUser = fopen("user.file", "r"); // the pointer to read the user files
		int userNo = 0;						   // the number of users
		int *attrNo;						   // the number of attributes
		char **userName;
		char **attribute;
		fscanf(fUser, "%d\n", &userNo);
		attrNo = (int *)malloc(sizeof(int) * userNo);
		userName = (char **)malloc(sizeof(char *) * userNo);
		attribute = (char **)malloc(sizeof(char *) * userNo);
		for (i = 0; i < userNo; i++)
		{
			userName[i] = (char *)malloc(sizeof(char) * 100);
			fscanf(fUser, "%s\n", userName[i]);
			fscanf(fUser, "%d\n", &attrNo[i]);
			attribute[i] = (char *)malloc(sizeof(char) * attrNo[i]);
			j = 0; // initialize the index of j
			k = attrNo[i];
			while (k != 0)
			{
				fscanf(fUser, "%c\n", &attribute[i][j]);
				j++;
				k--;
			}
		}
		// 1. Setup
		setup(rows, &pairing, &msp); // the first step to set up the public key and master key
		// // 2. KeyGen
		for (i = 0; i < userNo; i++)
		{
			keyGen(pairing, attrNo[i], attribute[i], userName[i]); // genereate the private key according to user's attribute
		}
		// 加密密文
		element_t message; // the plaintext message;
		element_init_GT(message, pairing);

		// QmRjZtG91EcBhbXhUYDDeDEQpnbJ6dorok9z6CGu4eCT3d ipfs 返回地址
		//
		// FILE *messagefile = fopen("txt/gA.key","r");
		// element_fread(messagefile,"%s %s",&message,10);
		char plaintext[2048] = "ddd\0"; // value形式的密文转成int形式;
		char plaintext_enc[2048], M[2048];
		mpz_t message_mpz;
		mpz_init(message_mpz);
		messageToValue(plaintext, message_mpz, plaintext_enc);
		strcpy(M, "[");
		strcat(M, plaintext_enc);
		strcat(M, ",2]");
		element_set_str(message, M, 10);
		element_printf("M1 = %B\n", message);
		// 3.Encrypt
		encrypt(message, pairing, &msp);
		// 4.Decrypt
		decrypt(pairing, &msp, message, attrNo[1], attribute[1], userName[1]);
		tEnd = clock();
		// 5.Time calculation presents
		difftime = (float)(tEnd - tStart) / CLOCKS_PER_SEC;
		printf("The cost time of tate pairing: %fs\n", difftime);
		fprintf(fTime, "%f\r\n", difftime);
		loopNum--;
	} // end of while-loop
	fclose(fTime);
	return 0;
}
