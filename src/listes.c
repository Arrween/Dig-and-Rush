#include <stdio.h>
#include <stdlib.h>
#include "listes.h"

#define N_MAX_LISTES 100
t_liste listes[N_MAX_LISTES];

void init_liste(int i) {
    listes[i].drapeau = malloc(sizeof(t_elem));
    listes[i].drapeau->pred = listes[i].drapeau->succ = listes[i].drapeau;
    listes[i].ec = listes[i].drapeau;
}

int liste_vide(int i) {
    return listes[i].drapeau->succ == listes[i].drapeau;
}

int hors_liste(int i) {
    return listes[i].ec == listes[i].drapeau;
}

void en_tete(int i) {
    listes[i].ec = listes[i].drapeau->succ;
}

void en_queue(int i) {
    listes[i].ec = listes[i].drapeau->pred;
}

void suivant(int i) {
    if (!hors_liste(i))
        listes[i].ec = listes[i].ec->succ;
}

void precedent(int i) {
    if (!hors_liste(i))
        listes[i].ec = listes[i].ec->pred;
}

void * valeur_elt(int i) {
    if (!hors_liste(i))
        return listes[i].ec->val;
    return NULL;
}

void modif_elt(int i, void * v) {
    if (!hors_liste(i))
        listes[i].ec->val = v;
}

void oter_elt(int i) {
    t_elem *ancien;
    if (!hors_liste(i)) {
        ancien = listes[i].ec;
        listes[i].ec->pred->succ = listes[i].ec->succ;
        listes[i].ec->succ->pred = listes[i].ec->pred;
        listes[i].ec = listes[i].ec->pred;
        free(ancien);
    }
}

void ajout_droit(int i, void * v) {
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

void ajout_gauche(int i, void * v) {
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

void vider_liste(int i) {
    en_queue(i);
    while (!hors_liste(i))
        oter_elt(i);
}

int taille_liste(int i) {
    int taille = 0;
    en_tete(i);
    while (!hors_liste(i)) {
        taille++;
        suivant(i);
    }
    return taille;
}

// void afficher_liste(int i) {
//     void * elem;
//     en_tete(i);
//     printf("affichage liste %i\n", i);
//     while (!hors_liste(i)) {
//         elem = valeur_elt(i);
//         printf("%s %i\n", elem->type, elem->rect_dst->y+elem->rect_dst->h);
//         suivant(i);
//     }
// }
