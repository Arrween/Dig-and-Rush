/** @file
*   @brief Structure de données liste chainée (source)
*/

#include <stdio.h>
#include <stdlib.h>
#include "listes.h"

/** @brief tableau global des listes chainées utilisables */
t_liste listes[N_MAX_LISTES];

/** @brief initialiser une liste vide avec son drapeau
*   @param i indice de la liste à initialiser dans le tableau global
*/
void init_liste(int i) {
    listes[i].drapeau = malloc(sizeof(t_elem));
    listes[i].drapeau->pred = listes[i].drapeau->succ = listes[i].drapeau;
    listes[i].ec = listes[i].drapeau;
}

/** @brief vérifier si une liste est vide
*   @param i indice de la liste à utiliser dans le tableau global
*   @return VRAI si la liste est vide, FAUX sinon
*/
int liste_vide(int i) {
    return listes[i].drapeau->succ == listes[i].drapeau;
}

/** @brief vérifier l’élément courant se situe hors de la liste
*   @param i indice de la liste à utiliser dans le tableau global
*   @return VRAI si l’élément courant est hors liste, FAUX sinon
*/
int hors_liste(int i) {
    return listes[i].ec == listes[i].drapeau;
}

/** @brief placer l’élément courant au début de la liste
*   @param i indice de la liste à utiliser dans le tableau global
*/
void liste_en_tete(int i) {
    listes[i].ec = listes[i].drapeau->succ;
}

/** @brief placer l’élément courant à la fin de la liste
*   @param i indice de la liste à utiliser dans le tableau global
*/
void liste_en_queue(int i) {
    listes[i].ec = listes[i].drapeau->pred;
}

/** @brief placer l’élément courant sur l’élément suivant
*   @param i indice de la liste à utiliser dans le tableau global
*/
void liste_suivant(int i) {
    if (!hors_liste(i))
        listes[i].ec = listes[i].ec->succ;
}

/** @brief placer l’élément courant sur l’élément précédent
*   @param i indice de la liste à utiliser dans le tableau global
*/
void liste_precedent(int i) {
    if (!hors_liste(i))
        listes[i].ec = listes[i].ec->pred;
}

/** @brief lire la valeur utile de l’élément courant
*   @param i indice de la liste à utiliser dans le tableau global
*   @return la valeur utile de l’élément courant, NULL s’il est hors-liste
*/
void * liste_lire(int i) {
    if (!hors_liste(i))
        return listes[i].ec->val;
    return NULL;
}

/** @brief modifier la valeur utile de l’élément courant
*   @param i indice de la liste à utiliser dans le tableau global
*/
void liste_ecrire(int i, void * v) {
    if (!hors_liste(i))
        listes[i].ec->val = v;
}

/** @brief retirer l’élément courant
*   @param i indice de la liste à utiliser dans le tableau global
*   @return la valeur utile de l’élément retiré
*/
void * liste_retirer(int i) {
    t_elem *ancien;
    void * val;
    if (!hors_liste(i)) {
        ancien = listes[i].ec;
        val = ancien->val;
        listes[i].ec->pred->succ = listes[i].ec->succ;
        listes[i].ec->succ->pred = listes[i].ec->pred;
        listes[i].ec = listes[i].ec->pred;
        free(ancien);
    }
    return val;
}

/** @brief ajouter un élémetn à droite de l’élément courant
*   @param i indice de la liste à utiliser dans le tableau global
*   @param v valeur utile de l’élément à rajouter
*/
void liste_ajouter_droite(int i, void * v) {
    t_elem *nouv = malloc(sizeof(t_elem));
    if (!hors_liste(i) || liste_vide(i)) {
        nouv->val = v;
        nouv->pred = listes[i].ec;
        nouv->succ = listes[i].ec->succ;
        listes[i].ec->succ->pred = nouv;
        listes[i].ec->succ = nouv;
        listes[i].ec = nouv;
    }
}

/** @brief ajouter un élémetn à gauche de l’élément courant
*   @param i indice de la liste à utiliser dans le tableau global
*   @param v valeur utile de l’élément à rajouter
*/
void liste_ajouter_gauche(int i, void * v) {
    t_elem *nouv = malloc(sizeof(t_elem));
    if (!hors_liste(i) || liste_vide(i)) {
        nouv->val = v;
        nouv->succ = listes[i].ec;
        nouv->pred = listes[i].ec->pred;
        listes[i].ec->pred->succ = nouv;
        listes[i].ec->pred = nouv;
        listes[i].ec = nouv;
    }
}

/** @brief retirer tous les éléments de la liste chainée
*   @param i indice de la liste à vider dans le tableau global
*/
void vider_liste(int i) {
    liste_en_queue(i);
    while (!hors_liste(i))
        liste_retirer(i);
}

/** @brief calculer le nombre d’éléments de la liste chainée
*   @param i indice de la liste à utiliser dans le tableau global
*   @return le nombre d’éléments
*/
int taille_liste(int i) {
    int taille = 0;
    liste_en_tete(i);
    while (!hors_liste(i)) {
        taille++;
        liste_suivant(i);
    }
    return taille;
}

// void afficher_liste(int i) {
//     void * elem;
//     liste_en_tete(i);
//     printf("affichage liste %i\n", i);
//     while (!hors_liste(i)) {
//         elem = liste_lire(i);
//         printf("%s %i\n", elem->type, elem->rect_dst->y+elem->rect_dst->h);
//         liste_suivant(i);
//     }
// }
