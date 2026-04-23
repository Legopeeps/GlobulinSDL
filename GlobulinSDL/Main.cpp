#include "Game.h"

Game* game = nullptr; // Global pointer to the Game instance, allowing access across the application

int main(int argc, char* argv[]) {

    // Heap instantiation of the Game engine.
    game = new Game();

    const int FPS = 60; //enforce a consistent frame, regardless of hardware
    const int frameDelay = 1000 / FPS; // About 16ms per frame
    
    // Window & title formatting
    game->init("Jack Stiller - CGP2015M - 25788983 - Virus Strike: Globulin Offensive",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

	// Main game loop, continues until the user closes the window or the timer runs out.
    while (game->running()) {
        Uint32 frameStart = SDL_GetTicks(); // Record start time

        game->handleEvents();
        game->update();
        game->render();

        // Calculate how long this frame took
        Uint32 frameTime = SDL_GetTicks() - frameStart;

        // If the frame finished too fast, wait until 16ms have passed
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    

    // Explicit resource deallocation to satisfy LO4 (Standard APIs).
    game->clean();

    return 0;
}