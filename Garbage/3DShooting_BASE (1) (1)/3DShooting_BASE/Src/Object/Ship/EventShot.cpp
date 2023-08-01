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
	// ���e���̃G�t�F�N�g
	mExplosion->Update();

	// �����`�F�b�N����������
	if (!IsAlive())
	{
		return;
	}

	// �ړ�����
	Move();

	//�J����������`�F�b�N
	VECTOR pos2D = ConvWorldPosToScreenPos(mTransform.pos);
	if (!(pos2D.z > 0.0f && pos2D.z < 1.0f))
	//if(CheckCameraViewClip(mTransform.pos))
	{

		VECTOR revDir = VScale(mDir, -1.0f);
		VECTOR velocity = VScale(revDir, 100.0f);
		VECTOR newPos = VAdd(mTransform.pos, velocity);
		mTransform.pos = newPos;
		mTransform.Update();

		//�J�����̎���O�ɓ��B������
		//�����G�t�F�N�g�̊J�n
		CreateExplosion(nullptr);
		//�J������h�炷
		SceneManager::GetInstance().GetCamera()->ChangeMode(
		Camera::MODE::SHAKE
		);
	}

	// ���f������̊�{���X�V
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
