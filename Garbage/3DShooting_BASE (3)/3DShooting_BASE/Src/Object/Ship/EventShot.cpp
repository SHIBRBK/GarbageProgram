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

	//�ړ�����
	Move();

	//�J�����̎�����`�F�b�N
	VECTOR pos2D=ConvWorldPosToScreenPos(mTransform.pos);
	if (!(pos2D.z>0.0f && pos2D.z<1.0f))
	//if(CheckCameraViewClip(mTransform.pos))
	{


		//�J�����̎���O�ɓ��B������
		VECTOR revDir = VScale(mDir, -1.0f);
		VECTOR velovity = VScale(revDir, 100.0f);
		VECTOR newPos = VAdd(mTransform.pos, velovity);
		mTransform.pos = newPos;
		mTransform, Update();


		//�����G�t�F�N�g�̊J�n
		CreateExplosion(nullptr);

		//�J������h�炷
		SceneManager::GetInstance().GetCamera()->changeMode(
			Camera::MODE::SHAKE
		);

		
	}
	//���f������̊�{���X�V
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
