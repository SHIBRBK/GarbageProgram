#pragma once
#include "ShotBase.h"
class TurretShot2 :
    public ShotBase
{
public:

	//コンスト
	TurretShot2(const Transform* parent);

	//デストラクタ
	~TurretShot2(void);

	//更新
	void Update(void) override;

	void Create(VECTOR birthPos, VECTOR dir) override;

	// 弾の速度
	float GetSpeed(void) const override;

	// 生存時間
	//float GetTimeAlive(void) const override;
};

