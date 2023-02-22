#include "elementIO.h"


void element_fread(FILE *fp,char *format,element_t *e,int base){

        char *temp1 = calloc(1024,sizeof(char));
        char *temp2 = calloc(1024,sizeof(char));
        char *tempAll = calloc(2048,sizeof(char));
        fscanf(fp,format,temp1,temp2);
        strcpy(tempAll,temp1);
        strcat(tempAll,temp2);
        // printf("tempAll %s",tempAll);
        element_set_str(*e,tempAll,base);
	free(temp1);
	free(temp2);
	free(tempAll);

}

void element_fread_line(FILE *fp,element_t *e,int base){

        char *temp1 = calloc(1024,sizeof(char));
        char *temp2 = calloc(1024,sizeof(char));
        char *tempAll = calloc(2048,sizeof(char));
		fgets(tempAll,2048,fp);
        element_set_str(*e,tempAll,base);
        free(temp1);
        free(temp2);
        free(tempAll);

}
/*
void elementStringToFile(char *outputFile, element_t element){

	FILE *fp = fopen(outputFile,"w");
	element_out_str(fp,10,element);
	fclose(fp);

}

void elementFileToString(char *inputFile,element_t element){
	
	char *temp = calloc(2048,sizeof(char));
	FILE *fp = fopen(inputFile,"r");
	fgets(temp,2048,fp);
	element_set_str(element,temp,10);
	fclose(fp);
	free(temp);
	
}*/
