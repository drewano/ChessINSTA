#include <SDL.h>
#include <SDL2/SDL.h>
#include <SDL_main.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BOARD_SIZE 8
#define TILE_SIZE 80

typedef struct {
    char type;  // 'R' pour Reine, 'T' pour Tour, 'F' pour Fou, 'C' pour Cheval
    int player; // 1 ou 2 pour les deux joueurs
} Piece;

Piece board[BOARD_SIZE][BOARD_SIZE];
int currentPlayer = 1;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_Texture *whiteRook;
SDL_Texture *blackRook;
SDL_Texture *whiteKnight;
SDL_Texture *blackKnight;
SDL_Texture *whiteBishop;
SDL_Texture *blackBishop;
SDL_Texture *whiteQueen;
SDL_Texture *blackQueen;

void initBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j].type = ' ';
            board[i][j].player = 0;
        }
    }

    board[0][0].type = 'T'; board[0][0].player = 1;
    board[0][1].type = 'C'; board[0][1].player = 1;
    board[0][2].type = 'F'; board[0][2].player = 1;
    board[0][3].type = 'R'; board[0][3].player = 1;

    board[7][0].type = 'T'; board[7][0].player = 2;
    board[7][1].type = 'C'; board[7][1].player = 2;
    board[7][2].type = 'F'; board[7][2].player = 2;
    board[7][3].type = 'R'; board[7][3].player = 2;
}

SDL_Texture* loadTexture(const char* path) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void loadMedia() {
    whiteRook = loadTexture("white_rook.png");
    blackRook = loadTexture("black_rook.png");
    whiteKnight = loadTexture("white_knight.png");
    blackKnight = loadTexture("black_knight.png");
    whiteBishop = loadTexture("white_bishop.png");
    blackBishop = loadTexture("black_bishop.png");
    whiteQueen = loadTexture("white_queen.png");
    blackQueen = loadTexture("black_queen.png");
}

void drawPiece(char type, int player, int x, int y) {
    SDL_Rect destRect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_Texture* texture = NULL;

    if (type == 'T') {
        texture = (player == 1) ? whiteRook : blackRook;
    } else if (type == 'C') {
        texture = (player == 1) ? whiteKnight : blackKnight;
    } else if (type == 'F') {
        texture = (player == 1) ? whiteBishop : blackBishop;
    } else if (type == 'R') {
        texture = (player == 1) ? whiteQueen : blackQueen;
    }

    if (texture != NULL) {
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
    }
}

void drawBoard() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255);
            }
            SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            SDL_RenderFillRect(renderer, &tile);

            if (board[i][j].player != 0) {
                drawPiece(board[i][j].type, board[i][j].player, j, i);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

bool saveGame(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return false;
    }

    fwrite(board, sizeof(Piece), BOARD_SIZE * BOARD_SIZE, file);
    fwrite(&currentPlayer, sizeof(int), 1, file);

    fclose(file);
    return true;
}

bool loadGame(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return false;
    }

    fread(board, sizeof(Piece), BOARD_SIZE * BOARD_SIZE, file);
    fread(&currentPlayer, sizeof(int), 1, file);

    fclose(file);
    return true;
}

void makeMove(int startX, int startY, int endX, int endY) {
    board[endX][endY] = board[startX][startY];
    board[startX][startY].type = ' ';
    board[startX][startY].player = 0;
}

void handleEvent(SDL_Event *e) {
    static int startX = -1, startY = -1;
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        int x = e->button.x / TILE_SIZE;
        int y = e->button.y / TILE_SIZE;

        if (startX == -1 && startY == -1) {
            startX = x;
            startY = y;
        } else {
            makeMove(startX, startY, x, y);
            currentPlayer = (currentPlayer == 1) ? 2 : 1;
            startX = -1;
            startY = -1;
        }
    }
}

void close() {
    SDL_DestroyTexture(whiteRook);
    SDL_DestroyTexture(blackRook);
    SDL_DestroyTexture(whiteKnight);
    SDL_DestroyTexture(blackKnight);
    SDL_DestroyTexture(whiteBishop);
    SDL_DestroyTexture(blackBishop);
    SDL_DestroyTexture(whiteQueen);
    SDL_DestroyTexture(blackQueen);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Chess Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BOARD_SIZE * TILE_SIZE, BOARD_SIZE * TILE_SIZE, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    loadMedia();
    initBoard();

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else {
                handleEvent(&e);
            }
        }

        drawBoard();
    }

    close();
    return 0;
}
