// Player.h, the header file for the Player class, defines the structure and functionality of the player entity in the game. 
// It includes methods for handling input, updating state, and rendering the player on the screen.
// The class also manages the player's position, speed, and the rectangle used for rendering.
#pragma once
#include <SDL.h>

class Player {
public:
    Player(int x, int y, int speed);
    ~Player();

    // Updates the internal velocity/direction based on current movement input
    void handleInput(const Uint8* keystate);

    // Handles state updates & boundary clamping
    void update(int screenWidth, int screenHeight, int borderThickness);

    // Dispatches the draw call for the entity to the current render target
    void render(SDL_Renderer* renderer);

    // Getters for the player's private coordinates
    int getX() { return xPos; }
    int getY() { return yPos; }

private:
    int xPos, yPos, speed;   // Base, spatial coordinates
    SDL_Rect destRect; // Mapping of logical coordinates to a renderable area
};