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

	//�{�X�j�󂩂�̃��X�^�[�g����
	static constexpr float  TIME_RESTART_BOSS_D = 5.0f;

	// �R���X�g���N�^
	BattleScene(void);

	// �f�X�g���N�^
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

	//���@�̔j�󉉏o����
	float mStepShipDestroy;

	//�Q�[���N���A�摜
	int mImageEndLog;

	//�{�X�j���̉��o����
	float mStepBossDestroy;
};


