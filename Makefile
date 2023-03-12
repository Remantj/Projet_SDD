all: worktree

hachage.o: hachage.c hachage.h
	gcc -Wall -c -o hachage.o hachage.c

LC.o: LC.c LC.h
	gcc -Wall -c -o LC.o LC.c

directory.o: directory.c directory.h
	gcc -Wall -c -o directory.o directory.c

worktree.o: worktree.c worktree.h
	gcc -Wall -c -o worktree.o worktree.c



hachage: hachage.o
	gcc -Wall -o hachage hachage.o

LC: LC.o
	gcc -Wall -o LC LC.o

directory: directory.o hachage.o LC.o
	gcc -Wall -o directory directory.o LC.o hachage.o

worktree: worktree.o hachage.o
	gcc -Wall -o worktree worktree.o hachage.o

