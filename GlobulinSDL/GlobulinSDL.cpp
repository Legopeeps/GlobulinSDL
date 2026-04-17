// GlobulinSDL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <string>
#include <iostream>
#include "SDL.h"
#include "Player.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Virus Strike: Globulin Offensive",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event event;

    

	Player player(400, 300, 1); //player starting position (x, y) and speed

    // Core game loop
    while (running)
    {
        // Player input
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
            
        }

        // Unity-esque update loop area

        // Render onto the window created at start of Main()
		//remember: clear the screen, draw everything, then present the renderer to the window
        SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255);
        SDL_RenderClear(renderer);


        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        player.handleInput(keystate);

        SDL_Rect playerRect = { player.playerX, player.playerY, 50, 50 };

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
        SDL_RenderFillRect(renderer, &playerRect);

        SDL_RenderPresent(renderer);
    }
	// Cleanup and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
