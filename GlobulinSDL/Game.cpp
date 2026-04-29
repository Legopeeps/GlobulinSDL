// Game.cpp - Implementation of the Game class, managing the main game loop and 
// the core SDL interactions allowing my game engine to function.
#include "Game.h"

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr),
isSplashScreen(true), splashTimer(0.0f), gameTimer(0.0f),
score(0), combo(1), highestCombo(1),lives(3), loggingEnabled(true), musicStarted(false) { }

Game::~Game() {}

/* 
Game::init - 
Initializes SDL subsystems, creates the window and renderer, 
sets up audio and font systems, and loads necessary assets.

Has logging functionality (toggled by 'L' key) to provide 
feedback on the success or failure of each initialization step,
*/
bool Game::init(const char* title, int x, int y, int w, int h, bool fullscreen) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(title, x, y, w, h, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
        renderer = SDL_CreateRenderer(window, -1, 0);

        // Logical scaling to maintain aspect ratio across resolutions.
        SDL_RenderSetLogicalSize(renderer, 800, 600);

        // Initialize audio system for music and sound effects.
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cout << "[LOG] Mixer Initialization Error: " << Mix_GetError() << std::endl;
        }

		// Initialize TTF for text rendering (score display, timer, etc.)
        if (TTF_Init() == -1) {
            std::cout << "[LOG] TTF Init Error: " << TTF_GetError() << std::endl;
        }
		gameFont = TTF_OpenFont("assets/Font.ttf", 24);

        // Load splash screen asset and music track.
        SDL_Surface* tempSurface = SDL_LoadBMP("assets/HowToPlay.bmp");
        if (tempSurface) {
            splashTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
            SDL_FreeSurface(tempSurface);
        }
		// Load beat texture for the falling beats.
        SDL_Surface* beatSurface = SDL_LoadBMP("assets/enemyBeat.bmp");
        if (beatSurface == nullptr) {
            std::cout << "Failed to load beat image! SDL Error: " << SDL_GetError() << std::endl;
        }
        else {
            beatTexture = SDL_CreateTextureFromSurface(renderer, beatSurface);
            SDL_FreeSurface(beatSurface);
        }
		// Load player texture for the player's character.
        SDL_Surface* pSurf = SDL_LoadBMP("assets/player.bmp");
        playerTexture = SDL_CreateTextureFromSurface(renderer, pSurf);
        SDL_FreeSurface(pSurf);

		// Load sound effect for catching a beat and background music for the game session.
        catchSound = Mix_LoadWAV("assets/catch.wav");
        backgroundMusic = Mix_LoadMUS("assets/GameMusic.mp3");

        //logging the results of assets being loaded, if enabled
        if (loggingEnabled) {
            if (backgroundMusic && catchSound) {
                std::cout << "[LOG] Music loaded successfully." << std::endl;
            }
            else {
                std::cout << "[LOG] Music Load Error: " << Mix_GetError() << std::endl;
            }
			if (beatTexture) {
				std::cout << "[LOG] Beat texture loaded successfully." << std::endl;
			}
			else {
				std::cout << "[LOG] Beat Texture Load Error: " << SDL_GetError() << std::endl;
			}
            if (gameFont) {
                std::cout << "[LOG] Font loaded successfully." << std::endl;
            }
            else {
                std::cout << "[LOG] Font Load Error: " << TTF_GetError() << std::endl;
            }
            if (splashTexture) {
                std::cout << "[LOG] Splash texture loaded successfully." << std::endl;
            }
            else {
                std::cout << "[LOG] Splash Texture Load Error: " << SDL_GetError() << std::endl;
            }
            if (window && renderer) {
                std::cout << "[LOG] Window and Renderer created successfully." << std::endl;
            }
            else {
                std::cout << "[LOG] Window/Renderer Creation Error: " << SDL_GetError() << std::endl;
            }
        }
        // Spawn player at bottom center
        player = new Player(400, 500, 10);

		// Load life icon texture for displaying remaining lives.
        SDL_Surface* lifeSurface = SDL_LoadBMP("assets/Life.bmp");
        if (lifeSurface) {
            lifeTexture = SDL_CreateTextureFromSurface(renderer, lifeSurface);
            SDL_FreeSurface(lifeSurface);
        }
		isRunning = true; // game loop flag after successful initialisation
    }
    return isRunning;
}

/*
Game::handleEvents -
Handles player input and system events, including quitting, 
toggling fullscreen mode with 'F', and toggle logging with 'L'.
*/
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
				std::cout << "[LOG] Fullscreen " << (isFullscreen ? "Enabled" : "Disabled") << std::endl;
                break;

            case SDLK_l: // Toggle diagnostic logging.
                loggingEnabled = !loggingEnabled;
				std::cout << "[LOG] Logging " << (loggingEnabled ? "Enabled" : "Disabled") << std::endl;
                break;
            }
        }
        if (event.type == SDL_KEYDOWN) { //Quit with escape key
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false; 
            }
        }
    }
}

/*
Game::update -
Manages the core game logic per frame:
splash screen timing, music control, player movement, 
beat spawning, and collision detection.
*/
void Game::update() {
    // 5-second splash screen with controls/how to play guide.
    if (isSplashScreen) {
        splashTimer += 0.016f;
        if (splashTimer >= 5.0f)
            isSplashScreen = false;
        return;
    }
    if (isGameOver) return;

    // Start background music once splashscreen is gone.
    if (!musicStarted) {
        Mix_PlayMusic(backgroundMusic, -1);
        musicStarted = true;
    }

    // 60-second game-time period, after which music stops and game ends.
    gameTimer += 0.016f;
    if (gameTimer >= 60.0f) {
        Mix_HaltMusic();
        isGameOver = true;
    }

    //if player reaches score of 70,000, give 15 seconds of extra time ONE TIME
	if (score >= 70000 && bonusAwarded == false) {
		gameTimer -= 15; // Subtracting from the timer effectively gives the player more time to play, as the game ends when gameTimer reaches 60 seconds.
        bonusAwarded = true;
		if (loggingEnabled)
			std::cout << "[LOG] 70,000 Points Reached. Bonus 15 Seconds Awarded." << std::endl;
	}

    // Process player movement and boundary constraints.
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    player->handleInput(keystate);
    player->update(SCREEN_WIDTH, SCREEN_HEIGHT, BORDER_THICKNESS);

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

            // Collision handling: Increments score based on current combo multiplier.
            if (SDL_HasIntersection(&pRect, &b.rect)) {
                if(loggingEnabled)
				    std::cout << "[LOG] Collision Detected at (" << b.rect.x << ", " << b.rect.y << ")" << std::endl;
				Mix_PlayChannel(-1, catchSound, 0); // Play catch sound effect on successful catch
                b.active = false;
                score += (100 * combo); // Score scaling
                combo++;                // Increment multiplier for consecutive catch

                if (combo > highestCombo) 
                {
                    highestCombo = combo;
                }

                if (loggingEnabled)
                    std::cout << "[LOG] Virus Caught. Combo: x" << combo << " || Score: " << score << std::endl;
            }

            // Out-of-bounds handling: Resets combo multiplier if a virus is missed.
            if (b.rect.y > 600) {
                lives--;
				combo = 1; // Reset combo on miss
                b.active = false;
                
                if (loggingEnabled) 
                    std::cout << "[LOG] Virus Missed, x" << lives << " Lives Remaining || Combo Reset, Highest Combo : x" << highestCombo << std::endl;
				
                if (lives <= 0) {
					Mix_HaltMusic();
					isGameOver = true;
				}
            }
        }
    }
}

/*
Game::render -
Handles all drawing operations for the current frame, including:
- Clearing the screen with a background color.
- Rendering the splash screen during the initial state.
- Rendering the player and active beats during gameplay.
- Displaying the current score, combo and lives in the top-left corner.
*/
void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255);
    SDL_RenderClear(renderer);
    SDL_Color white = { 255, 255, 255, 255 };

    if (isSplashScreen) {
        SDL_RenderCopy(renderer, splashTexture, NULL, NULL);
    }
    else if (isGameOver) {
        
        SDL_Color gold = { 255, 215, 0, 255 };
        
        // Header
        std::string title = "Game Over!";

        // Final Stats
        std::string summary = "Final Score: " + std::to_string(score) +
            " | Max Combo: x" + std::to_string(highestCombo);

        //bottom prompt
        std::string quitMsg = "Press ESC to Exit";

		// Render the text surfaces and convert them to textures for display.
        SDL_Surface* surfTitle = TTF_RenderText_Solid(gameFont, title.c_str(), gold);
        SDL_Surface* surfStats = TTF_RenderText_Solid(gameFont, summary.c_str(), white);

		// Check if surfaces were created successfully before rendering.
        if (surfTitle && surfStats) {
            SDL_Texture* texTitle = SDL_CreateTextureFromSurface(renderer, surfTitle);
            SDL_Texture* texStats = SDL_CreateTextureFromSurface(renderer, surfStats);

            // Center the text on the 800x600 screen
            SDL_Rect rectTitle = { 400 - (surfTitle->w / 2), 200, surfTitle->w, surfTitle->h };
            SDL_Rect rectStats = { 400 - (surfStats->w / 2), 300, surfStats->w, surfStats->h };

            SDL_RenderCopy(renderer, texTitle, NULL, &rectTitle);
            SDL_RenderCopy(renderer, texStats, NULL, &rectStats);

            SDL_FreeSurface(surfTitle);
            SDL_FreeSurface(surfStats);
            SDL_DestroyTexture(texTitle);
            SDL_DestroyTexture(texStats);
        }
		// Render the quit prompt at the bottom of the screen.
        SDL_Surface* surfQuit = TTF_RenderText_Solid(gameFont, quitMsg.c_str(), white);
        if (surfQuit) {
            SDL_Texture* texQuit = SDL_CreateTextureFromSurface(renderer, surfQuit);
            SDL_Rect rectQuit = { 400 - (surfQuit->w / 2), 400, surfQuit->w, surfQuit->h };
            SDL_RenderCopy(renderer, texQuit, NULL, &rectQuit);

            SDL_FreeSurface(surfQuit);
            SDL_DestroyTexture(texQuit);
        }
    }
    if (!isSplashScreen && !isGameOver) {
        // Render gameplay entities and falling viruses.
        player->render(renderer, playerTexture);

        // Draw Lives directly below the player
        for (int i = 0; i < lives; i++) {
            // Positioned 55 pixels below the player's top
            SDL_Rect lifeRect = {
                player->getX() + (i * 18), // Spacing between sprites
                player->getY() + 55,       // below the 50px tall player
                15, 15                     // Sprite size
            };
            SDL_RenderCopy(renderer, lifeTexture, NULL, &lifeRect);
        }
        for (auto& b : activeBeats) {
            if (b.active) 
                SDL_RenderCopy(renderer, beatTexture, NULL, &b.rect);
        }
		// Prepare the score and timer strings for display.
        std::string scoreStr = "Score: " + std::to_string(score) + " | Combo: x" + std::to_string(combo) + " | Highest Combo: x" + std::to_string(highestCombo);
        int timeLeft = 60 - (int)gameTimer;
        if (timeLeft < 0) timeLeft = 0;

        std::string timerStr = "Time: " + std::to_string(timeLeft) + "s";

        SDL_Color yellow = { 255, 255, 0, 255 }; 
        SDL_Surface* timerSurface = TTF_RenderText_Solid(gameFont, timerStr.c_str(), yellow);

        if (timerSurface) {
            SDL_Texture* timerTexture = SDL_CreateTextureFromSurface(renderer, timerSurface);

            // Positioned  in the top right (20px margin)
            SDL_Rect timerRect = { 800 - timerSurface->w - 20, 20, timerSurface->w, timerSurface->h };

            SDL_RenderCopy(renderer, timerTexture, NULL, &timerRect);

			// Clean up the timer surface and texture after rendering.
            SDL_FreeSurface(timerSurface);
            SDL_DestroyTexture(timerTexture);
        }

        SDL_Surface* scoreSurface = TTF_RenderText_Solid(gameFont, scoreStr.c_str(), white);
        if (scoreSurface) {
            SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
            SDL_Rect scoreRect = { 20, 20, scoreSurface->w, scoreSurface->h };
            SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
            SDL_FreeSurface(scoreSurface);
            SDL_DestroyTexture(scoreTexture);
        }
    }

    SDL_RenderPresent(renderer);
}

/*
Game::clean -
Handles the cleanup/deallocation of all game resources, including:
- Music and audio resources.
- Fonts and text rendering resources.
- Textures and graphical assets.
- Player object and other dynamically allocated entities.
- SDL window and renderer.
*/
void Game::clean() {
    Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();
    
    if (gameFont) TTF_CloseFont(gameFont);
    TTF_Quit();
    
    SDL_DestroyTexture(splashTexture);
    
    delete player;
    
	SDL_DestroyTexture(playerTexture);
	SDL_DestroyTexture(lifeTexture);
	SDL_DestroyTexture(beatTexture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}