#pragma once
#include <memory>
#include "ShotBase.h"
#include"BattleShip.h"
#include"../Common/Transform.h"
class PlayerShot :
    public ShotBase
{
public:
	static constexpr float SCALE = 70.0f;
	static constexpr float COL_RADIUS = 10.0f;

	//コンスト
	PlayerShot(const Transform* parent);
	 ~PlayerShot(void);

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
	//重力
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

