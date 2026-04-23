// Game.cpp - Implementation of the Game class, managing the main game loop and 
// the core SDL interactions allowing my game engine to function.
#include "Game.h"


Game::Game() : isRunning(false), window(nullptr), renderer(nullptr) {}
Game::~Game() {}

bool Game::init(const char* title, int x, int y, int w, int h, bool fullscreen) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(title, x, y, w, h, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
        renderer = SDL_CreateRenderer(window, -1, 0);

        // Logical scaling to maintain aspect ratio across resolutions.
        SDL_RenderSetLogicalSize(renderer, 800, 600);

        player = new Player(400, 300, 5);
        isRunning = true;
    }
    return isRunning;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_f: // Toggle with 'F' key
                isFullscreen = !isFullscreen;
                if (isFullscreen) {
                    // Use SDL_WINDOW_FULLSCREEN_DESKTOP to maintain resolution 
                    // and allow SDL_RenderSetLogicalSize to handle letterboxing
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
                else {
                    SDL_SetWindowFullscreen(window, 0); // Windowed mode
                }
                break;
            }
        }
    }
}

void Game::update() {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    player->handleInput(keystate);
    player->update(800, 600, 15); // Update with boundary constraints.
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255); // Background clear.
    SDL_RenderClear(renderer);

    player->render(renderer);

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}