/**
 *  @file
 *  @brief Fonctions de gestion simplifiée de textes via SDL_ttf (source)
 */

#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdarg.h>

#include "texte.h"
#include "ressources.h"

/**
 * @brief Change le contenu du texte avec un nouveau contenu formaté.
 * 
 * @param texte Pointeur vers la structure de texte à modifier.
 * @param format_contenu Format du nouveau contenu.
 * @param ... Arguments pour le format du contenu.
 */

void changer_texte(t_texte * texte, char * format_contenu, ...) {
    va_list ap;
    va_start(ap, format_contenu);
    char contenu[TAILLE_MAX_TEXTE];
    vsprintf(contenu, format_contenu, ap);
    texte->surface = TTF_RenderText_Solid(texte->police, contenu, texte->couleur);
    va_end(ap);
}

void changer_couleur_texte(t_texte * texte, int r, int v, int b, int a) {
    texte->couleur.r = r;
    texte->couleur.g = v;
    texte->couleur.b = b;
    texte->couleur.a = a;
}

/**
 * @brief Affiche le texte sur le rendu spécifié.
 * 
 * @param rend Rendu SDL où le texte doit être affiché.
 * @param texte Structure de texte à afficher.
 */

void afficher_texte(SDL_Renderer * rend, t_texte * texte) {
    SDL_Texture * texture = SDL_CreateTextureFromSurface(rend, texte->surface);
    SDL_RenderCopy(rend, texture, NULL, &texte->rect_dst);
    SDL_DestroyTexture(texture);
}

/**
 * @brief Crée une nouvelle structure de texte avec les attributs spécifiés.
 * 
 * @param id Identifiant de la police de caractères à utiliser.
 * @param rouge Valeur rouge de la couleur du texte.
 * @param vert Valeur verte de la couleur du texte.
 * @param bleu Valeur bleue de la couleur du texte.
 * @param alpha Valeur alpha de la couleur du texte.
 * @param x Position en x du texte.
 * @param y Position en y du texte.
 * @param w Largeur du texte.
 * @param h Hauteur du texte.
 * @return Pointeur vers la nouvelle structure de texte créée, NULL en cas d'échec
 *          de récupération de la ressource police.
 */

t_texte * creer_texte(char * id, int rouge, int vert, int bleu, int alpha, int x, int y, int w, int h) {
    TTF_Font * police = recuperer_police(id);
    if (!police) {
        fprintf(stderr, "creer_texte avec police « %s » a échoué\n", id);
        return NULL;
    }
    t_texte * nouv = malloc(sizeof(t_texte));
    nouv->police = police;
    strcpy(nouv->contenu, "");
    nouv->couleur.r = rouge;
    nouv->couleur.g = vert;
    nouv->couleur.b = bleu;
    nouv->couleur.a = alpha;
    nouv->rect_dst.x = x;
    nouv->rect_dst.y = y;
    nouv->rect_dst.w = w;
    nouv->rect_dst.h = h;
    nouv->surface = TTF_RenderText_Solid(nouv->police, nouv->contenu, nouv->couleur);
    return nouv;
}


/**
 * @brief Détruit une structure de texte et libère la mémoire associée.
 * 
 * @param texte Pointeur vers le pointeur de la structure de texte à détruire.
 */

 
void detruire_texte(t_texte ** texte) {
    if (*texte) {
        SDL_FreeSurface((*texte)->surface);
        free(*texte);
    }
    *texte = NULL;
}

/**
 * @brief Affiche un message au milieu de la fenêtre avec un fond noir.
 * 
 * @param rend Pointeur vers le renderer SDL.
 * @param message Message à afficher.
 */

// Fonction pour afficher un message au milieu de la fenêtre avec un fond noir
void afficher_message(SDL_Renderer * rend, const char * message) {
    SDL_Color couleurBlanche = {255, 255, 255}; 

    // Obtenir les dimensions de la fenêtre
    int largeurFenetre, hauteurFenetre;
    SDL_GetRendererOutputSize(rend, &largeurFenetre, &hauteurFenetre);

    // Calculer les dimensions et la position du rectangle noir au milieu de la fenêtre
    int largeurRectangle = largeurFenetre / 2;
    int hauteurRectangle = hauteurFenetre / 4;
    int posXRectangle = (largeurFenetre - largeurRectangle) / 2;
    int posYRectangle = (hauteurFenetre - hauteurRectangle) / 2;

    // Remplir le rectangle avec une couleur noire
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_Rect rectangle = {posXRectangle, posYRectangle, largeurRectangle, hauteurRectangle};
    SDL_RenderFillRect(rend, &rectangle);

    // Créer et afficher le texte au milieu du rectangle noir
    t_texte * texte = creer_texte("police_defaut", 255, 255, 255, 255, posXRectangle, posYRectangle, largeurRectangle, hauteurRectangle);
    if (!texte) {
        fprintf(stderr, "Erreur lors de la création du texte\n");
        return;
    }
    changer_texte(texte, "%s", message);
    afficher_texte(rend, texte);
    detruire_texte(&texte);
}
