#pragma once
#include "ShotBase.h"
class TurretShot2 :
    public ShotBase
{
public:

	//�R���X�g
	TurretShot2(const Transform* parent);

	//�f�X�g���N�^
	~TurretShot2(void);

	//�X�V
	void Update(void) override;

	void Create(VECTOR birthPos, VECTOR dir) override;

	// �e�̑��x
	float GetSpeed(void) const override;

	// ��������
	//float GetTimeAlive(void) const override;
};

