#pragma once
#include "../Common/Transform.h"

class SkyDome
{
public:

	static constexpr float SCALE = 100.0f;
	static constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };

	// 状態
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	// コンストラクタ
	SkyDome(const Transform* syncTransform);

	// デストラクタ
	~SkyDome(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// 自機の情報
	const Transform* mSyncTransform;

	// 状態
	STATE mState;

	// モデル制御の基本情報
	Transform transform_;

	// 状態遷移
	void ChangeState(STATE state);
	float Deg2RadF(float deg);
};
