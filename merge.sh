make myGit


if [ -r test2.txt ]; then
  rm test2.txt
fi
echo "hash2" > test2.txt

if [ -r test1.txt ]; then
  rm test1.txt
fi
echo "hash1" > test1.txt

if [ -r test3.txt ]; then
  rm test3.txt
fi
echo "hash3" > test3.txt


./myGit init
./myGit add test2.txt
./myGit commit master

./myGit branch b1
./myGit add test1.txt
./myGit commit b1


echo "hash4" > test2.txt
./myGit branch b2
./myGit add test2.txt
./myGit add test3.txt
./myGit commit b2
./myGit checkout-branch master
