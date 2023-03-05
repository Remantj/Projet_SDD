all: directory

hachage: hachage.c
	gcc -Wall -o hachage hachage.c

LC: LC.c
	gcc -Wall -o LC LC.c

directory: directory.c LC.h LC.c hachage.c
	gcc -Wall -o directory directory.c LC.c hachage.c

