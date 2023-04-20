rm *.o

if [ -x hachage ]; then
  rm hachage
fi
if [ -x LC ]; then
  rm LC
fi
if [ -x directory ]; then
  rm directory
fi
if [ -x worktree ]; then
  rm worktree
fi
if [ -x commit ]; then
  rm commit
fi
if [ -x branch ]; then
  rm branch
fi
if [ -x merge ]; then
  rm merge
fi
if [ -x myGit ]; then
  rm myGit
fi

rm test*

rm -r ??
rm -r .[a-z]*
rm -r repertoire
