#include "TurretShot2.h"



TurretShot2::TurretShot2(const Transform* parent) :ShotBase(parent)
{
}



TurretShot2::~TurretShot2(void)
{
}



void TurretShot2::Update(void)
{
    // �����`�F�b�N����������
    if (!IsAlive())
    {
        return;
    }



    // �ړ�����
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