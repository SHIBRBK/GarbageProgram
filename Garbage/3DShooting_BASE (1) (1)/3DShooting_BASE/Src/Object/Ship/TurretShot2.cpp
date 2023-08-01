#include "TurretShot2.h"



TurretShot2::TurretShot2(const Transform* parent) :ShotBase(parent)
{
}



TurretShot2::~TurretShot2(void)
{
}



void TurretShot2::Update(void)
{
    // 生存チェック＆生存判定
    if (!IsAlive())
    {
        return;
    }



    // 移動処理
    Move();



    mTransform.Update();
}

void TurretShot2::Create(VECTOR birthPos, VECTOR dir)
{
    float SCALE = 1.0f;
    mTransform.scl = { SCALE ,SCALE ,SCALE };
    mTransform.Update();

    mCollisionRadius = 100.0f;


    ShotBase::Create(birthPos, dir);
}



float TurretShot2::GetSpeed(void) const
{
    return 20.0f;
}