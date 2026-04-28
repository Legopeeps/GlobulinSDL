// Game.cpp - Implementation of the Game class, managing the main game loop and 
// the core SDL interactions allowing my game engine to function.
#include "Game.h"

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr),
isSplashScreen(true), splashTimer(0.0f), gameTimer(0.0f),
score(0), loggingEnabled(true), musicStarted(false) {
}

Game::~Game() {}

bool Game::init(const char* title, int x, int y, int w, int h, bool fullscreen) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(title, x, y, w, h, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
        renderer = SDL_CreateRenderer(window, -1, 0);

        // Logical scaling to maintain aspect ratio across resolutions.
        SDL_RenderSetLogicalSize(renderer, 800, 600);

        // Initialize audio system for music and sound effects.
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cout << "Mixer Initialization Error: " << Mix_GetError() << std::endl;
        }

		// Initialize TTF for text rendering (score display, timer, etc.)
        if (TTF_Init() == -1) {
            std::cout << "TTF Init Error: " << TTF_GetError() << std::endl;
        }

        // Load splash screen asset and music track.
        SDL_Surface* tempSurface = SDL_LoadBMP("assets/HowToPlay.bmp");
        if (tempSurface) {
            splashTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
            SDL_FreeSurface(tempSurface);
        }

        backgroundMusic = Mix_LoadMUS("assets/gameMusic.mp3");

        // Spawn player at baseline for catch-style gameplay.
        player = new Player(400, 530, 10);
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
            case SDLK_f: // Toggle fullscreen mode.
                isFullscreen = !isFullscreen;
                SDL_SetWindowFullscreen(window, isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
                break;
            case SDLK_l: // Toggle diagnostic logging.
                loggingEnabled = !loggingEnabled;
                break;
            }
        }
    }
}

void Game::update() {
    // 5-second splash screen period for instructions.
    if (isSplashScreen) {
        splashTimer += 0.016f;
        if (splashTimer >= 5.0f)
            isSplashScreen = false;
        return;
    }

    // Start background music loop once gameplay begins.
    if (!musicStarted) {
        Mix_PlayMusic(backgroundMusic, -1);
        musicStarted = true;
    }

    // 60-second game-time period, after which music stops and game ends.
    gameTimer += 0.016f;
    if (gameTimer >= 60.0f) {
        Mix_HaltMusic();
        isRunning = false;
    }

    // Process player movement and boundary constraints.
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    player->handleInput(keystate);
    player->update(800, 600, 20);

    // Procedural beat generation based on spawn intervals.
    beatTimer += 0.016f;
    if (beatTimer >= spawnInterval) {
        Beat b;
        b.rect = { rand() % 760, -50, 40, 40 };
        b.speed = 4.5f;
        b.active = true;
        activeBeats.push_back(b);
        beatTimer = 0.0f;
    }

    // Linear virus movement and collision detection.
    for (auto& b : activeBeats) {
        if (b.active) {
            b.rect.y += (int)b.speed;

            SDL_Rect pRect = { player->getX(), player->getY(), 50, 50 };
            if (SDL_HasIntersection(&pRect, &b.rect)) {
                b.active = false;
                score += 100;
                if (loggingEnabled) std::cout << "[LOG] Virus Caught. Score: " << score << std::endl;
            }
            if (b.rect.y > 600) b.active = false;
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255); // Background clear.
    SDL_RenderClear(renderer);

    if (isSplashScreen) {
        // Draw the instruction graphic during the splash state.
        SDL_RenderCopy(renderer, splashTexture, NULL, NULL);
    }
    else {
        // Render gameplay entities and falling viruses.
        player->render(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255);
        for (auto& b : activeBeats) {
            if (b.active) SDL_RenderFillRect(renderer, &b.rect);
        }
    }

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    // Explicit deallocation of textures, music, and heap-allocated objects.
    Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();
    SDL_DestroyTexture(splashTexture);
    delete player;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}