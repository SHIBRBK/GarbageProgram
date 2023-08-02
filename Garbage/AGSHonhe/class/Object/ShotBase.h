#pragma once
#include<vector>
#include<DxLib.h>
#include "../Common/Transform.h"
class ShotBase
{
public:

	enum class STATE
	{
		NONE,
		SHOT,
		BLAST,
		END
	};

	ShotBase(const Transform* parent);
	virtual ~ShotBase(void);
	virtual void CreateShot(VECTOR pos, VECTOR dir);

	virtual void Update(void);


	virtual void Draw(void);

	// 弾の速度
	virtual float GetSpeed(void) const;
	virtual float GetTimeAlive(void) const;
	virtual bool CheckAlive(void);
	// 座標の取得
	VECTOR GetPos(void) const;
	// 球体当たり判定用の半径を取得
	float GetCollisionRadius(void) const;
	// 生存判定
	bool IsAlive(void) const;
	virtual void Move(void);
	virtual void Release(void) ;
	float Deg2RadF(float deg);

protected:
	// モデル制御の基本情報
	Transform transform_;

	Quaternion ShotQua;
	// 発射方向
	VECTOR mDir;
	// 生存判定
	bool mIsAlive;

	// 生存時間
	float mStepAlive;
	float speed_;
	// 当たり判定の半径
	float mCollisionRadius;
	int shotmodel;
private:
	int baseModelId_;
	int* blastImgs_;
	int blastAnimNum_;
	// 弾の標準速度
	static constexpr float DEFAULT_SPEED = 10.0f;
	static constexpr float DEFAULT_TIME_ALIVE = 10.0f;

};

