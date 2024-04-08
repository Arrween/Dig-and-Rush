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

#define LARGEUR_MUR 5

void (*fonctions_generation[N_MORCEAUX_NIVEAU])(void) = {
    generer_morceau_niveau_0,
    generer_morceau_niveau_1,
};

void generer_morceau_niveau(int choix) {
    if (choix < 0 || choix >= N_MORCEAUX_NIVEAU)
        choix = rand() % N_MORCEAUX_NIVEAU;
    printf("génération morceau de niveau %i\n", choix);
    fonctions_generation[choix]();
}

// génération par fonctions C plutôt que par parsage d’un fichier pour
// bénéficier de la Turing-complétion

void generer_morceau_niveau_0(void) {
    // tailles ajustées pour être carrées
    t_entite * nouv;

    en_queue(I_LISTE_ENTITES);

    ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 0, 110, 20, 10, VRAI));
    ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 30, 160, 10, 10, VRAI));
    ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 40, 170, 25, 10, VRAI));
    ajout_droit(I_LISTE_ENTITES, creer_entite_destructible("bloc_terre", 20, 110, 20, 10, VRAI));
    ajout_droit(I_LISTE_ENTITES, creer_entite_destructible("bloc_terre", 65, 170, 22, 10, VRAI));

    nouv = creer_entite_pnj_depuis_spritesheet("squelette", 55, 150, 20, 20, VRAI);
    nouv->vitesse = 1./2;
    nouv->pnj->x_patrouille_g = 40;
    nouv->pnj->x_patrouille_d = 87;
    changer_hitbox(nouv, 20, 50, 50, 50);
    ajout_droit(I_LISTE_ENTITES, nouv);
}

void generer_morceau_niveau_1(void) {
    en_queue(I_LISTE_ENTITES);
    for (int i = 4; i > 0; i--)
        for (int j = 0; j < i+1; j++)
            ajout_droit(I_LISTE_ENTITES, creer_entite_destructible("bloc_terre", LARGEUR_MUR + 10*(4-i), 125 + (j-i)*5, 10, 5, VRAI));
    for (int i = 4; i < 8; i++)
        for (int j = 0; j < i-3; j++)
            ajout_droit(I_LISTE_ENTITES, creer_entite_destructible("bloc_terre", LARGEUR_MUR + 10*i, 145 + (j-i)*5, 10, 5, VRAI));
}

void generer_murs(void) {
    int n_blocs_mur = 20; // nombre pour chaque mur

    en_queue(I_LISTE_ENTITES);
    for (int i = 0; i < n_blocs_mur; i++) {
        ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 0, i*5, LARGEUR_MUR, 5, VRAI)); // côté gauche
        ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 0, (n_blocs_mur+i)*5, LARGEUR_MUR, 5, VRAI)); // côté gauche écran du dessous
        ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 95, i*5, LARGEUR_MUR, 5, VRAI)); // côté droit
        ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 95, (n_blocs_mur+i)*5, LARGEUR_MUR, 5, VRAI)); // côté droit écran du dessous
    }
}
