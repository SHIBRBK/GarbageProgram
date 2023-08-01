#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "Particle.h"

Particle::Particle(void)
{
}

Particle::~Particle(void)
{
}

void Particle::Generate(int image, VECTOR pos, float size, VECTOR dir, float speed, float lifetime)
{
    mImg = image;
    //出現場所
    mPos = pos;

    mSize = size;
    mDir = dir;
    mSpeed = speed;
    mLifeTime = lifetime;
}

void Particle::Update(void)
{
    float deltaTime = SceneManager::GetInstance().GetDeltaTime();
    if (mLifeTime > 0.0f)
    {
        mLifeTime -= deltaTime;
    }
    else
    {
        return;
    }

    //移動処理(移動方向×スピード×デルタタイム)
    mPos = VAdd(mPos, VScale(mDir, mSpeed * deltaTime));
}

void Particle::Draw(void)
{
    if (!IsAlive())
    {
        return;
    }
    DrawBillboard3D(mPos, 0.5f, 0.5f, mSize, 0.0f, mImg, true);
}

void Particle::Release(void)
{
}

bool Particle::IsAlive(void) const
{
    return mLifeTime > 0.0f;
}

VECTOR Particle::GetPos(void) const
{
    return mPos;
}

void Particle::SetPos(const VECTOR& pos)
{
    mPos = pos;
}

float Particle::GetZLen(void) const
{
    return mZLen;
}

void Particle::SetZLen(const float& zlen)
{
    mZLen = zlen;
}