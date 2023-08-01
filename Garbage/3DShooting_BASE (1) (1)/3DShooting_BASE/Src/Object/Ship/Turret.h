#pragma once
#include <vector>
#include "../Common/Transform.h"
#include "ShotBase.h"

class TurretShot2;
class SpriteAnimator;

class Turret:public ShotBase
{

public:

	//衝突判定；球体の半径
	static constexpr float COLLISION_RADIUS = 200.0f;

	//被ダメージエフェクト
	static constexpr float TIME_DAMAGED_EFFECT = 2.0f;

	//砲台ローカル回転稼働角
	static constexpr float ANGLE_Y_MIN_STAND = (DX_PI_F) / 180 * -30.0f;
	static constexpr float ANGLE_Y_MAX_STAND = (DX_PI_F) / 180 * 30.0f;

	//砲台ローカル回転量
	static constexpr float ANGLE_POW_STAND = 0.2f;

	//砲身ローカル回転稼働角
	static constexpr float ANGLE_Y_MIN_GUN = (DX_PI_F) / 180 * -10.0f;
	static constexpr float ANGLE_Y_MAX_GUN = (DX_PI_F) / 180 * 20.0f;

	//弾の発射間隔
	static constexpr float TIME_DELAY_SHOT = 0.2f;

	//砲身のローカル回転量
	static constexpr float ANGLE_POW_GUN = 0.2f;

	// タレットの大きさ
	static constexpr float SCALE = 80.0f;

	// 状態
	enum class STATE
	{
		NONE,
		ATTACK,
		DESTROY
	};

	// コンストラクタ
	Turret(
		const Transform* transformParent,
		VECTOR localPos,
		VECTOR localAddAxis);

	// デストラクタ
	~Turret(void);

	void Init(void);

	void Update(void);
	void UpdateAttack(void);
	void UpdateDestroy(void);

	void Draw(void);
	void DrawAttack(void);
	void DrawDestroy(void);

	void Release(void);

	// ボス戦艦(親モデル)と回転と位置の同期
	void SyncParent(Transform& transform,VECTOR addAxis);

	void ProcessShot(void);

	std::vector<TurretShot2*>& GetShots(void);

	//弾の発射間隔
	float mStepDelayShot;

	//生存判定
	bool IsAlive(void) const;

	VECTOR GetPos(void) const;

	//タレットにダメージを与える
	void Damage(void);

private:

	//被ダメージエフェクトの時間計測用
	float mStepDamaged;

	// ボスの基本情報
	const Transform* mTransformParent;

	//自身の爆発エフェクトのスプライト
	SpriteAnimator* mExplosion;

	// モデル制御の基本情報(砲台)
	Transform mTransformStand;

	// モデル制御の基本情報(砲身)
	Transform mTransformGun;

	std::vector<TurretShot2*> mShots;

	float mDeleyShot;

	// 状態
	STATE mState;

	// 戦艦からの相対座標
	VECTOR mLocalPos;

	// 追加のローカル回転
	VECTOR mLocalAddAxis;

	// 更に追加の砲身ローカル回転
	VECTOR mLocalAddAxisStand;

	// 更に追加の砲身ローカル回転
	VECTOR mLocalAddAxisGun;

	// 耐久力
	int mHp;

	//砲台のローカル稼働量 (deg)
	float mAnglePowStand;

	//砲身のローカル稼働量 (deg)
	float mAnglePowGun;

	// 状態遷移
	void ChangeState(STATE state);
	//自機の弾を発射
	void CreateShot(void);

};
