#include "../../Application.h"
#include "../Common/Quaternion.h"
#include "../Manager/SceneManager.h"
#include "../../Resource.h"
#include "../Common/Transform.h"
#include "ShotBase.h"

ShotBase::ShotBase(const Transform* parent)
{

	// 初期化系
	

	mIsAlive = false;

	// もし変動させたければ専用Getterを
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

	// 再利用可能なようにする

	// 指定方向に弾を飛ばす
	mDir = dir;

	transform_.pos = birthPos;
	transform_.quaRot = Quaternion::LookRotation(mDir);

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

	// 移動処理
	Move();

	// モデル制御の基本情報更新
	transform_.Update();

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

void ShotBase::Move(void)
{

	// 移動
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