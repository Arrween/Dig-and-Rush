#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "constantes.h"
#include "tour.h"
#include "menu.h"
#include "ressources.h"

int main() {
    SDL_Window * fenetre;
    SDL_Renderer * rend;
    SDL_Event event;


    initialiser_sdl();

    // Initialisation de SDL_image pour le support des images PNG
    initialiser_sdl_img();

    // Création de la fenêtre
    fenetre = creation_fenetre();

    // Création du renderer
    rend = creation_renderer(&fenetre);
    SDL_RenderSetLogicalSize(rend, TAILLE_L, TAILLE_H);

    init_ressources(rend);
        
    t_son * son_essai = recuperer_son("essai");
    SDL_AudioDeviceID audio_device = SDL_OpenAudioDevice(NULL, 0, &(son_essai->spec), NULL, 0);
    uint8_t * buffer_essai_mix =  SDL_calloc(1, son_essai->length);
    SDL_MixAudioFormat(buffer_essai_mix, son_essai->buffer, son_essai->spec.format, son_essai->length, SDL_MIX_MAXVOLUME/20);
    SDL_PauseAudioDevice(audio_device, 0);

    t_bouton btn_titre = { recuperer_texture("menu_titre"),
            {TAILLE_L * (0.5 - 0.238),
                TAILLE_H * (0.5 - 0.42),
                TAILLE_L * 0.476,
                TAILLE_H * 0.417},
            action_nulle,
            "TITRE"
        };
    t_bouton btn_parametres = { recuperer_texture("bouton_paramètres"),
            {TAILLE_L * 0.012,
                TAILLE_H * 0.021,
                TAILLE_L * 0.059,
                TAILLE_H * 0.104},
            action_parametres,
            "PARAMÈTRES"
        };
    t_bouton btn_volume = { recuperer_texture("bouton_volume_off"),
            {TAILLE_L * 0.095,
                TAILLE_H * 0.010,
                TAILLE_L * 0.065,
                TAILLE_H * 0.125},
            action_volume,
            "VOLUME"
        };
    t_bouton btn_personnages = { recuperer_texture("bouton_personnages"),
            {TAILLE_L * 0.761,
                TAILLE_H * 0.010,
                TAILLE_L * 0.065,
                TAILLE_H * 0.125},
            action_personnages,
            "PERSONNAGES"
        };
    t_bouton btn_fullscreen = { recuperer_texture("bouton_fullscreen_on"),
            {TAILLE_L * 0.892,
                TAILLE_H * 0.017,
                TAILLE_L * 0.065,
                TAILLE_H * 0.114},
            action_fullscreen,
            "FULLSCREEN"
        };
    t_bouton btn_menu = { recuperer_texture("bouton_menu"),
            {TAILLE_L * 0.012,
                TAILLE_H * 0.87,
                TAILLE_L * 0.065,
                TAILLE_H * 0.114},
            action_home,
            "HOME"
        };
    t_bouton btn_jouer = { recuperer_texture("bouton_jouer"),
            {TAILLE_L * 0.67,
                TAILLE_H * 0.7,
                TAILLE_L * 0.201,
                TAILLE_H * 0.154},
            action_jouer,
            "PLAY"
        };
    t_bouton btn_quitter = { recuperer_texture("bouton_quitter"),
            {TAILLE_L * 0.12,
                TAILLE_H * 0.7,
                TAILLE_L * 0.201,
                TAILLE_H * 0.154},
            action_quitter,
            "QUIT"
        };

    t_bouton * menus[4][10] = {
        {&btn_fullscreen, &btn_volume, &btn_jouer, &btn_quitter, &btn_parametres, &btn_personnages, &btn_titre, NULL}, // menu principal
        {&btn_fullscreen, &btn_volume, &btn_menu, &btn_parametres, &btn_personnages, NULL}, // menu serveur
        {&btn_fullscreen, &btn_volume, &btn_menu, &btn_personnages, NULL}, // menu paramètres
        {&btn_fullscreen, &btn_volume, &btn_menu, &btn_parametres, NULL} // menu personnages
    };
    SDL_Texture * fonds_menus[4] = {
        recuperer_texture("menu_fond"),
        recuperer_texture("menu_serveur"),
        recuperer_texture("menu_paramètres"),
        recuperer_texture("menu_personnages")
    };

    t_etat etat = {
        FAUX,
        FAUX,
        PAGE_MENU,
        menus[PAGE_MENU],
        fenetre
    };

    int i_btn;
    t_bouton * btn;
    while (!etat.doit_quitter) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    etat.doit_quitter = VRAI;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        SDL_Point pointeur = {event.button.x, event.button.y};
                        for (i_btn = 0; (btn = menus[etat.i_menu][i_btn]); i_btn++) {
                            if (SDL_PointInRect(&pointeur, &btn->rect)) {
                                btn->action(&etat);
                                printf("Bouton %s cliqué\n", btn->nom);
                            }
                        }
                    }
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                        case SDL_SCANCODE_Q:
                            etat.doit_quitter = VRAI;
                            break;
                        case SDL_SCANCODE_A:
                            boucle_jeu(rend);
                            break;
                        case SDL_SCANCODE_E:
                            SDL_QueueAudio(audio_device, buffer_essai_mix, son_essai->length);
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }
        
        // Mettre à jour l'écran
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, fonds_menus[etat.i_menu], NULL, NULL);
        for (i_btn = 0; (btn = menus[etat.i_menu][i_btn]); i_btn++)
            SDL_RenderCopy(rend, btn->texture, NULL, &btn->rect);
        SDL_RenderPresent(rend);
        SDL_Delay(1000 / FPS);
    }
//
    // Nettoyage

    detruire_ressources();

    SDL_CloseAudioDevice(audio_device);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(fenetre);
    IMG_Quit();
    SDL_Quit();
    exit(EXIT_SUCCESS);
}
