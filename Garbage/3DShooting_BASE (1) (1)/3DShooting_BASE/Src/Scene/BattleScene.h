#pragma once
#include"SceneBase.h"

class SpaceDome;
class BossShip;
class RockManager;
class PlayerShip;
class Turret;

class BattleScene : public SceneBase
{
public:

	//自機破壊時からのリスタート時間
	static constexpr float TIME_RESTART = 2.0f;

	//ボス破壊からのリスタート
	static constexpr float TIME_RESTART_BOSS_DESTROY = 2.0f;




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

	//ボスの破壊演出時間 
	float mStepBossDestroy;

	//ゲームクリア画像
	int mImgEndLogo;

	//自機の破壊演出時間 
	float mStepShipDestroy;
};

