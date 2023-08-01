#include "TurretShot.h"

TurretShot::TurretShot(const Transform* parent):ShotBase(parent)
{

}

TurretShot::~TurretShot()
{
}

void TurretShot::Draw(void)
{
}

void TurretShot::Update(void)
{
}

void TurretShot::Release(void)
{
}

void TurretShot::Create(VECTOR birthPos, VECTOR dir)
{
	// �ė��p�\�Ȃ悤�ɂ���

// �w������ɒe���΂�
	mDir = dir;

	mTransform.pos = birthPos;
	mTransform.quaRot = Quaternion::LookRotation(mDir);

	// �����n������
	mIsAlive = true;
	mStepAlive = GetTimeAlive();

	ShotBase::Create(birthPos, dir);

}

float TurretShot::GetSpeed(void) const
{
	return 20.0f;
}

float TurretShot::GetTimeAlive(void) const
{
	return 10.0f;
}
