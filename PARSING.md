Arbre utilisé
Chaque nœud de l’arbre représente une commande ou une opération (|, &&, ||, etc...)

exemple visuel
(cat file.txt && echo "ok") || echo "fail"

tokenization:
'(' 'cat' 'file.txt' '&&' 'echo' '"ok"' ')' '||' 'echo' '"fail"'

arbre
          [OR]
         /    \
     [AND]   echo "fail"
     /    \
cat f1  echo "ok"

Exécution:
Exécute cat f1

Si cat f1 réussit (code de retour 0), alors :

Exécute echo OK

Le || est ignoré (puisque AND a réussi)

Si cat f1 échoue :

AND échoue ⇒ le echo OK est ignoré

Le echo FAIL est exécuté