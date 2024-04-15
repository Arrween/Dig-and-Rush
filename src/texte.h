/**
 *  @file
 *  @brief Fonctions de gestion simplifiée de textes via SDL_ttf (entête)
 */

#ifndef TEXTE_H
#define TEXTE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/** @brief taille maximale du contenu d’un objet `t_texte` */
#define TAILLE_MAX_TEXTE 1000

/**
 * @brief Structure représentant un texte à afficher.
 */

typedef struct {
    TTF_Font * police; /**< Police de caractères utilisée pour le texte. */
    char contenu[TAILLE_MAX_TEXTE]; /**< Contenu du texte. */
    SDL_Color couleur; /**< Couleur du texte. */
    SDL_Rect rect_dst; /**< Rectangle de destination pour le texte. */
    SDL_Surface * surface; /**< Surface contenant le texte rendu. */
} t_texte;

t_texte * creer_texte(char * id, int r, int v, int b, int a,
                                 int x, int y, int w, int h);
void detruire_texte(t_texte**);
void changer_texte(t_texte*, char* format_contenu, ...);
void afficher_texte(SDL_Renderer*, t_texte*);

#endif
