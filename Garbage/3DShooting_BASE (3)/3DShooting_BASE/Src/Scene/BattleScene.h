#pragma once
#include<map>
#include "SceneBase.h"

class SpaceDome;
class PlayerShip;
class BossShip;
class RockManager;
class Turret;




class BattleScene : public SceneBase
{

public:

	static constexpr float  TIME_RESTART = 2.0f;

	//ボス破壊からのリスタート時間
	static constexpr float  TIME_RESTART_BOSS_D = 5.0f;

	// コンストラクタ
	BattleScene(void);

	// デストラクタ
	~BattleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	RockManager* mRockManager;
	SpaceDome* mSpaceDome;
	BossShip* mBossShip;
	PlayerShip* mPlayerShip;
	Turret* mTurret;

	//自機の破壊演出時間
	float mStepShipDestroy;

	//ゲームクリア画像
	int mImageEndLog;

	//ボス破壊後の演出時間
	float mStepBossDestroy;
};


