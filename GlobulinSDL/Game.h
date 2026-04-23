#pragma once
#include <SDL.h>
#include <iostream>
#include "Player.h"

class Game {
public:
    Game();
    ~Game();

    // Initializes SDL subsystems and creates the rendering context
    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    // Abstracted Game-Loop stages
	void handleEvents(); // Processes SDL events and updates input states
	void update();       // Updates game logic, including player state and session timer
	void render();       // Clears the screen, draws the player, and presents the rendered frame to the window
	void clean();        // Frees resources and shuts down SDL subsystems

    bool running() { return isRunning; }

private:
	bool isRunning;      // True/False flag to control the main game loop execution
    bool loggingEnabled; // Toggle for debug messages in console
    bool isFullscreen = false;

    SDL_Window* window;
    SDL_Renderer* renderer;

    float gameTimer;     // To keep to requirement of a 60 second session, tracking elapsed time
    int score;

    SDL_Rect playArea;   // Boundary rectangle defining the playable area

    Player* player;      // Futureproofing to allow multiple entities, derived from "Player" class
};