#pragma once
#include"ShotBase.h"
class Transform;

class PlayerShot: public ShotBase
{
public:
	PlayerShot(const Transform* parent);
	~PlayerShot();

	
	float mStepDeleyShot;
};

