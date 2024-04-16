#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "morceaux_niveau.h"
#include "constantes.h"
#include "entite_destructible.h"
#include "entite_bonus.h"
#include "entite_obstacle.h"
#include "entite_pnj.h"
#include "listes.h"

#define LARGEUR_MUR 10

#define CHANCE_BLOC_PIERRE 4 // en dizaine de %
#define CHANCE_BLOC_TERRE 4 // en dizaine de %
#define CHANCE_BLOC_VIDE 2 // en dizaine de %
#define MAX_BLOCS_PIERRE 7

void generer_premiere_ligne(float y) {
    liste_en_queue(I_LISTE_ENTITES);

    for (int j = 1; j <= 8; j++) {
        liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_destructible("bloc_terre", j*10, y, 10, 10, VRAI));
    }
}

/**
 * @brief Génère un morceau de niveau aléatoire qui se répète.
 */

void generer_morceau_niveau(float repere_defilement){
    liste_en_queue(I_LISTE_ENTITES);

    // générer un nombre aléatoire entre 0 et 9
    int random;
    srand(time(NULL));

    int i, j;
    float x = 10, y = repere_defilement;
    int n_pierres_ligne = 0;
    int presences_blocs[8];

    for (i = 1; i < 10; i+=2, y += 20) {
        n_pierres_ligne = 0;
        for (j = 0; j < 8; j++)
            presences_blocs[j] = FAUX;
        for (j = 0, x = 10; j < 8; j++, x += 10) {
            random = rand() % 10;
            // génération d’un bloc de pierre en (x,y) avec proba CHANCE_BLOC_PIERRE * 10% et un max de MAX_BLOCS_PIERRE
            if (random < CHANCE_BLOC_PIERRE && n_pierres_ligne < MAX_BLOCS_PIERRE) {
                liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", x, y, 10, 10, VRAI));
                n_pierres_ligne++;
                presences_blocs[j] = VRAI;
            }
            // génération d’un bloc de terre en (x,y) avec proba CHANCE_BLOC_TERRE * 10%
            else if (random >= CHANCE_BLOC_PIERRE && random < CHANCE_BLOC_PIERRE + CHANCE_BLOC_TERRE) {
                liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_destructible("bloc_terre", x, y, 10, 10, VRAI));
                presences_blocs[j] = VRAI;
            }
        }
        // ajouter un ennemi ou un bonus au-dessus d’un bloc aléatoire de la ligne
        random = rand() % 8;
        int limite = random - 1 > 0 ? random - 1 : 0;
        // parcourir la ligne à partir d’une colonne aléatoire jusqu’à trouver un bloc où poser l’ennemi
        for (j = random; j != limite && !presences_blocs[j]; j = (j + 1) % 8);
        if (presences_blocs[j]) {
            if (i == 5)
                liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_pnj_depuis_spritesheet("squelette", 10*(j+1)-5, y-17, 20, 17, VRAI));
            else if (i == 9)
                liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_pnj_depuis_spritesheet("feu", 10*(j+1)-5, y-20, 20, 20, VRAI));
            else if (i == 3) {
                if (random < 4)
                    liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_bonus("pieces", 10*(j+1)+2.5, y-6, 7, 7, VRAI));
                else
                    liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_bonus("coeur", 10*(j+1)+2, y-6, 6, 6, VRAI));
            }
            else if(i == 1)
                liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_pnj_depuis_spritesheet("oncle", 10*(j+1)-1.5, y-11.5, 15, 12, VRAI));

        }
    }
}

/**
 * @brief Génère les murs entourant le niveau.
 */

void generer_murs(void) {
    int n_blocs_mur = 20; // nombre pour chaque mur

    liste_en_queue(I_LISTE_ENTITES);
    for (int i = 0; i < n_blocs_mur; i++) {
        liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 0, i*5, LARGEUR_MUR, 10, VRAI)); // côté gauche
        liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 0, (n_blocs_mur+i)*5, LARGEUR_MUR, 10, VRAI)); // côté gauche écran du dessous
        liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 90, i*5, LARGEUR_MUR, 10, VRAI)); // côté droit
        liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 90, (n_blocs_mur+i)*5, LARGEUR_MUR, 10, VRAI)); // côté droit écran du dessous
    }
}
