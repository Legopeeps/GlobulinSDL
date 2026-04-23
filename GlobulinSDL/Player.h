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
    int xPos, yPos;   // Base, spatial coordinates
    int speed;        
    SDL_Rect destRect; // Mapping of logical coordinates to a renderable area
};