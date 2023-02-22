all: abe
	 rm *.o
abe: abe.o abeSetup.o keyGen.o LSSS.o elementIO.o encrypt.o decrypt.o
	gcc -o abe abe.o abeSetup.o keyGen.o LSSS.o elementIO.o encrypt.o decrypt.o -L. -lpbc -lgmp
abe.o: abe.c
	gcc -c abe.c
abeSetup.o: abeSetup.c abeSetup.h
	gcc -c abeSetup.c
keyGen.o: keyGen.c keyGen.h
	gcc -c keyGen.c
LSSS.o: LSSS.c LSSS.h
	gcc -c LSSS.c
elementIO.o: elementIO.c elementIO.h
	gcc -c elementIO.c
encrypt.o: encrypt.c encrypt.h
	gcc -c encrypt.c
decrypt.o: decrypt.c decrypt.h
	gcc -c decrypt.c
# clean: 
# 	rm *.o