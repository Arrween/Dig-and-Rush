#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>

#include "ressources.h"
#include "uthash.h"

t_texture * textures = NULL;
t_spritesheet * spritesheets = NULL;
t_son * sons = NULL;
t_musique * musiques = NULL;
t_police * polices = NULL;

struct chargement {
    char chemin[TAILLE_MAX_CHEMIN];
    char id[TAILLE_MAX_ID];
};

struct chargement_spritesheet {
    char chemin[TAILLE_MAX_CHEMIN];
    char id[TAILLE_MAX_ID];
    t_animation animations[100];
};

struct chargement_police {
    char chemin[TAILLE_MAX_CHEMIN];
    char id[TAILLE_MAX_ID];
    int taille;
};

struct chargement chargements_texs[] = {
    {"ressources/Menu/Background_Menu/Fond_Menu.png", "menu_fond"},
    {"ressources/Menu/Nom_du_Jeu/logo.png", "menu_titre"},
    {"ressources/Menu/Nom_du_Jeu/titre_personnages.png", "titre_personnages"},

    {"ressources/Menu/Background_Menu/choix_serveur.png", "menu_serveur"},
    {"ressources/Menu/Background_Menu/menu_parametre.png", "menu_paramètres"},
    {"ressources/Menu/Background_Menu/menu_personnages.png", "menu_personnages"},
    {"ressources/Menu/Background_Menu/menu_commandes.png", "menu_commandes"},



    {"ressources/Menu/Boutons/bouton_parametre.png", "bouton_paramètres"},
    {"ressources/Menu/Boutons/bouton_volume_on.png", "bouton_volume_on"},
    {"ressources/Menu/Boutons/bouton_volume_off.png", "bouton_volume_off"},
    {"ressources/Menu/Boutons/bouton_personnages.png", "bouton_personnages"},
    {"ressources/Menu/Boutons/bouton_fullscreen.png", "bouton_fullscreen_on"},
    {"ressources/Menu/Boutons/bouton_not_fullscreen.png", "bouton_fullscreen_off"},
    {"ressources/Menu/Boutons/bouton_home.png", "bouton_menu"},
    {"ressources/Menu/Boutons/bouton_play.png", "bouton_jouer"},
    {"ressources/Menu/Boutons/bouton_continue.png", "bouton_continue"},
    {"ressources/Menu/Boutons/bouton_quit.png", "bouton_quitter"},


    {"ressources/Menu/Boutons/bouton_back.png", "back"},


    {"ressources/Menu/Boutons/barre_de_vie_pleine.png", "barre_de_vie_pleine"},
    {"ressources/Menu/Boutons/barre_de_vie_semi.png", "barre_de_vie_semi"},

    {"ressources/Menu/Boutons/energie_pleine.png", "energie_pleine"},
    {"ressources/Menu/Boutons/energie_semi.png", "energie_semi"},



    {"ressources/Menu/Boutons/options_logo.png", "options"},



    {"ressources/Personnages/jackPerso.png", "jack"},
    {"ressources/Tour/Arriere_plan/fond.jpg", "fond_tour"},
    {"ressources/Tour/Arriere_plan/ciel_bleu.png", "fond_jeu"},
    {"ressources/Tour/Arriere_plan/ciel_orange.png", "fond_jeu2"},
    {"ressources/Tour/Arriere_plan/ciel_nuit.png", "fond_jeu_nuit"},
    {"ressources/Tour/Blocs/terre/terre3.png", "bloc_terre"},
    {"ressources/Tour/Blocs/pierres_claires/pierre_herbe_c.png", "bloc_pierre"},
    {"ressources/Tour/pieces.png", "pieces"},
    {"ressources/Tour/coeur.png", "coeur"},

    //Selections personnages
    {"ressources/Personnages/selections/koba.png", "koba"},

};

struct chargement_spritesheet chargements_spritesheets[] = {
    {"ressources/Personnages/matthieuPerso2.png", "matt", 
            {
                {REPOS, 0, 6*64, 64, 64, 0, 0, 1, 1.},
                {CHUTE_G, 5, 1*64, 64, 64, 0, 0, 1, 1.},
                {CHUTE_D, 5, 3*64, 64, 64, 0, 0, 1, 1.},
                {DEPL_G, 0, 9*64, 64, 64, 0, 0, 9, 1/5.},
                {DEPL_D, 0, 11*64, 64, 64, 0, 0, 9, 1/5.},
                {ATTQ_G, 0, 5*64, 64, 64, 0, 0, 8, 1/3.},
                {ATTQ_D, 0, 7*64, 64, 64, 0, 0, 8, 1/3.},
                {CREUSER, 0, 6*64, 64, 64, 0, 0, 8, 1./2},
                {ANIM_MORT, 0, 20*64, 64, 64, 0, 0, 5, 1./2},
                {ANIM_MORT_STATIQUE, 4, 20*64, 64, 64, 0, 0, 1, 1./2},
                {ANIM_NULLE, 0, 0, 0, 0, 0, 0, 0, 0.}
            }
    },
    //Perso 2
    {"ressources/Personnages/jackPerso.png", "jack", 
        {
            {REPOS, 0, 6*64, 64, 64, 0, 0, 1, 1.},
            {CHUTE_G, 5, 1*64, 64, 64, 0, 0, 1, 1.},
            {CHUTE_D, 5, 3*64, 64, 64, 0, 0, 1, 1.},
            {DEPL_G, 0, 9*64, 64, 64, 0, 0, 9, 1/5.},
            {DEPL_D, 0, 11*64, 64, 64, 0, 0, 9, 1/5.},
            {ATTQ_G, 0, 5*64, 64, 64, 0, 0, 8, 1/3.},
            {ATTQ_D, 0, 7*64, 64, 64, 0, 0, 8, 1/3.},
            {CREUSER, 0, 6*64, 64, 64, 0, 0, 8, 1./2},
            {ANIM_MORT, 0, 20*64, 64, 64, 0, 0, 5, 1./2},
            {ANIM_MORT_STATIQUE, 4, 20*64, 64, 64, 0, 0, 1, 1./2},
            {ANIM_NULLE, 0, 0, 0, 0, 0, 0, 0, 0.}
        }
    },
    //Perso 3
    {"ressources/Personnages/yohanPerso.png", "yohan", 
        {
            {REPOS, 0, 6*64, 64, 64, 0, 0, 1, 1.},
            {CHUTE_G, 5, 1*64, 64, 64, 0, 0, 1, 1.},
            {CHUTE_D, 5, 3*64, 64, 64, 0, 0, 1, 1.},
            {DEPL_G, 0, 9*64, 64, 64, 0, 0, 9, 1/5.},
            {DEPL_D, 0, 11*64, 64, 64, 0, 0, 9, 1/5.},
            {ATTQ_G, 0, 5*64, 64, 64, 0, 0, 8, 1/3.},
            {ATTQ_D, 0, 7*64, 64, 64, 0, 0, 8, 1/3.},
            {CREUSER, 0, 6*64, 64, 64, 0, 0, 8, 1./2},
            {ANIM_MORT, 0, 20*64, 64, 64, 0, 0, 5, 1./2},
            {ANIM_MORT_STATIQUE, 4, 20*64, 64, 64, 0, 0, 1, 1./2},
            {ANIM_NULLE, 0, 0, 0, 0, 0, 0, 0, 0.}
        }
    },
    //Perso 4
    {"ressources/Personnages/aniaPerso.png", "ania", 
        {
            {REPOS, 0, 6*64, 64, 64, 0, 0, 1, 1.},
            {CHUTE_G, 5, 1*64, 64, 64, 0, 0, 1, 1.},
            {CHUTE_D, 5, 3*64, 64, 64, 0, 0, 1, 1.},
            {DEPL_G, 0, 9*64, 64, 64, 0, 0, 9, 1/5.},
            {DEPL_D, 0, 11*64, 64, 64, 0, 0, 9, 1/5.},
            {ATTQ_G, 0, 5*64, 64, 64, 0, 0, 8, 1/3.},
            {ATTQ_D, 0, 7*64, 64, 64, 0, 0, 8, 1/3.},
            {CREUSER, 0, 6*64, 64, 64, 0, 0, 8, 1./2},
            {ANIM_MORT, 0, 20*64, 64, 64, 0, 0, 5, 1./2},
            {ANIM_MORT_STATIQUE, 4, 20*64, 64, 64, 0, 0, 1, 1./2},
            {ANIM_NULLE, 0, 0, 0, 0, 0, 0, 0, 0.}
        }
    },
    //Ennemi 1 : Squelette
    {"ressources/Personnages/squelette_craftpix.png", "squelette", 
            {
                {REPOS, 0, 5*128, 128, 128, 0, 0, 1, 1.},
                {DEPL_G, 0, 17*128, 128, 128, 0, 0, 8, 1/2.},
                {DEPL_D, 0, 8*128, 128, 128, 0, 0, 8, 1/2.},
                {ATTQ_G, 0, 10*128, 128, 128, 0, 0, 4, 1/5.},
                {ATTQ_D, 0, 1*128, 128, 128, 0, 0, 4, 1/5.},
                {ANIM_MORT, 0, 3*128, 128, 128, 0, 0, 3, 1/8.},
                {ANIM_MORT_STATIQUE, 2, 3*128, 128, 128, 0, 0, 1, 1},
                {ANIM_NULLE, 0, 0, 0, 0, 0, 0, 0, 0.}
            }
    },

    //Ennemi 2 : Boule de feu
    {"ressources/Personnages/feu.png", "feu", 
            {
                //Les coordonnées sont à modifier
                {REPOS, 0, 2*128, 128, 128, 0, -5, 1, 1.},
                {DEPL_G, 0, 13*128, 128, 128, 0, -5, 7, 1/2.},
                {DEPL_D, 0, 5*128, 128, 128, 0, -5, 7, 1/2.},
                {ATTQ_G, 0, 3*128, 128, 128, 0, -5, 13, 1/3.},
                {ATTQ_D, 0, 11*128, 128, 128, 0, -5, 13, 1/3.},
                {ANIM_MORT, 0, 1*128, 128, 128, 0, -5, 6, 1/8.},
                {ANIM_MORT_STATIQUE, 5, 1*128, 128, 128, 0, -5, 1, 1},
                {ANIM_NULLE, 0, 0, 0, 0, 0, 0, 0, 0.}
            }
    },
    //Ennemi 3 : ONCLE DE JACKKKKKKKKKKITO
    {"ressources/Personnages/onclePerso.png", "oncle", 
            {
                {REPOS, 0, 6*64, 64, 64, 0, 0, 1, 1.},
                {CHUTE_G, 5, 1*64, 64, 64, 0, 0, 1, 1.},
                {CHUTE_D, 5, 3*64, 64, 64, 0, 0, 1, 1.},
                {DEPL_G, 0, 9*64, 64, 64, 0, 0, 9, 1/5.},
                {DEPL_D, 0, 11*64, 64, 64, 0, 0, 9, 1/5.},
                {ATTQ_G, 0, 5*64, 64, 64, 0, 0, 8, 1/3.},
                {ATTQ_D, 0, 7*64, 64, 64, 0, 0, 8, 1/3.},
                {ANIM_MORT, 0, 20*64, 64, 64, 0, 0, 5, 1./2},
                {ANIM_MORT_STATIQUE, 4, 20*64, 64, 64, 0, 0, 1, 1./2},
                {ANIM_NULLE, 0, 0, 0, 0, 0, 0, 0, 0.}
            }
    }
};

struct chargement chargements_sons[] = {
    {"ressources/Audio/SFX/silence.mp3", "silence"},
    {"ressources/Audio/SFX/menu_confirmation.wav", "confirmation"},
    {"ressources/Audio/SFX/coq.mp3", "coq"},
    {"ressources/Audio/SFX/destruction_bloc_1.flac", "destruction_bloc_1"},
    {"ressources/Audio/SFX/destruction_bloc_2.flac", "destruction_bloc_2"},
    {"ressources/Audio/SFX/destruction_bloc_3.flac", "destruction_bloc_3"},
    {"ressources/Audio/Musique/musique_menu.mp3", "musique_jour"},
    {"ressources/Audio/Musique/musique_nuit.mp3", "musique_nuit"},
    {"ressources/Audio/SFX/attaque_squelette.wav", "attaque_squelette"},
    {"ressources/Audio/SFX/coup.mp3", "blessure_perso"},
    {"ressources/Audio/SFX/blessure_perso_f.mp3", "blessure_perso_f"},
    {"ressources/Audio/SFX/swing.mp3", "attaque_perso"},
    {"ressources/Audio/SFX/mort.mp3", "mort_perso"},
    {"ressources/Audio/SFX/mort_perso_f.mp3", "mort_perso_f"},
    {"ressources/Audio/SFX/explosion.mp3", "attaque_feu"},
    {"ressources/Audio/SFX/mort_feu.mp3", "mort_feu"},
    {"ressources/Audio/SFX/mort_squelette.wav", "mort_squelette"},
    {"ressources/Audio/SFX/pickup.mp3", "ramasser_bonus"},
    {"ressources/Audio/SFX/loup.mp3", "loup"},
};

struct chargement chargements_musiques[] = {
    {"ressources/Audio/Musique/musique_menu.mp3", "musique_menu"},
};

struct chargement_police chargements_polices[] = {
    {"ressources/Menu/Police/font1.ttf", "police_defaut", 50},
};


t_animation * recuperer_animation(t_animation ** anims, int n_anims, t_id_anim id) {
    for (int i = 0; i < n_anims; i++) {
        if (anims[i]->id == id)
            return anims[i];
    }
    return NULL;
}

void copier_animation(t_animation * dest, t_animation src) {
    dest->id = src.id;
    dest->x_sprite_ini = src.x_sprite_ini;
    dest->y_sprite = src.y_sprite;
    dest->w_sprite = src.w_sprite;
    dest->h_sprite = src.h_sprite;
    dest->decalage_dest_x = src.decalage_dest_x;
    dest->decalage_dest_y = src.decalage_dest_y;
    dest->longueur = src.longueur;
    dest->vitesse_anim = src.vitesse_anim;
}

/**
 * @brief Initialise les ressources du jeu.
 * @param rend Le pointeur vers le renderer SDL.
 */

void init_ressources(SDL_Renderer * rend) {
    t_texture * ressource_tex;
    t_spritesheet * ressource_spritesheet;
    t_son * ressource_son;
    t_musique * ressource_mus;
    t_police * ressource_police;
    struct chargement charg;
    struct chargement_spritesheet charg_sheet;
    struct chargement_police charg_police;
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
            ressource_spritesheet->animations = NULL;
            int i_anim;
            for (i_anim = 0; charg_sheet.animations[i_anim].id != ANIM_NULLE; i_anim++) {
                ressource_spritesheet->animations = realloc(ressource_spritesheet->animations,
                                                            sizeof(t_animation*) * i_anim + 1);
                ressource_spritesheet->animations[i_anim] = malloc(sizeof(t_animation));
                copier_animation(ressource_spritesheet->animations[i_anim], charg_sheet.animations[i_anim]);
            }
            ressource_spritesheet->n_animations = i_anim + 1;
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
            if (!(ressource_son->tampon = Mix_LoadWAV(charg.chemin)))
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                             "Erreur lors du chargement d’un son"
                             " : %s\n", Mix_GetError());
            strcpy(ressource_son->id, charg.id);
            HASH_ADD_STR(sons, id, ressource_son);
        }
        else {
            printf("Avertissement : ressource son avec l’id « %s » "
                   "déjà présente, fichier « %s » non chargé\n",
                   charg.id, charg.chemin);
        }
    }

    for (i = 0; i < sizeof(chargements_musiques)/sizeof(*chargements_musiques); i++) {
        charg = chargements_musiques[i];
        HASH_FIND_STR(musiques, charg.id, ressource_mus);
        if (!ressource_mus) {
            ressource_mus = malloc(sizeof(t_musique));
            if (!(ressource_mus->tampon = Mix_LoadMUS(charg.chemin)))
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                             "Erreur lors du chargement d’une musique"
                             " : %s\n", Mix_GetError());
            strcpy(ressource_mus->id, charg.id);
            HASH_ADD_STR(musiques, id, ressource_mus);
        }
        else {
            printf("Avertissement : ressource musique avec l’id « %s » "
                   "déjà présente, fichier « %s » non chargé\n",
                   charg.id, charg.chemin);
        }
    }

    for (i = 0; i < sizeof(chargements_polices)/sizeof(*chargements_polices); i++) {
        charg_police = chargements_polices[i];
        HASH_FIND_STR(polices, charg_police.id, ressource_police);
        if (!ressource_police) {
            ressource_police = malloc(sizeof(t_police));
            if (!(ressource_police->police = TTF_OpenFont(charg_police.chemin, charg_police.taille)))
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                             "Erreur lors du chargement d’une police"
                             " : %s\n", TTF_GetError());
            strcpy(ressource_police->id, charg_police.id);
            HASH_ADD_STR(polices, id, ressource_police);
        }
        else {
            printf("Avertissement : ressource police avec l’id « %s » "
                   "déjà présente, fichier « %s » non chargé\n",
                   charg_police.id, charg_police.chemin);
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

t_son * recuperer_son(const char * id, int est_silencieux) {
    t_son * ressource;
    HASH_FIND_STR(sons, id, ressource);
    if (!ressource && !est_silencieux)
        fprintf(stderr, "Avertissement : récupération d’une "
                "ressource son inexistante « %s »\n", id);
    return ressource;
}

t_musique * recuperer_musique(const char * id, int est_silencieux) {
    t_musique * ressource;
    HASH_FIND_STR(musiques, id, ressource);
    if (!ressource && !est_silencieux)
        fprintf(stderr, "Avertissement : récupération d’une "
                "ressource musique inexistante « %s »\n", id);
    return ressource;
}

int jouer_audio(int canal, const char * id, int repetitions) {
    t_son * son = recuperer_son(id, 1);
    if (son) {
        if (Mix_PlayChannel(canal, son->tampon, repetitions) == -1) {
            fprintf(stderr, "Erreur Mix_PlayChannel : %s\n", Mix_GetError());
            return -1;
        }
        return 0;
    }

    t_musique * musique = recuperer_musique(id, 1);
    if (musique) {
        if (Mix_PlayMusic(musique->tampon, repetitions) == -1) {
            fprintf(stderr, "Erreur Mix_PlayMusic : %s\n", Mix_GetError());
            return -1;
        }
        return 0;
    }

    fprintf(stderr, "Avertissement : tentative de jouer une "
            "ressource audio (son ou musique) inexistante « %s »\n", id);
    return -2;
}

TTF_Font * recuperer_police(const char * id) {
    t_police * ressource;
    HASH_FIND_STR(polices, id, ressource);
    if (ressource)
        return ressource->police;
    else {
        fprintf(stderr, "Avertissement : récupération d’une "
                "ressource police inexistante « %s »\n", id);
        return NULL;
    }
}

void detruire_ressources() {
    t_texture * tex_courant, * tex_tmp;
    t_spritesheet * sheet_courant, * sheet_tmp;
    t_son * son_courant, * son_tmp;
    t_musique * mus_courant, * mus_tmp;
    t_police * police_courant, * police_tmp;
    HASH_ITER(hh, textures, tex_courant, tex_tmp) {
        SDL_DestroyTexture(tex_courant->texture);
        free(tex_courant);
    }
    HASH_ITER(hh, spritesheets, sheet_courant, sheet_tmp) {
        SDL_DestroyTexture(sheet_courant->texture);
        // À FAIRE détruire les éléments…
        if (sheet_courant->animations)
            free(sheet_courant->animations);
        free(sheet_courant);
    }
    HASH_ITER(hh, sons, son_courant, son_tmp) {
        Mix_FreeChunk(son_courant->tampon);
        free(son_courant);
    }
    HASH_ITER(hh, musiques, mus_courant, mus_tmp) {
        Mix_FreeMusic(mus_courant->tampon);
        free(mus_courant);
    }
    HASH_ITER(hh, polices, police_courant, police_tmp) {
        TTF_CloseFont(police_courant->police);
        free(mus_courant);
    }
}
