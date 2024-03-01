#!/bin/sh

# se déplacer dans le répertoire parent du rép où est situé ce script
cd "$(dirname "$0")"/..

# indiquer où trouver les bibliothèques .so
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$PWD/lib/SDL2/lib"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$PWD/lib/SDL2_image/lib"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$PWD/lib/SDL2_ttf/lib"

# exécuter le binaire
./bin/dignrush
