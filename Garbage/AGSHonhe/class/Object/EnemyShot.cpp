#include "EnemyShot.h"

EnemyShot::EnemyShot(const Transform* parent) : ShotBase(parent)
{
}

EnemyShot::~EnemyShot(void)
{
}

void EnemyShot::Update(void)
{
    // 生存チェック＆生存判定
    if (!IsAlive())
    {
        return;
    }



    // 移動処理
    Move();



    transform_.Update();
}

void EnemyShot::CreateShot(VECTOR birthPos, VECTOR dir)
{
    float SCALE = 50.0f;
    transform_.scl = { SCALE ,SCALE ,SCALE };
    transform_.Update();

    mCollisionRadius = 100.0f;


    ShotBase::CreateShot(birthPos, dir);
}

float EnemyShot::GetSpeed(void) const
{
    return 20.0f;
}
