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

    int i, x, y;
    for (i=0, x=10, y=110; i<8; i++, x+=10){
        if (i==6){
            ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", x, y+20, 10, 10, VRAI));
            ajout_droit(I_LISTE_ENTITES, creer_entite_destructible("bloc_terre", x, y+30, 10, 10, VRAI));
            ajout_droit(I_LISTE_ENTITES, creer_entite_destructible("bloc_terre", x, y+40, 10, 10, VRAI));
        }
        
        if (i==7){
            ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", x, y+20, 10, 10, VRAI));
            ajout_droit(I_LISTE_ENTITES, creer_entite_destructible("bloc_terre", x, y+30, 10, 10, VRAI));
            ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", x, y+40, 10, 10, VRAI));
        }

        

        else{
            ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", x, y, 10, 10, VRAI));
            if (i != 5 && i != 7)
                ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", x, y+20, 10, 10, VRAI));
            if (i==0)
                ajout_droit(I_LISTE_ENTITES, creer_entite_destructible("bloc_terre", x, y+20, 10, 10, VRAI));

            if (i==2 || i==3){
                ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", x, y+60, 10, 10, VRAI));
                ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", x, y+70, 10, 10, VRAI));
            }
            ajout_droit(I_LISTE_ENTITES, creer_entite_destructible("bloc_terre", x, y+40, 10, 10, VRAI));

        }
    }



    nouv = creer_entite_pnj_depuis_spritesheet("squelette", 10, 130, 20, 20, VRAI);
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
        ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 0, i*5, LARGEUR_MUR, 10, VRAI)); // côté gauche
        ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 0, (n_blocs_mur+i)*5, LARGEUR_MUR, 10, VRAI)); // côté gauche écran du dessous
        ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 90, i*5, LARGEUR_MUR, 10, VRAI)); // côté droit
        ajout_droit(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", 90, (n_blocs_mur+i)*5, LARGEUR_MUR, 10, VRAI)); // côté droit écran du dessous
    }
}
