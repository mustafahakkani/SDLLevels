#ifndef LEVEL_H
#define LEVEL_H

#include "StandardIncludes.h"
#include "SpriteSheet.h"
#include "Renderer.h"
#include "Timing.h"
#include "TTFont.h"

class Level : public Resource
{
public:

    Level(Renderer* renderer, TTFont* font);
    Level(SpriteSheet* sheet, SpriteSheet* sheet1, Renderer* renderer, TTFont* font); ~Level();
    //save and load
    void Serialize(std::ostream& _stream);
    void Deserialize(std::istream& _stream);


    void RunLevel1Logic(float deltaTime, float gameTime);
    void RunLevel2Logic(float deltaTime, float gameTime);
    void SetAutoSaveStatus(const std::string& _status);
    bool Level2TransitionTriggered();
    bool Level2EndTriggered();

    bool autoSaved;


private:
    float rectX;
    float rectM;
    float scale;
    int spriteWidth;
    int spriteHeight;
    float scaleRock;
    int spriteWidthRock;
    int spriteHeightRock;
    int currentFrame;
    std::vector<int> randSpeeds;
    std::vector<bool> warriorIsAlive;
    std::vector<bool> rockIsAlive;
    std::string m_autoSaveStatus;
    std::vector<bool> warriorDeathState;
    std::vector<SpriteSheet*> warriorSheets;
    std::vector<SpriteSheet*> rockSheets;
    void DisplayHUD(float gameTime);
    std::vector<float> warriorXPositions;
    std::vector<float> rockYPositions;

    int viewportEdge;
    bool isGenerated = false;

    // Methods
    void GenerateRandomSpeeds();
    void InitializeWarriorPositions(vector<float> _warriorXPositions);
    void GenerateWarriorSheets();
    void GenerateRockSheets();
 
    bool IsColliding(const Rect& a, const Rect& b);

    float autoSaveMsgTimer;



    // Dependencies 
    SpriteSheet* sheet;
    SpriteSheet* sheetRock;
    Renderer* renderer;
    TTFont* font;
};

#endif
