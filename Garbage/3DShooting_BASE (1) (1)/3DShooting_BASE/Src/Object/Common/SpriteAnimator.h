#pragma once
#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
class Transform;

class SpriteAnimator
{

public:

	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		END
	};

	// コンストラクタ
	SpriteAnimator(ResourceManager::SRC src, float size, float speed);

	// デストラクタ
	~SpriteAnimator(void);

	// Init・Createの区別がややこしいので、Init無し

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="pos">初期表示座標</param>
	/// <param name="transform">指定されたオブジェクトを追跡する</param>
	void Create(VECTOR pos, const Transform* transform);

	void Update(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;
	void SetPos(VECTOR pos);

	float GetZLen(void) const;
	void SetZLen(float len);

	// アニメーション終了
	bool IsEnd(void) const;

private:

	// 追跡対象オブジェクト
	const Transform* mTrackingTransform;

	// １フレーム前の追跡対象座標
	VECTOR mTrackingPrePos;

	// 状態
	STATE mState;

	// 画像ハンドル
	int* mImages;

	// 再生インデックス
	int mIdxAnim;

	// 座標
	VECTOR mPos;

	// 角度
	VECTOR mAngles;

	// 大きさ
	float mSize;

	// アニメーション用更新ステップ
	float mStepAnim;

	// カメラからのZ距離
	float mZLen;

	// 総アニメーション数
	int mCntTotalAnim;

	// アニメーションスピード
	float mSpeedAnim;

	// 状態遷移
	void ChangeState(STATE state);

};
