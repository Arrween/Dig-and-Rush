/**
 * @file test_unit.c
 * @brief Fichier contenant les tests des fonctions de gestion des ressources du jeu.
 */

#include <CUnit/Basic.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "ressources.h" // Fichier contenant les fonctions de gestion des ressources



SDL_Renderer* renderer = NULL; // Renderer global utilisé dans les tests

/**
 * @brief Initialisation de la suite de tests.
 * 
 * Cette fonction initialise les bibliothèques SDL ainsi que le renderer nécessaire
 * pour les tests nécessitant du rendu.
 * 
 * @return 0 si l'initialisation réussit, -1 sinon.
 */

int initialiser_suite(void) {
    // Initialisation de SDL et ses extensions
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 ||
        IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG ||
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 ||
        TTF_Init() == -1) {
        return -1; // Indique un échec d'initialisation
    }

    // Création d'une fenêtre et d'un renderer pour les tests nécessitant du rendu
    SDL_Window* fenetre = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_HIDDEN);
    if (!fenetre) return -1;

    renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return -1;

    return 0; // Initialisation réussie
}

/**
 * @brief Nettoyage après l'exécution des tests.
 * 
 * Cette fonction nettoie les ressources SDL utilisées par les tests.
 * 
 * @return 0 si le nettoyage réussit, -1 sinon.
 */

int nettoyer_suite(void) {
    // Nettoyage des ressources SDL
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    SDL_Quit();
    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
    return 0;
}


/**
 * @brief Test de chargement des ressources.
 * 
 * Cette fonction teste le chargement des différentes ressources du jeu.
 */
void test_chargement_ressources(void) {
    init_ressources(renderer);
    // Tests de récupération de ressources
    CU_ASSERT_PTR_NOT_NULL_FATAL(recuperer_texture("menu_fond"));
    CU_ASSERT_PTR_NOT_NULL_FATAL(recuperer_spritesheet("matt"));
    CU_ASSERT_PTR_NOT_NULL(recuperer_son("confirmation", 0));
    CU_ASSERT_PTR_NOT_NULL(recuperer_musique("musique_menu", 0));
    CU_ASSERT_PTR_NOT_NULL(recuperer_police("police_defaut"));
}

/**
 * @brief Test de destruction des ressources.
 * 
 * Cette fonction teste la destruction des ressources chargées par le jeu.
 */
void test_detruire_ressources(void){
    detruire_ressources();
}

/**
 * @brief Fonction principale.
 * 
 * Cette fonction initialise les tests, ajoute les tests à une suite, exécute les tests
 * et nettoie après leur exécution.
 * 
 * @return Le code d'erreur des tests.
 */
int main() {
    CU_pSuite pSuite = NULL;

    // Initialisation de CUnit
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("Suite_Test_Ressources", initialiser_suite, nettoyer_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Ajout des tests à la suite
    if ((NULL == CU_add_test(pSuite, "test du chargement des ressources", test_chargement_ressources)) ||
        (NULL == CU_add_test(pSuite, "test de destruction des ressources", test_detruire_ressources))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Exécution des tests
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
