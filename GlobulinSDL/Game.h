// Game.h, the header file for the Game class, defines the core structure and functionality of the game engine, 
// including initialization, event handling, game logic updates, rendering, and cleanup. 
// It also includes necessary headers for SDL, player management, and common game data structures.

#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>
#include "Player.h"
#include "Common.h" //contains beat-related data and constant definitions

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
    bool isRunning;      // Main Game Loop flag
    bool loggingEnabled; // Debug toggle
    bool isFullscreen = false;
	bool isSplashScreen = true; 

    
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* splashTexture;

    float gameTimer;     // To keep to requirement of a 60 second session, tracking elapsed time
    int score;
	float splashTimer = 0.0f; // Timer to track duration of splashscreen
	const float splashDuration = 5.0f; // Duration for which the splash screen is displayed (in seconds)

    SDL_Rect playArea;   // Boundary rectangle defining the playable area

    Player* player;      // Futureproofing to allow multiple entities, derived from "Player" class

    std::vector<Beat> activeBeats;
    float beatTimer = 0.0f;
    float spawnInterval = 0.5f;
};