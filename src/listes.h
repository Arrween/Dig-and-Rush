#include "entite.h"

typedef struct elem { struct elem *pred, *succ; void * val; } t_elem;
typedef struct { t_elem *drapeau, *ec; } t_liste;
void init_liste(int i);
int liste_vide(int i);
int hors_liste(int i);
void en_tete(int i);
void en_queue(int i);
void suivant(int i);
void precedent(int i);
void * valeur_elt(int i);
void modif_elt(int i, void * v);
void oter_elt(int i);
void ajout_droit(int i, void * v);
void ajout_gauche(int i, void * v);
void vider_liste(int i);
int taille_liste(int i);
// void afficher_liste(int i);
