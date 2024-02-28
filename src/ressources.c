#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>

#include "ressources.h"
#include "global.h"
#include "uthash.h"

t_ressource * ressources = NULL;

struct fichier_id {
    char chemin[TAILLE_MAX_CHEMIN];
    char id[TAILLE_MAX_ID];
};

void init_ressources(SDL_Renderer * rend) {
    t_ressource * ressource;
    char chemin[TAILLE_MAX_CHEMIN];
    char id[TAILLE_MAX_ID];

    struct fichier_id fichiers_ids[] = {
        {"ressources/Personnages/jackPerso.png", "jack"},
        {"ressources/Menu/Background_Menu/Fond_Menu.png", "fond_menu"}
    };
    int i;

    for (i = 0; i < sizeof(fichiers_ids)/sizeof(*fichiers_ids); i++) {
        strcpy(chemin, fichiers_ids[i].chemin);
        strcpy(id, fichiers_ids[i].id);
        HASH_FIND_STR(ressources, id, ressource);
        if (!ressource) {
            ressource = malloc(sizeof(t_ressource));
            ressource->texture = IMG_LoadTexture(rend, chemin);
            if (!ressource->texture) {
                fprintf(stderr, "Erreur lors du chargement de « %s » : %s\n",
                        chemin, IMG_GetError());
            }
            strcpy(ressource->id, id);
            HASH_ADD_STR(ressources, id, ressource);
        }
        else {
            printf("Avertissement : ressource avec l’id « %s » "
                   "déjà présente, fichier « %s » non chargé\n", id, chemin);
        }
    }
}

SDL_Texture * recuperer_texture(const char* id) {
    t_ressource * ressource;
    HASH_FIND_STR(ressources, id, ressource);
    if (ressource)
        return ressource->texture;
    else {
        fprintf(stderr, "Avertissement : récupération d’une "
                "ressource inexistante « %s »\n", id);
        return NULL;
    }
}

void detruire_ressources() {
    t_ressource * courant, * tmp;
    HASH_ITER(hh, ressources, courant, tmp) {
        SDL_DestroyTexture(courant->texture);
        free(courant);
    }

}
