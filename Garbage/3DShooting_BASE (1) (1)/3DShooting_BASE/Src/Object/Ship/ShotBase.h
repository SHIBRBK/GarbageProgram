#pragma once
#include <DxLib.h>
#include "../../Common/Quaternion.h"
#include "../../Object/Common/Transform.h"
class SpriteAnimator;

class ShotBase
{

public:

	// コンストラクタ・Init・Createの区別がややこしいので、
	// Init無し
	ShotBase(const Transform* parent);
	virtual ~ShotBase(void);

	/// <summary>
	/// 弾の生成
	/// </summary>
	/// <param name="birthPos">初期座標</param>
	/// <param name="dir">進行方向</param>
	virtual void Create(VECTOR birthPos, VECTOR dir);

	virtual void Update(void);

	// 生存チェック＆生存判定
	virtual bool CheckAlive(void);

	virtual void Move(void);
	virtual void Draw(void);
	virtual void Release(void);

	// 爆発エフェクト生成
	virtual void CreateExplosion(const Transform* transform);

	// 弾の速度
	virtual float GetSpeed(void) const;

	// 生存時間
	virtual float GetTimeAlive(void) const;

	// 生存判定
	bool IsAlive(void) const;

	// 座標の取得
	VECTOR GetPos(void) const;

	// 球体当たり判定用の半径を取得
	float GetCollisionRadius(void) const;

protected:

	// 弾の爆発エフェクト
	SpriteAnimator* mExplosion;

	// モデル制御の基本情報
	Transform mTransform;

	// 発射方向
	VECTOR mDir;

	// 生存判定
	bool mIsAlive;

	// 生存時間
	float mStepAlive;

	// 当たり判定の半径
	float mCollisionRadius;

private:

	// 弾の標準速度
	static constexpr float DEFAULT_SPEED = 35.0f;

	// 弾の生存時間
	static constexpr float DEFAULT_TIME_ALIVE = 5.0f;

};