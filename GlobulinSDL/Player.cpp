// Player.cpp - Implementation of the Player class, managing player input, state updates, and rendering.
#include "Player.h"

Player::Player(int x, int y, int s) : xPos(x), yPos(y), speed(s) {
    destRect.w = 50;
    destRect.h = 50;
}

void Player::handleInput(const Uint8* keystate) {
    // Movement input handling
    if (keystate[SDL_SCANCODE_W]) yPos -= speed;
    if (keystate[SDL_SCANCODE_S]) yPos += speed;
    if (keystate[SDL_SCANCODE_A]) xPos -= speed;
    if (keystate[SDL_SCANCODE_D]) xPos += speed;
}

void Player::update(int screenWidth, int screenHeight, int border) {
    /* Boundary Limiting logic;
     * Ensures the player's logical coordinates remain within the playArea mentioned in the breif.
     * Prevents going out of bounds.
     */

     // Horizontal Clamping
    if (xPos < border) xPos = border;
    if (xPos > screenWidth - destRect.w - border) xPos = screenWidth - destRect.w - border;

    // Vertical Clamping
    if (yPos < border) yPos = border;
    if (yPos > screenHeight - destRect.h - border) yPos = screenHeight - destRect.h - border;

    // Sync the renderable rectangle with the calculated logical position,
    // even in the event of the screen size changing
    destRect.x = xPos;
    destRect.y = yPos;
}

void Player::render(SDL_Renderer* renderer) {
	// Set the draw color to white and render the player's rectangle at its current position
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &destRect);
}