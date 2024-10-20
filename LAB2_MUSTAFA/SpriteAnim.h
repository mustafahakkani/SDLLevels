#ifndef SPRITEANIM_H
#define SPRITEANIM_H

#include "Resource.h"

class SpriteAnim : Resource {
public:
    // Constructors/Destructors
    SpriteAnim();
    virtual ~SpriteAnim();

    // Accessors
    short GetClipCurrent() { return (short)m_clipCurrent; }

    // Methods
    virtual void Serialize(std::ostream& _stream);
    virtual void Deserialize(std::istream& _stream);
    virtual void ToString();

    void ClearMemory();
    void Create(short _clipStart, short _clipCount, float _clipSpeed);
    void Update(float _deltaTime);

    // Members
    static ObjectPool<SpriteAnim>* Pool;
    enum AnimationNames {
        EN_AN_IDLE = 0,
        EN_AN_RUN,
        EN_AN_TWO_COMBO_ATTACK,
        EN_AN_DEATH,
        EN_AN_HURT,
        EN_AN_JUMP_UP_FALL,
        EN_AN_EDGE_GRAB,
        EN_AN_EDGE_IDLE,
        EN_AN_WALL_SIDE,
        EN_AN_CROUCH,
        EN_AN_DASH,
        EN_AN_DASH_ATTACH,
        EN_AN_SLIDE,
        EN_AN_LADDDER_GRAB,
        ROCK_FALL 
    };

private:
    // Members
    short m_clipStart;
    short m_clipCount;
    float m_clipCurrent;
    float m_clipSpeed;
    short m_clipEnd; 
};

#endif // SPRITEANIM_H
