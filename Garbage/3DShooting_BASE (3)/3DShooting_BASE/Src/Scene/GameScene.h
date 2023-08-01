#pragma once
#include <vector>
#include "SceneBase.h"


class Stage;

class SpaceDome;

class PlayerShip;

class RockManager;

class GameScene : public SceneBase
{

public:

	//自機破壊時からのリスタート時間
	static constexpr float TIME_RESTART = 2.0f;
	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//スペースドーム
	SpaceDome* mSpaceDome;

	//背景
	Stage* mStage;

	PlayerShip* mPlayerShip;

	//背景の岩
	RockManager* mRockManager;

	//自機の破壊演出時間
	float mStepShipDestroy;

};
