#include "Level.h"
#include <random>
#include <stdbool.h>

// Constructor: Initializes the level with default values and prepares sprites and game elements.
Level::Level(Renderer* renderer, TTFont* font)
    : rectX(0.0f), rectM(0.0f), scale(1.8f),
    spriteWidth(69), spriteHeight(44), currentFrame(0),
    renderer(renderer), font(font), autoSaved(false),
    m_autoSaveStatus("AutoSave:No"), autoSaveMsgTimer(0.0f),
    warriorXPositions(10, 0.0f), rockYPositions(10, 0.0f),
    warriorIsAlive(10, true), rockIsAlive(10, true),
    warriorDeathState(10, false), scaleRock(1.0f),
    spriteWidthRock(20), spriteHeightRock(20), viewportEdge(1920)
{
    if (!isGenerated)
        GenerateRandomSpeeds(); // Generate random speeds for warriors and rocks.

    GenerateWarriorSheets(); // Load and prepare warrior sprites.
    GenerateRockSheets();    // Load and prepare rock sprites.
}

// Destructor: Cleans up when the level is destroyed.
Level::~Level() {}

// Serialize: Saves the current state of the level to a stream (used for saving).
void Level::Serialize(std::ostream& _stream)
{
    _stream.write(reinterpret_cast<char*>(&rectX), sizeof(rectX));
    _stream.write(reinterpret_cast<char*>(&rectM), sizeof(rectM));
    _stream.write(reinterpret_cast<char*>(&currentFrame), sizeof(currentFrame));
    _stream.write(reinterpret_cast<char*>(&scale), sizeof(scale));
    _stream.write(reinterpret_cast<char*>(&spriteWidth), sizeof(spriteWidth));
    _stream.write(reinterpret_cast<char*>(&spriteHeight), sizeof(spriteHeight));
    std::cout << "Level Saved Successfully" << std::endl;
    Resource::Serialize(_stream); // Call base class serialization.
}

// SetAutoSaveStatus: Updates the auto-save status message.
void Level::SetAutoSaveStatus(const std::string& _status)
{
    m_autoSaveStatus = _status;
}

// InitializeWarriorPositions: Sets the initial positions of all warriors to 0.
void Level::InitializeWarriorPositions(std::vector<float> _warriorXPositions)
{
    _warriorXPositions.assign(10, 0.0f);
}

// Deserialize: Loads the state of the level from a stream (used for loading).
void Level::Deserialize(std::istream& _stream)
{
    _stream.read(reinterpret_cast<char*>(&rectX), sizeof(rectX));
    _stream.read(reinterpret_cast<char*>(&rectM), sizeof(rectM));
    _stream.read(reinterpret_cast<char*>(&currentFrame), sizeof(currentFrame));
    _stream.read(reinterpret_cast<char*>(&scale), sizeof(scale));
    _stream.read(reinterpret_cast<char*>(&spriteWidth), sizeof(spriteWidth));
    _stream.read(reinterpret_cast<char*>(&spriteHeight), sizeof(spriteHeight));
    Resource::Deserialize(_stream); // Call base class deserialization.
    std::cout << "Level Loaded Successfully" << std::endl;
}

// GenerateRandomSpeeds: Generates random speeds for warriors and rocks.
void Level::GenerateRandomSpeeds() {
    std::random_device rd;
    std::uniform_int_distribution<int> dist(80, 100);

    randSpeeds.resize(10);
    for (int& speed : randSpeeds) {
        speed = dist(rd);
        std::cout << "Generated Speed: " << speed << std::endl;
    }

    isGenerated = true; // Mark speeds as generated.
}

// GenerateWarriorSheets: Loads warrior sprite sheets and animations.
void Level::GenerateWarriorSheets()
{
    for (int i = 0; i < 10; i++) {
        SpriteSheet* sheet = SpriteSheet::Pool->GetResource();
        sheet->Load("../Assets/Textures/Warrior.tga");
        sheet->SetSize(17, 6, 69, 44); // Define sprite dimensions.
        sheet->AddAnimation(EN_AN_RUN, 6, 7, randSpeeds[i] / 100.0f * 6.0f);
        sheet->AddAnimation(EN_AN_DEATH, 27, 10, randSpeeds[i] / 100.0f * 6.0f);

        warriorSheets.push_back(sheet); // Store the sprite sheet.
    }
}

// GenerateRockSheets: Loads rock sprite sheets and animations.
void Level::GenerateRockSheets()
{
    for (int i = 0; i < 10; i++) {
        SpriteSheet* sheet = SpriteSheet::Pool->GetResource();
        sheet->Load("../Assets/Textures/Rock.tga");
        sheet->SetSize(1, 4, 20, 20); // Define sprite dimensions.
        sheet->AddAnimation(ROCK_FALL, 0, 4, randSpeeds[i] / 100.0f * 6.0f);

        rockSheets.push_back(sheet); // Store the sprite sheet.
    }
}

// CheckCollision: Checks if two objects are colliding.
bool CheckCollision(float x1A, float y1A, float x2A, float y2A,
    float x1B, float y1B, float x2B, float y2B)
{
    return !(x1A > x2B || x2A < x1B || y1A > y2B || y2A < y1B);
}

// RunLevel1Logic: Handles the logic and rendering for Level 1.
void Level::RunLevel1Logic(float deltaTime, float gameTime)
{
    int offsets[] = { 10, 110, 210, 310, 410, 510, 610, 710, 810, 910 };

    renderer->SetDrawColor(Color(128, 128, 128, 255));
    renderer->ClearScreen();

    // Update and render warriors.
    for (int i = 0; i < 10; i++) {
        warriorXPositions[i] += randSpeeds[i] * deltaTime;
        renderer->RenderTexture(warriorSheets[i],
            warriorSheets[i]->Update(EN_AN_RUN, deltaTime),
            Rect(warriorXPositions[i], offsets[i],
                warriorXPositions[i] + spriteWidth * scale,
                offsets[i] + spriteHeight * scale));
    }

    // Display FPS and game time.
    font->Write(renderer->GetRenderer(),
        ("FPS: " + std::to_string(Timing::Instance().GetFPS())).c_str(),
        SDL_Color{ 0, 0, 255 }, SDL_Point{ 100, 0 });

    font->Write(renderer->GetRenderer(),
        ("Game Time: " + std::to_string(static_cast<int>(gameTime))).c_str(),
        SDL_Color{ 0, 0, 255 }, SDL_Point{ 250, 0 });

    font->Write(renderer->GetRenderer(),
        m_autoSaveStatus.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 500, 0 });
}

// Level2TransitionTriggered: 
// Checks if any warrior has reached the edge of the viewport to trigger the transition to Level 2.
bool Level::Level2TransitionTriggered()
{
    // Iterate through all warriors to check their positions.
    for (int i = 0; i < 10; i++)
    {
        // If a warrior's X position has reached or exceeded the viewport edge.
        if (warriorXPositions[i] >= viewportEdge - spriteWidth)
        {
            std::cout << "Warrior " << i << " was first!" << std::endl;
            // Reset all warrior positions to the starting point (0.0f).
            warriorXPositions = std::vector<float>(10, 0.0f);
            return true;  // Transition to Level 2 is triggered.
        }
    }
    return false;  // No transition if no warrior reaches the edge.
}

// RunLevel2Logic: 
// Manages the logic, rendering, and updates for Level 2.
void Level::RunLevel2Logic(float deltaTime, float gameTime)
{
    // Define vertical offsets for rendering warriors and rocks.
    int offsets[] = { 10, 110, 210, 310, 410, 510, 610, 710, 810, 910 };
    int offsetsRock[] = { 10, 110, 210, 310, 410, 510, 610, 710, 810, 910 };

    // Set the background color to green and clear the screen.
    renderer->SetDrawColor(Color(0, 128, 0, 255));
    renderer->ClearScreen();

    // Update and render warriors.
    for (int i = 0; i < 10; i++)
    {
        if (warriorIsAlive[i])  // Check if the warrior is alive.
        {
            if (!warriorDeathState[i])  // If the warrior is not in death animation.
            {
                // Move the warrior based on its speed and deltaTime.
                warriorXPositions[i] += randSpeeds[i] * deltaTime;

                // Render the running animation of the warrior.
                renderer->RenderTexture(
                    warriorSheets[i],
                    warriorSheets[i]->Update(EN_AN_RUN, deltaTime),
                    Rect(warriorXPositions[i], offsets[i],
                        warriorXPositions[i] + spriteWidth * scale,
                        offsets[i] + spriteHeight * scale)
                );
            }
            else  // If the warrior is in death animation.
            {
                // Check if the death animation has completed.
                if (warriorSheets[i]->GetCurrentClip(EN_AN_DEATH) == 36)
                {
                    warriorIsAlive[i] = false;  // Mark the warrior as dead.
                }
                else
                {
                    // Render the death animation of the warrior.
                    renderer->RenderTexture(
                        warriorSheets[i],
                        warriorSheets[i]->Update(EN_AN_DEATH, deltaTime),
                        Rect(warriorXPositions[i], offsets[i],
                            warriorXPositions[i] + spriteWidth * scale,
                            offsets[i] + spriteHeight * scale)
                    );
                }
            }
        }
    }

    // Update and render falling rocks.
    for (int i = 0; i < 10; i++)
    {
        if (rockIsAlive[i])  // Check if the rock is still active.
        {
            // Move the rock downwards based on its speed and deltaTime.
            rockYPositions[i] += randSpeeds[i] * deltaTime;

            // Render the falling animation of the rock.
            renderer->RenderTexture(
                rockSheets[i],
                rockSheets[i]->Update(ROCK_FALL, deltaTime),
                Rect(offsetsRock[i], rockYPositions[i],
                    offsetsRock[i] + spriteHeightRock,
                    rockYPositions[i] + spriteWidthRock * scaleRock)
            );
        }
    }

    // Display the current FPS on the screen.
    std::string fps = "FPS: " + std::to_string(Timing::Instance().GetFPS());
    font->Write(renderer->GetRenderer(), fps.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 100, 0 });

    // Display the game time on the screen.
    std::string time = "Game Time: " + std::to_string(static_cast<int>(gameTime));
    font->Write(renderer->GetRenderer(), time.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 250, 0 });

    // Display the auto-save status on the screen.
    font->Write(renderer->GetRenderer(), m_autoSaveStatus.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 500, 0 });

    // Check for collisions between warriors and rocks.
    for (int i = 0; i < 10; i++)
    {
        if (warriorIsAlive[i])  // Check if the warrior is alive.
        {
            for (int j = 0; j < 10; j++)
            {
                if (rockIsAlive[j])  // Check if the rock is still active.
                {
                    // Check for collision between the warrior and the rock.
                    if (CheckCollision(
                        warriorXPositions[i], offsets[i],
                        warriorXPositions[i] + spriteWidth, offsets[i] + spriteHeight,
                        offsetsRock[j], rockYPositions[j],
                        offsetsRock[j] + spriteWidthRock, rockYPositions[j] + spriteHeightRock))
                    {
                        // Mark the rock as inactive upon collision.
                        rockIsAlive[j] = false;

                        // Trigger the death animation for the warrior.
                        warriorDeathState[i] = true;
                    }
                }
            }
        }
    }
}

// Level2EndTriggered: 
// Checks if all warriors are either dead or have reached the end of the viewport.
bool Level::Level2EndTriggered()
{
    // Iterate through all warriors to check their status.
    for (int i = 0; i < 10; i++)
    {
        // Check if any warrior has reached the edge of the viewport or if all warriors are dead.
        if (warriorXPositions[i] >= viewportEdge - spriteWidth ||
            std::find(warriorIsAlive.begin(), warriorIsAlive.end(), true) == warriorIsAlive.end())
        {
            return true;  // Level 2 ends.
        }
    }
    return false;  // Level 2 continues if no end condition is met.
}
