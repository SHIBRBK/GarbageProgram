#include "../Common/SpriteAnimator.h"
#include"../../Manager/SceneManager.h"
#include"../../Manager/Camera.h"
#include "EventShot.h"


EventShot::EventShot(const Transform* parent)
	:ShotBase(parent)
{
}

EventShot::~EventShot(void)
{
}

void EventShot::Update(void)
{
	// 着弾時のエフェクト
	mExplosion->Update();

	// 生存チェック＆生存判定
	if (!IsAlive())
	{
		return;
	}

	// 移動処理
	Move();

	//カメラ視野内チェック
	VECTOR pos2D = ConvWorldPosToScreenPos(mTransform.pos);
	if (!(pos2D.z > 0.0f && pos2D.z < 1.0f))
	//if(CheckCameraViewClip(mTransform.pos))
	{

		VECTOR revDir = VScale(mDir, -1.0f);
		VECTOR velocity = VScale(revDir, 100.0f);
		VECTOR newPos = VAdd(mTransform.pos, velocity);
		mTransform.pos = newPos;
		mTransform.Update();

		//カメラの視野外に到達したら
		//爆発エフェクトの開始
		CreateExplosion(nullptr);
		//カメラを揺らす
		SceneManager::GetInstance().GetCamera()->ChangeMode(
		Camera::MODE::SHAKE
		);
	}

	// モデル制御の基本情報更新
	mTransform.Update();
}

float EventShot::GetSpeed(void) const
{
	return 20.0f;
}

float EventShot::GetTimeAlive(void) const
{
	return 10.0f;
}
