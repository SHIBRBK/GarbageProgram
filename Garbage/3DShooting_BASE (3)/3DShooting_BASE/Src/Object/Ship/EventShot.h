#pragma once
#include "ShotBase.h"

class Transform;

class EventShot:public ShotBase
{
public:
	EventShot(const Transform* parent);

	~EventShot(void);


	void Update(void) override;

	//’e‚Ì‘¬“x
	float GetSpeed(void) const override;

	//¶‘¶ŠÔ
	float GetTimeAlive(void)const const;
};

