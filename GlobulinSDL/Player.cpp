#include <iostream>
#include "SDL.h"
#include "Player.h"

void Player::handleInput(const Uint8* keystate) {
	if (keystate[SDL_SCANCODE_W]) playerY -= playerSpeed;
	if (keystate[SDL_SCANCODE_S]) playerY += playerSpeed;
	if (keystate[SDL_SCANCODE_A]) playerX -= playerSpeed;
	if (keystate[SDL_SCANCODE_D]) playerX += playerSpeed;
};
Player::Player(int x, int y, int speed) {
	playerX = x;
	playerY = y;
	playerSpeed = speed;
};