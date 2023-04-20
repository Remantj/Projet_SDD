if [ -d repertoire ]; then
  rm -r repertoire
fi

mkdir repertoire
mkdir repertoire/dossier
touch repertoire/fichier1.txt
touch repertoire/dossier/fichier2.txt

echo "test1" > repertoire/fichier1.txt
echo "test2" > repertoire/dossier/fichier2.txt
