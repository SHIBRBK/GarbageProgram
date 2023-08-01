#pragma once
#include"../Common/Transform.h"
#include<vector>

class EventShot;
class Turret;
class SpriteAnimator;

class BossShip
{
public:

	static constexpr float SPEED_MOVE = 4.0f;
	//着弾後のイベント待機時間
	static constexpr float TIME_EVENT = 4.0f;

	//フェード時間
	static constexpr float TIME_FADE = 8.0f;

	//フェード開始・終了時の色
	static constexpr COLOR_F FADE_FROM = { 1.0f,1.0f,1.0f,1.0f };
	static constexpr COLOR_F FADE_TO   = { 0.8f,0.1f,0.1f,0.0f };

	//楕円体半径
	static constexpr float EX_RADIUS_X = 800.0f;
	static constexpr float EX_RADIUS_Y = 800.0f;
	static constexpr float EX_RADIUS_Z = 2000.0f;

	//爆発間隔
	static constexpr float TIME_EXPLOSION = 0.2f;

	//状態
	enum class STATE
	{
		NONE,
		EVENT,
		BATTLE,
		DESTROY,
		END
	};


	// コンストラクタ
	BossShip(void);

	// デストラクタ
	~BossShip(void);


	void Init(void);
	void Update(void);
	void UpdateTurret(void);
	void Draw(void);
	void DrawTurret(void);
	void Release(void);

	//3D制御の基本情報を読み取り専用で取得 
	const Transform& GetTransform(void) const;

	//生存判定
	bool IsAlive(void) const;

	std::vector<Turret*> GetTurrets(void)const;

	bool IsDestroy(void) const;

	bool IsEnd(void) const;

private:

	//イベント用の弾
	EventShot* mEventShot;

	//３Dモデルの基本情報
	Transform mTransform;

	//タレット
	std::vector<Turret*>mTurrets;


	//状態
	STATE mState;

	//時間計測
	float mStepEvent;
	float mStepFade;
	float mStepExplosion;

	//タレットのゼンメツフラグ
	bool mIsAllDestroyTurrets;

	//破壊時の爆発エフェクト
	std::vector<SpriteAnimator*> mExplosions;

	//状態遷移
	void ChangeState(STATE state);
	
	//タレットの作成
	void MakeTurret(VECTOR localPos, VECTOR localAddAxis);

	//ボス破壊時の演出(爆発)作成
	void CreateRandomExplosion(void);

	//爆発エフェクトのZソート
	void ZSortExplosion(void);
};

