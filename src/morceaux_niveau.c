/**
 * @file morceaux_niveau.c
 * @brief Programme pour la génération de morceaux de niveau et de murs.
 */

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

#define LARGEUR_MUR 10 ///< Largeur des murs entourant le niveau

#define CHANCE_BLOC_PIERRE 4 ///< Chance de générer un bloc de pierre (en dizaines de %)
#define CHANCE_BLOC_TERRE 4 ///< Chance de générer un bloc de terre (en dizaines de %)
#define CHANCE_BLOC_VIDE 2 ///< Chance de générer un bloc vide (en dizaines de %)
#define MAX_BLOCS_PIERRE 7 ///< Nombre maximum de blocs de pierre par ligne

/**
 * @brief Génère la première ligne du niveau avec des blocs de terre.
 *
 * @param y Position en y de la première ligne.
 */
void generer_premiere_ligne(float y) {
    liste_en_queue(I_LISTE_ENTITES);

    for (int j = 1; j <= 8; j++) {
        liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_destructible("bloc_terre", j*10, y, 10, 10, VRAI));
    }
}

/**
 * @brief Génère un ennemi sur un bloc aléatoire de la ligne y si ce dernier est présent.
 * @param y Position verticale
 * @param presence_blocs Tableau d'entiers.
 * Le tableau d'entiers à l'indice i vaut VRAI s'il y a bien un bloc et FAUX sinon
 */
void generer_ennemi(float y, int presences_blocs[]) {
    int case_dispo;
    int alea_ennemi = rand() % 100;
    int alea_case = rand() % 8;
    int limite = alea_case - 1 > 0 ? alea_case - 1 : 0;
    // Parcours de la ligne à partir d’une colonne aléatoire jusqu’à trouver un bloc où poser l’ennemi
    for (case_dispo = alea_case; case_dispo != limite && !presences_blocs[case_dispo]; case_dispo = (case_dispo + 1) % 8);
    if (presences_blocs[case_dispo]) {
        if (alea_ennemi < 50)
            liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_pnj_depuis_spritesheet("squelette", 10*(case_dispo+1)-5, y-17, 20, 17, VRAI));
        else if (alea_ennemi >= 50 && alea_ennemi < 75)
            liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_pnj_depuis_spritesheet("feu", 10*(case_dispo+1)-5, y-20, 20, 20, VRAI));
        else if (alea_ennemi >= 75)
            liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_pnj_depuis_spritesheet("oncle", 10*(case_dispo+1)-1.5, y-11.5, 15, 12, VRAI));
    }
    presences_blocs[case_dispo] = FAUX;
}

/**
 * @brief Génère un bonus sur un bloc aléatoire de la ligne y si ce dernier est présent.
 * @param y Position verticale
 * @param presence_blocs Tableau d'entiers.
 * Le tableau d'entiers à l'indice i vaut VRAI s'il y a bien un bloc et FAUX sinon
 */
void generer_bonus(float y, int presences_blocs[]) {
    int case_dispo;
    int alea_bonus = rand() % 100;
    int alea_case = rand() % 8;
    int limite = alea_case - 1 > 0 ? alea_case - 1 : 0;
    // Parcours de la ligne à partir d’une colonne aléatoire jusqu’à trouver un bloc où poser le bonus
    for (case_dispo = alea_case; case_dispo != limite && !presences_blocs[case_dispo]; case_dispo = (case_dispo + 1) % 8);
    if (presences_blocs[case_dispo]) {
        if (alea_bonus < 70)
            liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_bonus("pieces", 10*(case_dispo+1)+2.5, y-6, 7, 7, VRAI));
        else
            liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_bonus("coeur", 10*(case_dispo+1)+2, y-6, 6, 6, VRAI));
    }
    presences_blocs[case_dispo] = FAUX;
}

/**
 * @brief Génère un morceau de niveau aléatoire qui se répète.
 *
 * Cette fonction génère un morceau de niveau aléatoire qui se répète verticalement.
 * Le morceau généré consiste en une série de lignes de blocs de pierre et de blocs de terre,
 * ainsi que des ennemis et des bonus placés au-dessus de certains blocs.
 *
 * @param repere_defilement Position verticale du repère de défilement, où le morceau de niveau sera généré.
 * Cette position est utilisée comme référence pour la génération des blocs et des entités.
 */
void generer_morceau_niveau(float repere_defilement){
    liste_en_queue(I_LISTE_ENTITES);

    // Générer un nombre aléatoire entre 0 et 9
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
            // Génération d’un bloc de pierre avec probabilité CHANCE_BLOC_PIERRE * 10% et un maximum de MAX_BLOCS_PIERRE
            if (random < CHANCE_BLOC_PIERRE && n_pierres_ligne < MAX_BLOCS_PIERRE) {
                liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_obstacle("bloc_pierre", x, y, 10, 10, VRAI));
                n_pierres_ligne++;
                presences_blocs[j] = VRAI;
            }
            // Génération d’un bloc de terre avec probabilité CHANCE_BLOC_TERRE * 10%
            else if (random >= CHANCE_BLOC_PIERRE && random < CHANCE_BLOC_PIERRE + CHANCE_BLOC_TERRE) {
                liste_ajouter_droite(I_LISTE_ENTITES, creer_entite_destructible("bloc_terre", x, y, 10, 10, VRAI));
                presences_blocs[j] = VRAI;
            }
        }
        random = rand() % 100;
        // ligne sans ennemi
        if (random < 25) {
            // présence bonus sur ligne sans ennemi
            if (rand() % 100 < 35)
                generer_bonus(y, presences_blocs);
        }
        // ligne avec un seul ennemi
        else if (random >= 25 && random < 90) {
            generer_ennemi(y, presences_blocs);
            // présence bonus sur ligne avec un seul ennemi
            if (rand() % 100 < 30)
                generer_bonus(y, presences_blocs);
        }
        // ligne avec deux ennemis
        else if (random >= 90) {
            generer_ennemi(y, presences_blocs);
            generer_ennemi(y, presences_blocs);
            // présence bonus sur ligne avec deux ennemis
            if (rand() % 100 < 50)
                generer_bonus(y, presences_blocs);
        }
    }
}

/**
 * @brief Génère les murs entourant le niveau.
 *
 * Cette fonction génère les murs qui bordent le niveau de jeu.
 * Elle crée des obstacles en forme de blocs de pierre pour former les murs
 * du côté gauche et du côté droit du niveau, ainsi que pour les parties
 * inférieures de l'écran.
 * Les murs sont générés de manière à entourer complètement le niveau de jeu.
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
