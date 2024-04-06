#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "constantes.h"
#include "entite.h"
#include "morceaux_niveau.h"
#include "listes.h"

#define MAX_ENTITES 100
#define LARGEUR_MUR 5

int (*fonctions_generation[N_MORCEAUX_NIVEAU])(t_entite**) = {
    generer_morceau_niveau_0,
    generer_morceau_niveau_1,
};

void generer_morceau_niveau(int i_liste, int choix) {
    if (choix < 0 || choix >= N_MORCEAUX_NIVEAU)
        choix = rand() % N_MORCEAUX_NIVEAU;
    printf("génération morceau de niveau %i\n", choix);
    t_entite * entites[MAX_ENTITES];
    fonctions_generation[choix](entites);
    en_queue(i_liste);
    for (int i = 0; entites[i] != NULL; i++)
        ajout_droit(i_liste, entites[i]);
}

// génération par fonctions C plutôt que par parsage d’un fichier pour
// bénéficier de la Turing-complétion

int generer_morceau_niveau_0(t_entite * entites[MAX_ENTITES]) {
    // tailles ajustées pour être carrées
    entites[0] = creer_entite("bloc_pierre", 0, 110, 20, 10, VRAI);
    entites[1] = creer_entite("bloc_pierre", 50, 170, 10, 10, VRAI);
    entites[2] = creer_entite("bloc_terre", 20, 110, 10, 10, VRAI);
    entites[3] = creer_entite("bloc_terre", 60, 170, 10, 10, VRAI);
    entites[4] = NULL;
    return VRAI;
}

int generer_morceau_niveau_1(t_entite * entites[MAX_ENTITES]) {
    int i,j;
    int k = 0;
    for (i = 4; i > 0; i--) {
        for (j = 0; j < i+1; j++) {
            entites[k] = creer_entite("bloc_terre", LARGEUR_MUR + 10*(4-i), 125 + (j-i)*5, 10, 5, VRAI);
            if (++k >= MAX_ENTITES)
                goto erreur_max_atteint;
        }
    }
    for (i = 4; i < 8; i++) {
        for (j = 0; j < i-3; j++) {
            entites[k] = creer_entite("bloc_terre", LARGEUR_MUR + 10*i, 145 + (j-i)*5, 10, 5, VRAI);
            if (++k >= MAX_ENTITES)
                goto erreur_max_atteint;
        }
    }
    if (k >= MAX_ENTITES)
        goto erreur_max_atteint;

    entites[k] = NULL;
    return VRAI;

    erreur_max_atteint:
        entites[MAX_ENTITES-1] = NULL;
        return FAUX;
}

void generer_murs(int i_liste) {
    int n_blocs_mur = 20;
    int n_blocs_total = n_blocs_mur * 2 * 2; // une rangée fois deux côtés fois deux écrans à générer
    t_entite * entites[n_blocs_total];

    for (int i = 0; i < n_blocs_mur; i++) {
        entites[i] = creer_entite("bloc_pierre", 0, i*5, LARGEUR_MUR, 5, VRAI); // côté gauche
        entites[n_blocs_mur + i] = creer_entite("bloc_pierre", 0, (n_blocs_mur+i)*5, LARGEUR_MUR, 5, VRAI); // côté gauche écran du dessous
        entites[n_blocs_mur*2 + i] = creer_entite("bloc_pierre", 95, i*5, LARGEUR_MUR, 5, VRAI); // côté droit
        entites[n_blocs_mur*3 + i] = creer_entite("bloc_pierre", 95, (n_blocs_mur+i)*5, LARGEUR_MUR, 5, VRAI); // côté droit écran du dessous
    }

    en_queue(i_liste);
    for (int i = 0; i < n_blocs_total; i++)
        ajout_droit(i_liste, entites[i]);
}
