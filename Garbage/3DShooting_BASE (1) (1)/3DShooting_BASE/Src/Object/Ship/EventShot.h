#pragma once
#include "ShotBase.h"

class EventShot : public ShotBase
{
public:

	//�R���X�g
	EventShot(const Transform* parent);

	//�f�X�g���N�^
	~EventShot(void);

	//�X�V
	void Update(void) override;


	// �e�̑��x
	float GetSpeed(void) const override;

	// ��������
	float GetTimeAlive(void) const override;

};

