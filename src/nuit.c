#include "nuit.h"

void calculer_ombre(t_nuit * nuit);

t_nuit * creer_nuit(SDL_Renderer * rend, t_entite * perso, SDL_FRect zone_jeu,
                    SDL_Texture * texture_jour, SDL_Texture * texture_nuit) {
    t_nuit * nouv = malloc(sizeof(t_nuit));

    nouv->centre = perso;
    nouv->etendue = zone_jeu;
    nouv->texture_jour = texture_jour;
    nouv->texture_nuit = texture_nuit;
    SDL_SetTextureBlendMode(nouv->texture_jour, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(nouv->texture_nuit, SDL_BLENDMODE_BLEND);

    nouv->min_alpha = 0;
    nouv->max_alpha = 255;
    nouv->alpha = nouv->min_alpha;
    nouv->min_rayon = FACTEUR_MIN_RAYON_OMBRE * nouv->centre->rect_dst->w;
    nouv->max_rayon = MAX_RAYON_OMBRE;
    nouv->rayon = nouv->min_rayon;

    nouv->texture_ombre = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TAILLE_L, TAILLE_H);
    SDL_SetTextureBlendMode(nouv->texture_ombre, SDL_BLENDMODE_BLEND);

    nouv->max_musique = MIX_MAX_VOLUME * FACTEUR_VOLUME_SONS_INI;
    nouv->min_musique = 0;
    nouv->volume_musique_jour = nouv->max_musique;

    nouv->est_active = FAUX;
    nouv->est_active_prec = VRAI;

    return nouv;
}

void basculer_nuit(t_nuit * nuit) {
    nuit->est_active = !nuit->est_active;
    nuit->est_active_prec = !nuit->est_active;
}

void decrementer_parametre(float * param, float min, float max) {
    *param = *param - (max - min) * PAS_TRANSITION/100.;
    if (*param < min)
        *param = min;
}

void incrementer_parametre(float * param, float min, float max) {
    *param = *param + (max - min) * PAS_TRANSITION/100.;
    if (*param > max)
        *param = max;
}

void transitionner_nuit(t_nuit * nuit) {
    // le jour se lève
    if (!nuit->est_active && nuit->est_active_prec) {
        incrementer_parametre(&(nuit->alpha), nuit->min_alpha, nuit->max_alpha);
        incrementer_parametre(&(nuit->rayon), nuit->min_rayon, nuit->max_rayon);
        incrementer_parametre(&(nuit->volume_musique_jour), nuit->min_musique, nuit->max_musique);

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
        decrementer_parametre(&(nuit->alpha), nuit->min_alpha, nuit->max_alpha);
        decrementer_parametre(&(nuit->rayon), nuit->min_rayon, nuit->max_rayon);
        decrementer_parametre(&(nuit->volume_musique_jour), nuit->min_musique, nuit->max_musique);

        if (nuit->alpha <= nuit->min_alpha && nuit->rayon <= nuit->min_rayon)
            nuit->est_active_prec = VRAI;

        Mix_Volume(CANAL_MUS_JOUR, nuit->volume_musique_jour);
        Mix_Volume(CANAL_MUS_NUIT, nuit->max_musique - nuit->volume_musique_jour);
        SDL_SetTextureAlphaMod(nuit->texture_jour, nuit->alpha);
        SDL_SetTextureAlphaMod(nuit->texture_nuit, nuit->max_alpha - nuit->alpha);

        calculer_ombre(nuit);
    }
    else if (nuit->est_active)
        calculer_ombre(nuit);
}

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

void detruire_nuit(t_nuit ** nuit) {
    if (*nuit)
        free(*nuit);
    *nuit = NULL;
}
