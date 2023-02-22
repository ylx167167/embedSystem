#include "/usr/local/include/pbc/pbc.h"
#include "LSSS.h"
#include "keyGen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "elementIO.h"

void keyGen(pairing_t pairing, int attrNo, char *attribute, char *userName)
{

	FILE *fMsk = fopen("MSK/msk.key", "r");		// fMsk to read the master key
	FILE *fG = fopen("publicKey/g.key", "r");	// fG to read the public key -genterator g
	FILE *fGA = fopen("publicKey/gA.key", "r"); // fGA to read the public key -gA
	FILE *fH;									// fH to read the public key-h attribute
	char hCmd[100];								// the command line of attribute h
	char attrName[2];							// the name of the attributes
	element_t g;								// generator g
	element_t gA;								// gA = g^A
	element_t msk;								// msk = g^alpha
	element_t h[attrNo];						// h[0]...h[attrNo-1]
	element_init_G2(g, pairing);
	element_fread(fG, "%s %s", &g, 10);
	// element_printf("g= %B\n",g);
	element_init_G2(gA, pairing);
	element_fread(fGA, "%s %s", &gA, 10);
	// element_printf("gA= %B\n",gA);
	element_init_G2(msk, pairing);
	memset(hCmd, 0, 100);	// initialize the hCmd
	memset(attrName, 0, 2); // initialize the attrName
	int i = 0;				// the index of the following loop
	for (i = 0; i < attrNo; i++)
	{
		strcpy(hCmd, "publicKey/h");
		sprintf(attrName, "%c", attribute[i]);
		strcat(hCmd, attrName);
		strcat(hCmd, ".key");
		printf("hCmd= %s ", hCmd);
		fH = fopen(hCmd, "r");
		element_init_G2(h[i], pairing);
		element_fread(fH, "%s %s\n", &h[i], 10);
		memset(attrName, 0, 2);
		fclose(fH);
	}

	element_fread(fMsk, "%s %s", &msk, 10);
	// element_printf("MSK= %B\n",msk);
	// close all file pointer
	fclose(fG);
	fclose(fGA);
	fclose(fMsk);
	// end of read file
	// start to calculate private key and write file
	element_t t;  // the random t over Z_p
	element_t L;  // L = g^t
	element_t K;  // K = (g^alpha)*(g^at)
	element_t Kx; // Kx = hx^t
	element_t temp;
	element_init_Zr(t, pairing);
	element_init_G2(L, pairing);
	element_init_G2(K, pairing);
	element_init_G2(Kx, pairing);
	element_init_G2(temp, pairing);
	char command[100];
	// establish the user's own directory
	//  memset(command,0,100);
	//  strcpy(command,"mkdir ");
	//  strcat(command,userName);
	//  system(command);
	// open the files of users' private keys
	char fileL[100];
	char fileK[100];
	char fileKx[100];
	strcpy(fileL, userName);
	strcat(fileL, "/L.key");
	strcpy(fileK, userName);
	strcat(fileK, "/K.key");
	strcpy(fileKx, userName);
	strcat(fileKx, "/Kx.key");
	FILE *fL = fopen(fileL, "w");	// fL to write the privateKey L
	FILE *fK = fopen(fileK, "w");	// fK to write the privateKey K
	FILE *fKx = fopen(fileKx, "w"); // fKx to write the privateKey Kx

	element_random(t);
	element_pow_zn(L, g, t);
	element_fprintf(fL, "%B", L);
	element_pow_zn(temp, gA, t); // first K = g^at
	element_mul(K, temp, msk);	 // second K = K*g^alpha
	element_fprintf(fK, "%B", K);

	for (i = 0; i < attrNo; i++)
	{
		element_set0(Kx);
		element_pow_zn(Kx, h[i], t);	  // Kx = hx^t
		element_fprintf(fKx, "%B\n", Kx); // Kx = hx^t
	}
	// close all file pointer
	fclose(fL);
	fclose(fK);
	fclose(fKx);
} // end of keyGen
