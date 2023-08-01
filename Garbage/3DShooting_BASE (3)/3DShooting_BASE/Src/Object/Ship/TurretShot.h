#pragma once
#include "ShotBase.h"

class Transform;

class TurretShot:ShotBase
{
public:
	TurretShot(const Transform* parent);
	~TurretShot();

	void Draw(void) override;
	void Update(void) override;
	void Release(void) override;

	void Create(VECTOR birthPos, VECTOR dir) override;
	//íeÇÃë¨ìx
	float GetSpeed(void) const override;

	//ê∂ë∂éûä‘
	float GetTimeAlive(void)const const;


};

