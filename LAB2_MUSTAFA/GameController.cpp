#include "GameController.h"
#include "Renderer.h"
#include "SpriteSheet.h"
#include "SpriteAnim.h"
#include "TTFont.h"
#include "Timing.h"
#include "Level.h"
#include <memory>
#include <iostream>
#include <fstream>

// Constructor: Initializes the game state to Level1 and sets the default values.
GameController::GameController()
    : currentState(GameState::Level1), gameTime(0.0f), autoSaved(false), m_sdlEvent({})
{}

// Destructor: Ensures cleanup when the GameController is destroyed.
GameController::~GameController() {}

// Global instance of Timing for managing frame times and delta times.
Timing* t = &Timing::Instance();

// Utility function for generating random numbers within a range.
int RandomNumber(int min, int max) {
    return min + rand() % (max - min + 1); // Returns a number between min and max (inclusive).
}

// SaveLevel: Serializes the given level to a binary file.
void SaveLevel(Level* level, const std::string& filename) {
    std::ofstream writeStream(filename, std::ios::binary); // Open file in binary mode for writing.
    level->Serialize(writeStream); // Save the state of the level.
    writeStream.close(); // Close the file.
    level->SetAutoSaveStatus("AutoSave:Yes"); // Update autosave status.
}

// LoadLevel: Deserializes a level from a binary file and returns it.
std::unique_ptr<Level> LoadLevel(Renderer* r, TTFont* font, const std::string& filename) {
    auto level = std::make_unique<Level>(r, font); // Create a new Level object.
    std::ifstream readStream(filename, std::ios::binary); // Open file in binary mode for reading.
    level->Deserialize(readStream); // Load the level state from the file.
    readStream.close(); // Close the file.
    return level; // Return the loaded level as a unique_ptr.
}

// Main game loop and logic control.
void GameController::RunGame() {
    // Initialize assets with a predefined memory size.
    AssetController::Instance().Initialize(10000000);

    // Initialize the renderer with a 1920x1080 resolution.
    Renderer* r = &Renderer::Instance();
    r->Initialize(1920, 1080);

    // Initialize the font with a size of 20.
    std::unique_ptr<TTFont> font = std::make_unique<TTFont>();
    font->Initialize(20);

    // Retrieve the current window size.
    Point windowSize = r->GetWindowSize();

    // Create object pools for sprite sheets and sprite animations.
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    // Create two levels using the renderer and font.
    auto level1 = std::make_unique<Level>(r, font.get());
    auto level2 = std::make_unique<Level>(r, font.get());

    // Main game loop that continues until SDL_QUIT event is detected.
    while (m_sdlEvent.type != SDL_QUIT) {
        t->Tick(); // Update timing (frame rate and delta time).
        gameTime += t->GetDeltaTime(); // Accumulate game time.
        SDL_PollEvent(&m_sdlEvent); // Poll SDL events.

        // Handle different game states.
        switch (currentState) {
        case GameState::Level1:
            // Run Level 1 logic and check for transition conditions.
            level1->RunLevel1Logic(t->GetDeltaTime(), gameTime);

            // Save the level after 5 seconds if not already autosaved.
            if (!autoSaved && gameTime >= 5.0f) {
                SaveLevel(level1.get(), "level1.bin"); // Save level state.
                auto loadedLevel = LoadLevel(r, font.get(), "level1.bin"); // Load saved state.
                autoSaved = true; // Mark autosave as complete.
            }

            // Check if transition to Level 2 is triggered.
            if (level1->Level2TransitionTriggered()) {
                currentState = GameState::Level2; // Change game state to Level 2.
                level1->SetAutoSaveStatus("AutoSave:No"); // Reset autosave status.
                autoSaved = false; // Reset autosave flag.
                gameTime = 0.0f; // Reset game time.
                std::cout << "Transitioning to Level 2" << std::endl; // Log transition.
            }
            break;

        case GameState::Level2:
            // Run Level 2 logic and check for end conditions.
            level2->RunLevel2Logic(t->GetDeltaTime(), gameTime);

            // Save the level after 5 seconds if not already autosaved.
            if (!autoSaved && gameTime >= 5.0f) {
                SaveLevel(level2.get(), "level2.bin"); // Save level state.
                auto loadedLevel = LoadLevel(r, font.get(), "level2.bin"); // Load saved state.
                autoSaved = true; // Mark autosave as complete.
            }

            // Check if Level 2 has ended, triggering game shutdown.
            if (level2->Level2EndTriggered()) {
                SDL_Quit(); // Quit the game.
            }
            break;
        }

        // Render the current frame.
        SDL_RenderPresent(r->GetRenderer());
    }

    // Clean up resources when the game ends.
    delete SpriteAnim::Pool; // Delete the sprite animation pool.
    delete SpriteSheet::Pool; // Delete the sprite sheet pool.

    r->Shutdown(); // Shutdown the renderer.
}
