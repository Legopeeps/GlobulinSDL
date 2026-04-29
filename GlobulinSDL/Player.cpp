// Player.cpp - Implementation of the Player class, managing player input, state updates, and rendering.
#include "Player.h"

Player::Player(int x, int y, int s) : xPos(x), yPos(y), speed(s) {
    destRect.w = 50;
    destRect.h = 50;}

Player::~Player() {}

void Player::handleInput(const Uint8* keystate) {
    // Normal movement
    if (keystate[SDL_SCANCODE_A]) xPos -= speed;
    if (keystate[SDL_SCANCODE_D]) xPos += speed;

    //  Dash movement
    if (keystate[SDL_SCANCODE_LSHIFT] && !isDashing && dashCooldownTimer <= 0) {
        isDashing = true;
        dashTimer = DASH_DURATION;
        dashCooldownTimer = DASH_COOLDOWN;

        // Determine dash direction based on last movement
        if (keystate[SDL_SCANCODE_A]) dashDir = -1;
        else if (keystate[SDL_SCANCODE_D]) dashDir = 1;
        else dashDir = 0; // Dash in place or forward if you prefer
    }
}

void Player::update(int screenWidth, int screenHeight, int border) {
    if (isDashing) {
        xPos += (speed * DASH_SPEED_MULT) * dashDir;
        dashTimer -= 0.016f;

        if (dashTimer <= 0) {
            isDashing = false;
        }
    }

    if (dashCooldownTimer > 0) {
        dashCooldownTimer -= 0.016f;
    }
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