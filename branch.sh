bash worktree.sh
echo "test3" > test_branch.txt

make myGit

./myGit init
./myGit add repertoire
./myGit commit master -m test_branch_repertoire

./myGit add test_branch.txt
./myGit commit master -m test_branch_txt
