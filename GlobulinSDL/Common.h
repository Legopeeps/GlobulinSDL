// Common.h, used for defining the Beat struct and game constants, 
// included in Game.h to allow both the Game class and any other relevant classes (like Player) 
// to access beat-related data and constants without circular dependencies.
#pragma once
#include <SDL.h>

struct Beat {
    SDL_Rect rect;
    bool active;
    float speed;
};

const int FPS = 60; //enforce a consistant framerate, regardless of hardware
const int frameDelay = 1000 / FPS; // About 16ms per frame
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BORDER_THICKNESS = 30;