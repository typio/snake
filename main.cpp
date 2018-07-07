#include <time.h>
#include <iostream>
#include <tuple>

#include <SDL.h>

// window size
const int SCREEN_WIDTH = 950;
const int SCREEN_HEIGHT = 530;

void drawBackground();
void gameManager();
void close();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

// number of cells in grid dimensions
const int GRID_WIDTH = 82;
const int GRID_HEIGHT = 43;

// exists, xPos, yPos
std::tuple<bool, int, int> food;
int foodXPos, foodYPos;

// padding around game grid
int padding = 25;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &gWindow, &gRenderer) == 0) {
            SDL_bool quit = SDL_FALSE;

            // seed for random food location
            srand(time(NULL));

            while (!quit) {
                SDL_Event e;

                // game
                drawBackground();

                gameManager();

                SDL_RenderPresent(gRenderer);

                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) {
                        quit = SDL_TRUE;
                    }
                }
            }
        }
    }
    close();
    return 0;
}

void drawBackground() {
    // background color
    SDL_SetRenderDrawColor(gRenderer, 0xCE, 0xd6, 0xb5, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(gRenderer);

    // draw grid
    int squareSize = 11;
    int i = padding;
    while (i < SCREEN_WIDTH - padding) {
        int j = padding;
        while (j < SCREEN_HEIGHT - padding) {
            SDL_SetRenderDrawColor(gRenderer, 0xB9, 0xC0, 0xA3, SDL_ALPHA_OPAQUE);
            SDL_Rect outsideRect = {i, j, 10, 10};
            SDL_RenderDrawRect(gRenderer, &outsideRect);

            SDL_Rect insideRect = {i + 2, j + 2, 6, 6};
            SDL_RenderFillRect(gRenderer, &insideRect);
            j += squareSize;
        }
        i += squareSize;
    }
}

void gameManager() {
    // if no food make food
    if (!std::get<0>(food)) {
        food = std::make_tuple(true, rand() % GRID_WIDTH, rand() % GRID_HEIGHT);
        foodXPos = std::get<1>(food) * 11 + padding;
        foodYPos = std::get<2>(food) * 11 + padding;
    }

    // draw food
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_Rect outsideRect = {foodXPos, foodYPos, 10, 10};
    SDL_RenderDrawRect(gRenderer, &outsideRect);

    SDL_Rect insideRect = {foodXPos + 2, foodYPos + 2, 6, 6};
    SDL_RenderFillRect(gRenderer, &insideRect);
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    gWindow = NULL;
    gRenderer = NULL;
    SDL_Quit();
}