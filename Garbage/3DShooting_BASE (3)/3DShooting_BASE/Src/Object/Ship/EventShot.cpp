#pragma once
#include"../Common/SpriteAnimator.h"
#include"../../Manager/SceneManager.h"
#include"../../Manager/Camera.h"
#include "EventShot.h"

EventShot::EventShot(const Transform* parent):ShotBase(parent)
{

}

EventShot::~EventShot(void)
{

}

void EventShot::Update(void)
{
	mExplosion->Update();

	if(!IsAlive())
	{
		return;
	}

	//移動処理
	Move();

	//カメラの視野内チェック
	VECTOR pos2D=ConvWorldPosToScreenPos(mTransform.pos);
	if (!(pos2D.z>0.0f && pos2D.z<1.0f))
	//if(CheckCameraViewClip(mTransform.pos))
	{


		//カメラの視野外に到達したら
		VECTOR revDir = VScale(mDir, -1.0f);
		VECTOR velovity = VScale(revDir, 100.0f);
		VECTOR newPos = VAdd(mTransform.pos, velovity);
		mTransform.pos = newPos;
		mTransform, Update();


		//爆発エフェクトの開始
		CreateExplosion(nullptr);

		//カメラを揺らす
		SceneManager::GetInstance().GetCamera()->changeMode(
			Camera::MODE::SHAKE
		);

		
	}
	//モデル制御の基本情報更新
	mTransform, Update();
	
}

float EventShot::GetSpeed(void) const
{
	return 20.0f;
}

float EventShot::GetTimeAlive(void) const
{
	return 10.0f;
}
