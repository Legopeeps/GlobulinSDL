#pragma once

#include <iostream>
#include "SDL.h"

class Player {
public: // Constructor to initialize player position and speed
	Player(int x, int y, int speed);
	void handleInput(const Uint8* keystate);
	int playerX;
	int playerY;
	int playerSpeed;
private:
	//sort later
};