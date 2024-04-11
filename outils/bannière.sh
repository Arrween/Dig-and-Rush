#!/bin/bash

texte="$@"
l_texte="$(echo -n "$texte"|wc -c)"


remplissage=" "
l_remplissage=3

n_colonnes="$(tput cols)"

separateur="–"
n_separateurs="$(expr $n_colonnes - 2 \* $l_remplissage - $l_texte)"

echo

for i in $(seq 1 $(expr $n_separateurs / 2))
do
    echo -n "$separateur"
done
for i in $(seq 1 $l_remplissage)
do
    echo -n "$remplissage"
done

printf '%s' "$texte"

for i in $(seq 1 $l_remplissage)
do
    echo -n "$remplissage"
done
for i in $(seq 1 $(expr $n_separateurs / 2))
do
    echo -n "$separateur"
done

echo
echo
