/**
 *  @file
 *  @brief Implémentation d’un effet de nuit
 *
 *  L’effet consiste à cacher la zone de jeu derrière une texture
 *  remplie de noir à l’exception d’un cercle centré sur le joueur
 *  avec une transition de transparence entre la zone cachée et la
 *  zone visible.
 */

#include "nuit.h"

void calculer_ombre(t_nuit * nuit);

/**
 * @brief alloue la mémoire pour un objet `t_nuit` et initialise ses champs
 * 
 * @param rend Rendu SDL auquel rattacher la texture contenant l’effet de nuit
 * @param perso entité sur laquelle centrer la zone visible
 * @param zone_jeu rectangle auquel appliquer l’effet
 * @param texture_jour texture du fond de jeu pour le mode jour
 * @param texture_jour texture du fond de jeu pour le mode nuit
 * @return l’objet `t_nuit` alloué et initialisé
 */
t_nuit * creer_nuit(SDL_Renderer * rend, t_entite * perso, SDL_FRect zone_jeu,
                    SDL_Texture * texture_jour, SDL_Texture * texture_nuit) {
    t_nuit * nouv = malloc(sizeof(t_nuit));

    nouv->centre = perso;
    nouv->etendue = zone_jeu;
    nouv->texture_jour = texture_jour;
    nouv->texture_nuit = texture_nuit;
    /** activer la transparence des textures de fond de jeu */
    SDL_SetTextureBlendMode(nouv->texture_jour, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(nouv->texture_nuit, SDL_BLENDMODE_BLEND);

    nouv->min_alpha = 0;
    nouv->max_alpha = 255;
    nouv->alpha = nouv->min_alpha;
    nouv->min_rayon = FACTEUR_MIN_RAYON_VISIBLE * nouv->centre->rect_dst->w;
    nouv->max_rayon = MAX_RAYON_VISIBLE;
    nouv->rayon = nouv->min_rayon;

    /** créer la texture utilisée pour dessiner l’effet et activer sa transparence */
    nouv->texture_ombre = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TAILLE_L, TAILLE_H);
    SDL_SetTextureBlendMode(nouv->texture_ombre, SDL_BLENDMODE_BLEND);

    nouv->max_musique = MIX_MAX_VOLUME * FACTEUR_VOLUME_SONS_INI;
    nouv->min_musique = 0;
    nouv->volume_musique_jour = nouv->max_musique;

    /** définir initialement l’effet comme non actif et précédemment actif pour commencer
        * par une transition nuit vers jour
        */
    nouv->est_active = FAUX;
    nouv->est_active_prec = VRAI;

    return nouv;
}

/**
 * @brief basculer le mode jour ou nuit de l’effet pour effectuer une transition 
 * 
 * @param nuit objet d’effet de nuit à modifier
 */
void basculer_nuit(t_nuit * nuit) {
    if (!nuit->est_active) jouer_audio(0, "loup", 0);
    nuit->est_active = !nuit->est_active;
    nuit->est_active_prec = !nuit->est_active;
}

/**
 * @brief décrémenter un paramètre de l’effet de nuit d’un pourcentage de sa plage de valeurs
 * 
 * @param param paramètre à décrémenter
 * @param min valeur minimum du paramètre
 * @param max valeur maximum du paramètre
 * @param pas pas à retirer, en pourcentage
 */
void decrementer_parametre(float * param, float min, float max, float pas) {
    *param = *param - (max - min) * pas/100.;
    if (*param < min)
        *param = min;
}

/**
 * @brief incrémenter un paramètre de l’effet de nuit d’un pourcentage de sa plage de valeurs
 * 
 * @param param paramètre à incrémenter
 * @param min valeur minimum du paramètre
 * @param max valeur maximum du paramètre
 * @param pas pas à ajouter, en pourcentage
 */
void incrementer_parametre(float * param, float min, float max, float pas) {
    *param = *param + (max - min) * pas/100.;
    if (*param > max)
        *param = max;
}

/**
 * @brief effectuer un pas de transition entre le mode jour et le mode nuit
 *        ou l’inverse
 *
 *  Modifier les paramètres de l’effet de nuit par un pas de transition
 *  et actualiser les variables textures et musique en conséquence
 * 
 * @param nuit objet d’effet de nuit sur lequel effectuer la transition
 */
void transitionner_nuit(t_nuit * nuit) {
    // le jour se lève
    if (!nuit->est_active && nuit->est_active_prec) {
        incrementer_parametre(&(nuit->alpha), nuit->min_alpha, nuit->max_alpha, PAS_TRANSITION);
        incrementer_parametre(&(nuit->rayon), nuit->min_rayon, nuit->max_rayon, PAS_TRANSITION);
        incrementer_parametre(&(nuit->volume_musique_jour), nuit->min_musique, nuit->max_musique, PAS_TRANSITION);

        if (nuit->alpha >= nuit->max_alpha && nuit->rayon >= nuit->max_rayon)
            nuit->est_active_prec = FAUX;

        Mix_Volume(CANAL_MUS_JOUR, nuit->volume_musique_jour);
        Mix_Volume(CANAL_MUS_NUIT, nuit->max_musique - nuit->volume_musique_jour);
        SDL_SetTextureAlphaMod(nuit->texture_jour, nuit->alpha);
        SDL_SetTextureAlphaMod(nuit->texture_nuit, nuit->max_alpha - nuit->alpha);

        calculer_ombre(nuit);
    }
    // la nuit tombe
    else if (nuit->est_active && !nuit->est_active_prec) {
        decrementer_parametre(&(nuit->alpha), nuit->min_alpha, nuit->max_alpha, PAS_TRANSITION);
        decrementer_parametre(&(nuit->rayon), nuit->min_rayon, nuit->max_rayon, PAS_TRANSITION);
        decrementer_parametre(&(nuit->volume_musique_jour), nuit->min_musique, nuit->max_musique, PAS_TRANSITION);

        if (nuit->alpha <= nuit->min_alpha && nuit->rayon <= nuit->min_rayon)
            nuit->est_active_prec = VRAI;

        Mix_Volume(CANAL_MUS_JOUR, nuit->volume_musique_jour);
        Mix_Volume(CANAL_MUS_NUIT, nuit->max_musique - nuit->volume_musique_jour);
        SDL_SetTextureAlphaMod(nuit->texture_jour, nuit->alpha);
        SDL_SetTextureAlphaMod(nuit->texture_nuit, nuit->max_alpha - nuit->alpha);

        calculer_ombre(nuit);
    }
    // c’est la nuit, la zone visible doit suivre le centre
    else if (nuit->est_active)
        calculer_ombre(nuit);
}

/**
 * @brief calculer les pixels de la texture d’un effet de nuit
 *
 * @param nuit objet de l’effet de nuit à calculer
 */
void calculer_ombre(t_nuit * nuit) {
    SDL_PixelFormat * format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    Uint32 * pixels;
    int pitch;
    // permettre d’écrire les pixels de la texture
    SDL_LockTexture(nuit->texture_ombre, NULL, (void**)&pixels, &pitch);

    SDL_FRect rect_perso_abs = convertir_vers_absolu(nuit->centre->rect_dst, nuit->etendue);

    for (int i = nuit->etendue.x; i < nuit->etendue.x + nuit->etendue.w; i++) {
        for (int j = nuit->etendue.y; j < nuit->etendue.y + nuit->etendue.h; j++) {
            int dx = (i-(rect_perso_abs.x+rect_perso_abs.w/2));
            int dy = (j-(rect_perso_abs.y+rect_perso_abs.h/2));
            int carre_distance = dx*dx + dy*dy;
            // si la distance euclidienne entre le point (i,j) de la zone
            // de jeu et le centre du personnage est plus grande que `rayon`
            // écrire un pixel d’autant moins transparent qu’il est loin
            if (carre_distance > nuit->rayon*nuit->rayon) {
                float alpha = ((float)(carre_distance - nuit->rayon*nuit->rayon)) / (TAILLE_L/2.*TAILLE_L/2. + TAILLE_H/2.*TAILLE_H/2.) * 255. * FACTEUR_OBSCURCISSEMENT;
                pixels[j * TAILLE_L + i] = SDL_MapRGBA(format, 0, 0, 0, alpha < 255 ? alpha : 255);
            }
            // sinon écrire un pixel transparent
            else
                pixels[j * TAILLE_L + i] = SDL_MapRGBA(format, 0, 0, 0, 0);
        }
    }

    // fin de l’écriture des pixels
    SDL_UnlockTexture(nuit->texture_ombre);
    SDL_FreeFormat(format);
}

/** @brief libérer la mémoire d’un objet `t_nuit`
*   @param nuit pointeur sur pointeur de l’objet à détruire
*/
void detruire_nuit(t_nuit ** nuit) {
    if (*nuit) {
        SDL_DestroyTexture((*nuit)->texture_ombre);
        free(*nuit);
    }
    *nuit = NULL;
}
