#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "constantes.h"
#include "entite.h"
#include "morceaux_niveau.h"
#include "listes.h"

t_entite ** (*fonctions_generation[N_MORCEAUX_NIVEAU])(void) = {
    generer_morceau_niveau_0,
    generer_morceau_niveau_1,
};

void generer_morceau_niveau(int i_liste) {
    int choix = rand() % N_MORCEAUX_NIVEAU;
    printf("génération morceau de niveau %i\n", choix);
    t_entite ** entites = fonctions_generation[choix]();
    en_queue(i_liste);
    for (int i = 0; entites[i] != NULL; i++)
        ajout_droit(i_liste, entites[i]);
}

// génération par fonctions C plutôt que par parsage d’un fichier pour
// bénéficier de la Turing-complétion

t_entite ** generer_morceau_niveau_0(void) {
    int n_entites = 4;
    t_entite ** entites = malloc(sizeof(t_entite*) * (n_entites+1)); // « +1 » pour ajouter un marqueur de fin de tableau NULL

    // tailles ajustées pour être carrées
    entites[0] = creer_entite("bloc_pierre", 0, 110, 10, 10, VRAI);
    entites[1] = creer_entite("bloc_pierre", 50, 170, 10, 10, VRAI);
    entites[2] = creer_entite("bloc_terre", 10, 110, 10, 10, VRAI);
    entites[3] = creer_entite("bloc_terre", 60, 170, 10, 10, VRAI);
    entites[4] = NULL;

    return entites;
}

t_entite ** generer_morceau_niveau_1(void) {
    int n_entites = 6;
    t_entite ** entites = malloc(sizeof(t_entite*) * (n_entites+1)); // « +1 » pour ajouter un marqueur de fin de tableau NULL

    for (int i = 0; i < n_entites; i++) {
        entites[i] = creer_entite("bloc_terre", 10*i, 100 + i*5, 10, 10, VRAI);
    }
    entites[6] = NULL;

    return entites;
}
