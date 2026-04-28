// Game.cpp - Implementation of the Game class, managing the main game loop and 
// the core SDL interactions allowing my game engine to function.
#include "Game.h"


Game::Game() : isRunning(false), window(nullptr), renderer(nullptr),
               isSplashScreen(true), splashTimer(0.0f), gameTimer(0.0f),
               score(0), loggingEnabled(true) {}

Game::~Game() {}

bool Game::init(const char* title, int x, int y, int w, int h, bool fullscreen) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(title, x, y, w, h, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
        renderer = SDL_CreateRenderer(window, -1, 0);

        // Logical scaling to maintain aspect ratio across resolutions.
        SDL_RenderSetLogicalSize(renderer, 800, 600);

        SDL_Surface* tempSurface = SDL_LoadBMP("assets/HowToPlay.bmp");
        if (tempSurface == nullptr) {
            std::cout << "Failed to load BMP: " << SDL_GetError() << std::endl;
        }
        else {
            splashTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
            SDL_FreeSurface(tempSurface);
        }
        player = new Player(400, 300, 5);
        isRunning = true;
    }
    return isRunning;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_f: // Toggle with 'F' key
                isFullscreen = !isFullscreen;
                if (isFullscreen) {
                    // Use SDL_WINDOW_FULLSCREEN_DESKTOP to maintain resolution 
                    // and allow SDL_RenderSetLogicalSize to handle letterboxing
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
                else {
                    SDL_SetWindowFullscreen(window, 0); // Windowed mode
                }
                break;
            }
        }
    }
}

void Game::update() {
    // 5-second Splash Screen
    if (isSplashScreen) {
        splashTimer += 0.016f; 
        if (splashTimer >= splashDuration) 
            isSplashScreen = false;
        return;
    }

	// 60-second game-time period, after which 
    // the game ends and the final score is displayed
    gameTimer += 0.016f;
    if (gameTimer >= 60.0f) 
        isRunning = false;

    // Entity Logic
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    player->handleInput(keystate);
    player->update(800, 600, 20);

    // Beat Spawning: Procedural beat generation
    beatTimer += 0.016f;
    if (beatTimer >= spawnInterval) {
        Beat b;
        b.rect = { rand() % 760, -50, 40, 40 }; // Spawn above view-port
        b.speed = 4.5f;
        b.active = true;
        activeBeats.push_back(b);
        beatTimer = 0.0f;
    }

    // Interaction & collisiion
    for (auto& b : activeBeats) {
        if (b.active) {
            b.rect.y += (int)b.speed;

            SDL_Rect pRect = { player->getX(), player->getY(), 50, 50 };
            if (SDL_HasIntersection(&pRect, &b.rect)) {
                b.active = false;
                score += 100;
                if (loggingEnabled) std::cout << "[LOG] Catch Detected. Score: " << score << std::endl;
            }
            // Memory optimization: flag for removal if missed
            if (b.rect.y > 600) b.active = false;
        }
    }
}

void Game::render() {
    // Clear to dark blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255);
    SDL_RenderClear(renderer);

    if (isSplashScreen) {
        // DRAW THE SPLASH SCREEN
        if (splashTexture != nullptr) {
            SDL_RenderCopy(renderer, splashTexture, NULL, NULL);
        }
    }
    else {
        // DRAW THE GAME
        player->render(renderer);

        // Draw falling beats (Neon Green)
        SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255);
        for (auto& b : activeBeats) {
            if (b.active) {
                SDL_RenderFillRect(renderer, &b.rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    std::cout << "Cleaning game..." << std::endl;
    SDL_DestroyTexture(splashTexture); // Destroy the image
    delete player;                     // Delete the player pointer
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}