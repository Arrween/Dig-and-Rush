/** @file
*   @brief Structure de données liste chainée (entête)
*/

#ifndef _LISTES_H_
#define _LISTES_H_

/** @brief indice utilisé pour la liste stockant toutes les entités */
#define I_LISTE_ENTITES 0

/** @brief nombre maximum de listes chainées utilisables */
#define N_MAX_LISTES 100

/** @brief élément d’une liste chainée */
typedef struct elem {
    struct elem * pred; /**< élément précédent de la liste */
    struct elem * succ; /**< élément suivant de la liste */
    void * val;         /**< valeur utile de l’élément */
} t_elem;

/** @brief liste chainée */
typedef struct {
    t_elem * drapeau; /**< sert pour définir les extrémités de la liste */
    t_elem * ec;      /**< élément courant dans la liste */
} t_liste;

void init_liste(int i);
int liste_vide(int i);
int hors_liste(int i);
void liste_en_tete(int i);
void liste_en_queue(int i);
void liste_suivant(int i);
void liste_precedent(int i);
void * liste_lire(int i);
void liste_ecrire(int i, void * v);
void * liste_retirer(int i);
void liste_ajouter_droite(int i, void * v);
void liste_ajouter_gauche(int i, void * v);
void vider_liste(int i);
int taille_liste(int i);
// void afficher_liste(int i);

#endif
