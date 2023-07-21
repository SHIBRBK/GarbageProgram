#include "ShotBase.h"
#include"../../Resource.h"
#include"../Manager/SceneManager.h"
ShotBase::ShotBase(const Transform* parent)
{
	mIsAlive = false;

	// もし変動させたければ専用Getterを
	mCollisionRadius = 10.0f;
	shotmodel=Resource::LoadModel(Resource::PATH_MODEL + "shell/shell.mv1");
	transform_.modelId = MV1DuplicateModel(shotmodel);
	float scale = 10.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = parent->pos;
	mDir = { 0.0f, 500.0f, 0.0f };


	transform_.Update();

	mStepAlive = 0.0f;
	speed_ = 10.0f;
}

ShotBase::~ShotBase(void)
{
}

void ShotBase::CreateShot(VECTOR pos, VECTOR dir)
{
	mDir = dir;

	transform_.pos = pos;
	transform_.quaRotLocal = Quaternion::LookRotation(mDir);

	// 生存系初期化
	mIsAlive = true;
	mStepAlive = GetTimeAlive();
}

void ShotBase::Update(void)
{
	// 生存チェック＆生存判定
	if (!CheckAlive())
	{
		return;
	}

	Move();
	transform_.Update();

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
		DrawFormatString(0, 300, 0xffffff, "%d", transform_.modelId);
	}
}

float ShotBase::GetSpeed(void) const
{
	return DEFAULT_SPEED;
}

float ShotBase::GetTimeAlive(void) const
{
	return DEFAULT_TIME_ALIVE;
}

bool ShotBase::CheckAlive(void)
{
	// 生存時間
	mStepAlive -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepAlive < 0.0f)
	{
		mIsAlive = false;
	}

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

bool ShotBase::IsAlive(void) const
{
	return mIsAlive;
}

void ShotBase::Move(void)
{
	VECTOR velocity = VScale(mDir, speed_);
	transform_.pos = VAdd(transform_.pos, velocity);
}

void ShotBase::Release(void)
{
}

float ShotBase::Deg2RadF(float deg)
{
	return deg * (DX_PI_F / 180.0f);
}
