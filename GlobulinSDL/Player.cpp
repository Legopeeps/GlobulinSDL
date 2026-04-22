#include <iostream>
#include "SDL.h"
#include "Player.h"

void Player::handleInput(const Uint8* keystate) { // Handle player movement based on keyboard input using WASD keys
	if (keystate[SDL_SCANCODE_W]) playerY -= playerSpeed;
	if (keystate[SDL_SCANCODE_S]) playerY += playerSpeed;
	if (keystate[SDL_SCANCODE_A]) playerX -= playerSpeed;
	if (keystate[SDL_SCANCODE_D]) playerX += playerSpeed;
};
Player::Player(int x, int y, int speed) { // Initialize player position and speed
	playerX = x;
	playerY = y;
	playerSpeed = speed;
};