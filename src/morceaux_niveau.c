#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "morceaux_niveau.h"
#include "constantes.h"
#include "entite.h"
#include "entite_destructible.h"
#include "entite_obstacle.h"
#include "entite_pnj.h"
#include "listes.h"

#define LARGEUR_MUR 10


/**
 * @brief Génère un morceau de niveau aléatoire qui se répète.
 */

void generer_morceau_niveau(float repere_defilement){
    en_queue(I_LISTE_ENTITES);

    // Demander un nombre aléatoire entre 0 et 10 à l'utilisateur
    int random;
    srand(time(NULL));

    int i, j;
    float x = 10, y = 200 - repere_defilement;
    int nb_pierres = 0;
    int n_ennemis;

    for (i = 0; i < 10; i++, y += 10) {
        if (i % 2) {
            for (j = 0, x = 10, nb_pierres = 0, n_ennemis = 0; j < 8; j++, x += 10) {
                random = rand() % 10 + 1;
                if ((random < 4) && (nb_pierres < 7)) {
                    ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", x, y, 10, 10, VRAI));
                    nb_pierres++;
                    if (i % 5 == 0 && n_ennemis == 0){
                        ajout_droit(I_LISTE_ENTITES, creer_entite_pnj_depuis_spritesheet("squelette", x-5, y-17, 20, 17, VRAI));
                        n_ennemis = 1;
                    }
                    else if (i%3==0 && n_ennemis==0){
                    	ajout_droit(I_LISTE_ENTITES, creer_entite_pnj_depuis_spritesheet("feu", x-5, y-20, 20, 20, VRAI));
                    	n_ennemis=1;
                    }

                }
                else if (random == 4 || random == 5);
                else if (random > 5) {
                    ajout_droit(I_LISTE_ENTITES, creer_entite_destructible("bloc_terre", x, y, 10, 10, VRAI));

                    if (i % 5 == 0 && n_ennemis == 0){
                        ajout_droit(I_LISTE_ENTITES, creer_entite_pnj_depuis_spritesheet("squelette", x-5, y-17, 20, 17, VRAI));
                        n_ennemis = 1;
                    }

                    else if (i%3==0 && n_ennemis==0){
                        ajout_droit(I_LISTE_ENTITES, creer_entite_pnj_depuis_spritesheet("feu", x-5, y-20, 20, 20, VRAI));
                        n_ennemis = 1;
                    }
                    
                }
            }
        }
    }
}

/**
 * @brief Génère les murs entourant le niveau.
 */

void generer_murs(void) {
    int n_blocs_mur = 20; // nombre pour chaque mur

    en_queue(I_LISTE_ENTITES);
    for (int i = 0; i < n_blocs_mur; i++) {
        ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 0, i*5, LARGEUR_MUR, 10, VRAI)); // côté gauche
        ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 0, (n_blocs_mur+i)*5, LARGEUR_MUR, 10, VRAI)); // côté gauche écran du dessous
        ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 90, i*5, LARGEUR_MUR, 10, VRAI)); // côté droit
        ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 90, (n_blocs_mur+i)*5, LARGEUR_MUR, 10, VRAI)); // côté droit écran du dessous
    }
}
