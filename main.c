
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Créer une structure pour gérer l'état du jeu
typedef struct {
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
} GameState;

// Fonctions pour initialiser, mettre à jour et rendre l'état du jeu
bool initGame(GameState* game) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return false;
    }

    game->window = SDL_CreateWindow("Jeu d'Échecs", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    if (!game->window) {
        printf("Erreur de création de la fenêtre: %s\n", SDL_GetError());
        return false;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!game->renderer) {
        printf("Erreur de création du renderer: %s\n", SDL_GetError());
        return false;
    }

    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de TTF: %s\n", TTF_GetError());
        return false;
    }

    game->font = TTF_OpenFont("arial.ttf", 24);
    if (!game->font) {
        printf("Erreur de chargement de la police: %s\n", TTF_GetError());
        return false;
    }

    game->isRunning = true;
    return true;
}

void updateGame(GameState* game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game->isRunning = false;
        }
    }
}

void renderGame(GameState* game) {
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255); // Blanc
    SDL_RenderClear(game->renderer);

    // TODO: Ajouter le rendu pour l'échiquier, les pièces, et l'interface utilisateur

    SDL_RenderPresent(game->renderer);
}

void destroyGame(GameState* game) {
    TTF_CloseFont(game->font);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    GameState game;
    if (!initGame(&game)) {
        return -1;
    }

    while (game.isRunning) {
        updateGame(&game);
        renderGame(&game);
    }

    destroyGame(&game);
    return 0;
}
