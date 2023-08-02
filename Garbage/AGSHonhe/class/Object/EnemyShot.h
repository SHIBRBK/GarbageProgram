#pragma once
#include <memory>
#include "ShotBase.h"
#include"BattleShip.h"
#include"../Common/Transform.h"
class EnemyShot :
	public ShotBase
{
public:
	static constexpr float SCALE = 70.0f;
	static constexpr float COL_RADIUS = 10.0f;

	//コンスト
	EnemyShot(const Transform* parent);
	~EnemyShot(void);
	//更新
	void Update(void) override;

	void CreateShot(VECTOR birthPos, VECTOR dir) override;


	// 弾の速度
	float GetSpeed(void) const override;


};

