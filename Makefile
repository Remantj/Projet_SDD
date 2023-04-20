all: myGit

hachage.o: hachage.c hachage.h
	gcc -Wall -c -o hachage.o hachage.c

LC.o: LC.c LC.h
	gcc -Wall -c -o LC.o LC.c

directory.o: directory.c directory.h
	gcc -Wall -c -o directory.o directory.c

worktree.o: worktree.c worktree.h
	gcc -Wall -c -o worktree.o worktree.c

commit.o: commit.c commit.h
	gcc -Wall -c -o commit.o commit.c

branch.o: branch.c branch.h
	gcc -Wall -c -o branch.o branch.c

merge.o: merge.c merge.h
	gcc -Wall -c -o merge.o merge.c

hachage: hachage.o main_hachage.c
	gcc -Wall -o hachage main_hachage.c hachage.o

LC: LC.o main_LC.c
	gcc -Wall -o LC main_LC.c LC.o 

directory: directory.o hachage.o LC.o main_directory.c
	gcc -Wall -o directory main_directory.c directory.o LC.o hachage.o

worktree: worktree.o hachage.o directory.o LC.o main_worktree.c
	gcc -Wall -o worktree main_worktree.c worktree.o hachage.o directory.o LC.o

commit: commit.o hachage.o directory.o LC.o worktree.o main_commit.c
	gcc -Wall -o commit main_commit.c commit.o hachage.o directory.o LC.o worktree.o

branch: main_branch.c branch.o LC.o directory.o commit.o worktree.o hachage.o
	gcc -Wall -o branch main_branch.c branch.o LC.o directory.o commit.o worktree.o hachage.o

merge: main_merge.c merge.o  LC.o directory.o commit.o worktree.o hachage.o branch.o
	gcc -Wall -o merge main_merge.c merge.o branch.o LC.o directory.o commit.o worktree.o hachage.o

myGit: myGit.c branch.o LC.o directory.o commit.o worktree.o hachage.o merge.o
	gcc -Wall -o myGit myGit.c branch.o LC.o directory.o commit.o worktree.o hachage.o merge.o

clean:
	bash clean.sh

