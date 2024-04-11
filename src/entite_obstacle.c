#include "entite_obstacle.h"
#include "entite.h"
#include "constantes.h"

/**
 * @brief Crée une entité obstacle avec les attributs spécifiés.
 * 
 * Cette fonction crée une entité obstacle avec l'identifiant, la position, la largeur, 
 * la hauteur et le mode de positionnement (relatif ou absolu) spécifiés.
 * 
 * @param id L'identifiant de l'entité obstacle.
 * @param x La position en x de l'entité obstacle.
 * @param y La position en y de l'entité obstacle.
 * @param w La largeur de l'entité obstacle.
 * @param h La hauteur de l'entité obstacle.
 * @param est_relatif 1 si les coordonnées sont relatives, 0 si elles sont absolues.
 * @return Un pointeur vers la nouvelle entité obstacle créée.
 */
 
t_entite * creer_entite_obstacle(char * id, float x, float y, float w, float h, int est_relatif) {
    t_entite * nouv = creer_entite(id, x, y, w, h, est_relatif);
    nouv->est_obstacle = VRAI;
    return nouv;
}
