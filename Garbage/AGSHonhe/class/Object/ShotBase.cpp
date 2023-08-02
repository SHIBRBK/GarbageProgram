#include "../../Application.h"
#include "../Common/Quaternion.h"
#include "../Manager/SceneManager.h"
#include "../../Resource.h"
#include "../Common/Transform.h"
#include "ShotBase.h"

ShotBase::ShotBase(const Transform* parent)
{

	// �������n
	

	mIsAlive = false;

	// �����ϓ�����������ΐ�pGetter��
	mCollisionRadius = 10.0f;

	shotmodel = Resource::LoadModel(Resource::PATH_MODEL+"shell/shell.mv1");
	transform_.modelId = MV1DuplicateModel(shotmodel);
	float scale = 0.2f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = parent->pos;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler(Deg2RadF(90.0f), Deg2RadF(0.0f), 0.0f);
	transform_.Update();

	mStepAlive = 0.0f;
	mDir = { 0.0f, 0.0f, 0.0f };

}

ShotBase::~ShotBase(void)
{
}

void ShotBase::CreateShot(VECTOR birthPos, VECTOR dir)
{

	// �ė��p�\�Ȃ悤�ɂ���

	// �w������ɒe���΂�
	mDir = dir;

	transform_.pos = birthPos;
	transform_.quaRot = Quaternion::LookRotation(mDir);

	// �����n������
	mIsAlive = true;
	mStepAlive = GetTimeAlive();

}

void ShotBase::Update(void)
{


	// �����`�F�b�N����������
	if (!CheckAlive())
	{
		return;
	}

	// �ړ�����
	Move();

	// ���f������̊�{���X�V
	transform_.Update();

}

bool ShotBase::CheckAlive(void)
{

	// ��������
	mStepAlive -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepAlive < 0.0f)
	{
		mIsAlive = false;
	}

	return mIsAlive;

}

void ShotBase::Move(void)
{

	// �ړ�
	VECTOR velocity = VScale(mDir, GetSpeed());
	transform_.pos = VAdd(transform_.pos, velocity);

}

void ShotBase::Draw(void)
{

	

	if (!mIsAlive)
	{
		return;
	}

	if (!CheckCameraViewClip(transform_.pos))
	{
		MV1DrawModel(transform_.modelId);
	}

}

void ShotBase::Release(void)
{

}

float ShotBase::GetSpeed(void) const
{
	return DEFAULT_SPEED;
}

float ShotBase::GetTimeAlive(void) const
{
	return DEFAULT_TIME_ALIVE;
}

bool ShotBase::IsAlive(void) const
{
	return mIsAlive;
}



VECTOR ShotBase::GetPos(void) const
{
	return transform_.pos;
}

float ShotBase::GetCollisionRadius(void) const
{
	return mCollisionRadius;
}

float ShotBase::Deg2RadF(float deg)
{
	return deg * (DX_PI_F / 180.0f);
}