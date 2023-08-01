#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Common/Quaternion.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../Common/Transform.h"
#include "../Common/SpriteAnimator.h"
#include "ShotBase.h"

ShotBase::ShotBase(const Transform* parent)
{

	// 初期化系
	mExplosion = new SpriteAnimator(ResourceManager::SRC::SHOT_EXPLOSION, 20.0f, 8.0f);

	mIsAlive = false;

	// もし変動させたければ専用Getterを
	mCollisionRadius = 10.0f;

	// モデル制御の基本情報
	ResourceManager& rem = ResourceManager::GetInstance();
	mTransform.modelId = rem.LoadModelDuplicate(ResourceManager::SRC::SHOT_MODEL);
	float scale = 0.2f;
	mTransform.scl = { scale, scale, scale };
	mTransform.pos = parent->pos;
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal = Quaternion::Euler(AsoUtility::Deg2RadF(90.0f), 0.0f, 0.0f);
	mTransform.Update();

	mStepAlive = 0.0f;
	mDir = { 0.0f, 0.0f, 0.0f };

}

ShotBase::~ShotBase(void)
{
}

void ShotBase::Create(VECTOR birthPos, VECTOR dir)
{

	// 再利用可能なようにする

	// 指定方向に弾を飛ばす
	mDir = dir;

	mTransform.pos = birthPos;
	mTransform.quaRot = Quaternion::LookRotation(mDir);

	// 生存系初期化
	mIsAlive = true;
	mStepAlive = GetTimeAlive();

}

void ShotBase::Update(void)
{

	// 着弾時のエフェクト
	mExplosion->Update();

	// 生存チェック＆生存判定
	if (!CheckAlive())
	{
		return;
	}

	// 移動処理
	Move();

	// モデル制御の基本情報更新
	mTransform.Update();

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
	mTransform.pos = VAdd(mTransform.pos, velocity);

}

void ShotBase::Draw(void)
{

	mExplosion->Draw();

	if (!mIsAlive)
	{
		return;
	}

	if (!CheckCameraViewClip(mTransform.pos))
	{
		MV1DrawModel(mTransform.modelId);
	}

}

void ShotBase::Release(void)
{
	mExplosion->Release();
	delete mExplosion;
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

void ShotBase::CreateExplosion(const Transform* transform)
{
	mIsAlive = false;
	// 衝突前の位置にして、できるだけ表示が重ならないように
	VECTOR pos = VAdd(mTransform.pos, VScale(mTransform.GetBack(), 5.0f));
	mExplosion->Create(pos, transform);
}

VECTOR ShotBase::GetPos(void) const
{
	return mTransform.pos;
}

float ShotBase::GetCollisionRadius(void) const
{
	return mCollisionRadius;
}
