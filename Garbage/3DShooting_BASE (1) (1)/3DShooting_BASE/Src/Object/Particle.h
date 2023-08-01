#pragma once
#include <DxLib.h>

class Particle
{
public:
    Particle(void);
    ~Particle(void);

    void Generate(int image, VECTOR pos, float size, VECTOR dir, float speed, float lifetime);
    void Update(void);
    void Draw(void);
    void Release(void);

    //¶‘¶”»’è
    bool IsAlive(void) const;

    VECTOR GetPos(void) const;
    void SetPos(const VECTOR& pos);

    float GetZLen(void)const;
    void SetZLen(const float& zlen);
private:

    //‰æ‘œ
    int mImg;

    //À•W
    VECTOR mPos;

    //‘å‚«‚³
    float mSize;

    //ˆÚ“®•ûŒü
    VECTOR mDir;

    //ˆÚ“®‘¬“x
    float mSpeed;

    //¶‘¶ŠÔ
    float mLifeTime;

    //Z‹——£
    float mZLen;
};