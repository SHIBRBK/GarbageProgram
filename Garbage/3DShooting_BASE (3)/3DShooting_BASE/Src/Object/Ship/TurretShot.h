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
	//�e�̑��x
	float GetSpeed(void) const override;

	//��������
	float GetTimeAlive(void)const const;


};

