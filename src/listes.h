#include "entite.h"

typedef struct elem { struct elem *pred, *succ; t_entite * val; } t_elem;
typedef struct { t_elem *drapeau, *ec; } t_liste;
void init_liste(int i);
int liste_vide(int i);
int hors_liste(int i);
void en_tete(int i);
void en_queue(int i);
void suivant(int i);
void precedent(int i);
t_entite * valeur_elt(int i);
void modif_elt(int i, t_entite * v);
void oter_elt(int i);
void ajout_droit(int i, t_entite * v);
void ajout_gauche(int i, t_entite * v);
void ajout_entite(int i, char * id, int x, int y, int w, int h, int est_relatif);
void ajout_entite_depuis_spritesheet(int i, char * id, int x, int y, int w, int h, int est_relatif);
void vider_liste(int i);
int taille_liste(int i);
void afficher_liste(int i);
