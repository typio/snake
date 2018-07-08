#include <time.h>
#include <algorithm>
#include <iostream>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

// window size
const int SCREEN_WIDTH = 950;
const int SCREEN_HEIGHT = 530;

void drawBackground();
void gameManager();
void close();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Surface* gScreenSurface = NULL;

const int FPS = 60;
const int frameDelay = 1000 / FPS;

Uint32 frameStart;
int frameTime;

int frameCount = 0;

// number of cells in grid dimensions
const int GRID_WIDTH = 82;
const int GRID_HEIGHT = 43;

// padding around game grid
int padding = 25;

// representing food
bool foodExists;
int foodXPos, foodYPos;

// representing the snake
std::vector<int> snakeXPos;
std::vector<int> snakeYPos;

// directions
int dir = 2;
int dx[] = {0, 0, 1, -1}, dy[] = {1, -1, 0, 0};

// length of snake
int len;

SDL_Surface* gCurrentSurface = NULL;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &gWindow,
                                        &gRenderer) == 0) {
            SDL_bool quit = SDL_FALSE;

            // seed for random food location
            srand(time(NULL));

            SDL_Surface* icon = IMG_Load("./icon.png");
            SDL_SetWindowIcon(gWindow, icon);

            snakeXPos.push_back(2);
            snakeYPos.push_back(2);

            while (!quit) {
                len = snakeXPos.size();

                frameStart = SDL_GetTicks();

                SDL_Event e;

                // game
                if (frameCount % 5 == 0) {
                    drawBackground();

                    // check if snake is out of bounds
                    if (snakeXPos[0] + dx[dir] < 0 ||
                        snakeXPos[0] + dx[dir] > 81 ||
                        snakeYPos[0] + dy[dir] < 0 ||
                        snakeYPos[0] + dy[dir] > 43) {
                        quit = SDL_TRUE;
                        break;
                    }
                    gameManager();
                }

                frameTime = SDL_GetTicks() - frameStart;

                if (frameDelay > frameTime) {
                    SDL_Delay(frameDelay - frameTime);
                }

                frameCount++;

                SDL_RenderPresent(gRenderer);

                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) {
                        quit = SDL_TRUE;
                    } else if (e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                            case SDLK_UP:
                                if (dir != 0) {
                                    dir = 1;
                                }
                                break;
                            case SDLK_DOWN:
                                if (dir != 1) {
                                    dir = 0;
                                }
                                break;
                            case SDLK_LEFT:
                                if (dir != 2) {
                                    dir = 3;
                                }
                                break;
                            case SDLK_RIGHT:
                                if (dir != 3) {
                                    dir = 2;
                                }
                                break;
                            default:
                                break;
                        }
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
            SDL_SetRenderDrawColor(gRenderer, 0xB9, 0xC0, 0xA3,
                                   SDL_ALPHA_OPAQUE);
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
    // moving snake
    snakeXPos.insert(snakeXPos.begin(), snakeXPos[0] + dx[dir]);
    snakeYPos.insert(snakeYPos.begin(), snakeYPos[0] + dy[dir]);

    if (snakeXPos[0] == foodXPos && snakeYPos[0] == foodYPos) {
        foodExists = false;
    } else {
        snakeXPos.pop_back();
        snakeYPos.pop_back();
    }

    // drawing snake
    for (auto n = 0; n < snakeXPos.size(); n++) {
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        SDL_Rect outsideRect = {snakeXPos[n] * 11 + padding,
                                snakeYPos[n] * 11 + padding, 10, 10};
        SDL_RenderDrawRect(gRenderer, &outsideRect);

        SDL_Rect insideRect = {snakeXPos[n] * 11 + padding + 2,
                               snakeYPos[n] * 11 + padding + 2, 6, 6};
        SDL_RenderFillRect(gRenderer, &insideRect);
    }

    // if no food make food
    if (!foodExists) {
        foodExists = true;
        foodXPos = rand() % GRID_WIDTH;
        foodYPos = rand() % GRID_HEIGHT;
    }

    // draw food
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_Rect outsideRect = {foodXPos * 11 + padding, foodYPos * 11 + padding, 10,
                            10};
    SDL_RenderDrawRect(gRenderer, &outsideRect);

    SDL_Rect insideRect = {foodXPos * 11 + padding + 2,
                           foodYPos * 11 + padding + 2, 6, 6};
    SDL_RenderFillRect(gRenderer, &insideRect);
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    gWindow = NULL;
    gRenderer = NULL;
    SDL_Quit();
}