#pragma once
#include"../Common/Transform.h"

class EventShot;

class  Turret;

class SpriteAnimator;

class BossShip
{
public:
	//移動スピード
	static constexpr float SPEED_MOVE = 4.0f;

	//着弾後のイベント待機時間
	static constexpr float TIME_EVENT = 4.0f;

	//フェード時間
	static constexpr float TIME_FADE = 8.0f;


	//フェード開始・終了時の色
	static constexpr COLOR_F FADE_FROM = {1.0f,1.0f,1.0f,1.0f};
	static constexpr COLOR_F FADE_TO = { 0.8f,1.0f,1.0f,0.0f };

	//楕円体半径
	static constexpr float EX_RADIUS_X = 800.0f;
	static constexpr float EX_RADIUS_Y = 800.0f;
	static constexpr float EX_RADIUS_Z = 2000.0f;

	//爆発間隔
	static constexpr float TIME_EXPLOSION = 0.2f;



	enum class  STATE
	{
		NOVE,
		EVENT,
		BATTLE,
		DESTROY,
		END
	};
	BossShip(void);
	~BossShip(void);

	void Init();
	void Update();
	void UpdateTurret();
	void Draw();
	void DrawTurret();
	void Release();

	const Transform& GetTransform(void) const;

	//生存判定
	bool IsAlive(void) const;

	const std::vector<Turret*> GetTurrets(void) const;

	//ボスの破壊判定
	bool IsDestroy(void)const;

	//ボスの破壊演出終了判定
	bool IsEnd(void)const;


	//違ったら消す編
	void SyncParent(Transform& transform, VECTOR addAxis);

private:

	EventShot* mEventshot;

	Transform mTransform_;

	 std::vector<Turret*> mTurrets;

	STATE mState;


	//時間計測
	float mStepEvent;
	float mStepFade;
	float mStepExplosion;

	//ボスの全滅フラグ
	bool isAllDestroyTurrets;

	//破壊時の爆発エフェクト
	std::vector<SpriteAnimator*> mExplosions;


	//状態遷移
	void ChangeState(STATE state);

	//タレットの作成
	void MakeTurret(VECTOR localPos, VECTOR localAddAxis);

	//ボスの破壊時の演出(爆発)作成
	void CreateRandomExplosion(void);

	//爆発エフェクトのZソート
	void ZSortExplosion(void);
};

