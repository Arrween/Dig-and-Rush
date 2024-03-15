#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>

#include "ressources.h"
#include "uthash.h"

t_texture * textures = NULL;
t_spritesheet * spritesheets = NULL;
t_son * sons = NULL;

struct chargement {
    char chemin[TAILLE_MAX_CHEMIN];
    char id[TAILLE_MAX_ID];
};

struct chargement_spritesheet {
    char chemin[TAILLE_MAX_CHEMIN];
    int sprite_l, sprite_h;
    char id[TAILLE_MAX_ID];
};

struct chargement chargements_texs[] = {
    {"ressources/Menu/Background_Menu/Fond_Menu.png", "menu_fond"},
    {"ressources/Menu/Nom_du_Jeu/logo.png", "menu_titre"},
    {"ressources/Menu/Background_Menu/choix_serveur.png", "menu_serveur"},
    {"ressources/Menu/Background_Menu/menu_parametre.png", "menu_paramètres"},
    {"ressources/Menu/Background_Menu/menu_personnages.png", "menu_personnages"},
    {"ressources/Menu/Boutons/bouton_parametre.png", "bouton_paramètres"},
    {"ressources/Menu/Boutons/bouton_volume_on.png", "bouton_volume_on"},
    {"ressources/Menu/Boutons/bouton_volume_off.png", "bouton_volume_off"},
    {"ressources/Menu/Boutons/bouton_personnages.png", "bouton_personnages"},
    {"ressources/Menu/Boutons/bouton_fullscreen.png", "bouton_fullscreen_on"},
    {"ressources/Menu/Boutons/bouton_not_fullscreen.png", "bouton_fullscreen_off"},
    {"ressources/Menu/Boutons/bouton_home.png", "bouton_menu"},
    {"ressources/Menu/Boutons/bouton_play.png", "bouton_jouer"},
    {"ressources/Menu/Boutons/bouton_quit.png", "bouton_quitter"},

    {"ressources/Personnages/jackPerso.png", "jack"},
    {"ressources/img/fond_tour.png", "fond_tour"},
};

struct chargement_spritesheet chargements_spritesheets[] = {
    {"ressources/Personnages/jackPerso.png", 64, 64, "jack"},
};

struct chargement chargements_sons[] = {
    {"ressources/essais_audio/confirmation_001.wav", "essai"},
};

void init_ressources(SDL_Renderer * rend) {
    t_texture * ressource_tex;
    t_spritesheet * ressource_spritesheet;
    t_son * ressource_son;
    struct chargement charg;
    struct chargement_spritesheet charg_sheet;
    unsigned long i;

    for (i = 0; i < sizeof(chargements_texs)/sizeof(*chargements_texs); i++) {
        charg = chargements_texs[i];
        HASH_FIND_STR(textures, charg.id, ressource_tex);
        if (!ressource_tex) {
            ressource_tex = malloc(sizeof(t_texture));
            ressource_tex->texture = IMG_LoadTexture(rend, charg.chemin);
            if (!ressource_tex->texture) {
                fprintf(stderr, "Erreur lors du chargement d’une texture"
                                " : %s\n", IMG_GetError());
            }
            strcpy(ressource_tex->id, charg.id);
            HASH_ADD_STR(textures, id, ressource_tex);
        }
        else {
            printf("Avertissement : ressource texture avec l’id « %s » "
                   "déjà présente, fichier « %s » non chargé\n",
                   charg.id, charg.chemin);
        }
    }

    for (i = 0; i < sizeof(chargements_spritesheets)/sizeof(*chargements_spritesheets); i++) {
        charg_sheet = chargements_spritesheets[i];
        HASH_FIND_STR(spritesheets, charg_sheet.id, ressource_spritesheet);
        if (!ressource_spritesheet) {
            ressource_spritesheet = malloc(sizeof(t_spritesheet));
            ressource_spritesheet->texture = IMG_LoadTexture(rend, charg_sheet.chemin);
            if (!ressource_spritesheet->texture) {
                fprintf(stderr, "Erreur lors du chargement d’une spritesheet"
                            " : %s\n", IMG_GetError());
            }
            ressource_spritesheet->sprite_l = charg_sheet.sprite_l;
            ressource_spritesheet->sprite_h = charg_sheet.sprite_h;
            strcpy(ressource_spritesheet->id, charg_sheet.id);
            HASH_ADD_STR(spritesheets, id, ressource_spritesheet);
        }
        else {
            printf("Avertissement : ressource spritesheet avec l’id « %s » "
                   "déjà présente, fichier « %s » non chargé\n",
                   charg_sheet.id, charg_sheet.chemin);
        }
    }

    for (i = 0; i < sizeof(chargements_sons)/sizeof(*chargements_sons); i++) {
        charg = chargements_sons[i];
        HASH_FIND_STR(sons, charg.id, ressource_son);
        if (!ressource_son) {
            ressource_son = malloc(sizeof(t_son));
            if (!SDL_LoadWAV(charg.chemin, &(ressource_son->spec),
                                &(ressource_son->buffer),
                                &(ressource_son->length))) {
                fprintf(stderr, "Erreur lors du chargement d’un son"
                                " : %s\n", IMG_GetError());
            }
            strcpy(ressource_son->id, charg.id);
            HASH_ADD_STR(sons, id, ressource_son);
        }
        else {
            printf("Avertissement : ressource son avec l’id « %s » "
                   "déjà présente, fichier « %s » non chargé\n",
                   charg.id, charg.chemin);
        }
    }
}

SDL_Texture * recuperer_texture(const char * id) {
    t_texture * ressource;
    HASH_FIND_STR(textures, id, ressource);
    if (ressource)
        return ressource->texture;
    else {
        fprintf(stderr, "Avertissement : récupération d’une "
                "ressource texture inexistante « %s »\n", id);
        return NULL;
    }
}

t_spritesheet * recuperer_spritesheet(const char * id) {
    t_spritesheet * ressource;
    HASH_FIND_STR(spritesheets, id, ressource);
    if (ressource)
        return ressource;
    else {
        fprintf(stderr, "Avertissement : récupération d’une "
                "ressource spritesheet inexistante « %s »\n", id);
        return NULL;
    }
}

t_son * recuperer_son(const char * id) {
    t_son * ressource;
    HASH_FIND_STR(sons, id, ressource);
    if (!ressource)
        fprintf(stderr, "Avertissement : récupération d’une "
                "ressource son inexistante « %s »\n", id);
    return ressource;
}

void detruire_ressources() {
    t_texture * tex_courant, * tex_tmp;
    t_spritesheet * sheet_courant, * sheet_tmp;
    t_son * son_courant, * son_tmp;
    HASH_ITER(hh, textures, tex_courant, tex_tmp) {
        SDL_DestroyTexture(tex_courant->texture);
        free(tex_courant);
    }
    HASH_ITER(hh, spritesheets, sheet_courant, sheet_tmp) {
        SDL_DestroyTexture(sheet_courant->texture);
        free(sheet_courant);
    }
    HASH_ITER(hh, sons, son_courant, son_tmp) {
        SDL_FreeWAV(son_courant->buffer);
        free(son_courant);
    }

}
