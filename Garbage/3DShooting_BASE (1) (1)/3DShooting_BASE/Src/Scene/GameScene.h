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

	//���@�j�󎞂���̃��X�^�[�g����
	static constexpr float TIME_RESTART = 2.0f;

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	SpaceDome* mSpaceDome;

	Stage* mStage;

	PlayerShip* mPlayerShip;
	RockManager* mRockManager;

	//���@�̔j�󉉏o���� 
	float mStepShipDestroy;

};
