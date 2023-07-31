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

	//�R���X�g
	EnemyShot(const Transform* parent);
	~EnemyShot(void);
	//�X�V
	void Update(void) override;

	void CreateShot(VECTOR birthPos, VECTOR dir) override;

	// �e�̑��x
	float GetSpeed(void) const override;

private:
	STATE state_;
	int baseModelId_;
	int modelId_;
	int* blastImgs_;
	int blastAnimNum_;
	int blastCntAnim_;
	int blastIdxAnim_;
	float blastSpeedAnim_;
	bool isAlive_;
	//�d��
	float gravityPow_;
	VECTOR rot_;
	VECTOR scl_;
	VECTOR pos_;
	VECTOR dir_;
	MATRIX matRot;
	MATRIX matScl;
	MATRIX matPos;
	Transform transform_;
	Quaternion qua_;
	void ChangeState(STATE state);
};

