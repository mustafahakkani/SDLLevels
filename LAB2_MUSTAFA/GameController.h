#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "StandardIncludes.h"
#include <SDL.h>

// Use enum class to provide stronger type safety and scope
enum class GameState {
    Level1,
    Level2
};

// GameController class definition, inheriting from Singleton
class GameController : public Singleton<GameController> {
public:
    // Constructor and Destructor
    GameController();
    virtual ~GameController();

    // Public Methods
    void RunGame();

private:
    // Private Members
    SDL_Event m_sdlEvent{};
    GameState currentState{ GameState::Level1 };
    float gameTime{ 0.0f };
    bool autoSaved{ false };
};

#endif // GAME_CONTROLLER_H
