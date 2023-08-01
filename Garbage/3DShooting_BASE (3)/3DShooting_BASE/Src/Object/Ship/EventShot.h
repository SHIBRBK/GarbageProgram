#pragma once
#include "ShotBase.h"

class Transform;

class EventShot:public ShotBase
{
public:
	EventShot(const Transform* parent);

	~EventShot(void);


	void Update(void) override;

	//�e�̑��x
	float GetSpeed(void) const override;

	//��������
	float GetTimeAlive(void)const const;
};

