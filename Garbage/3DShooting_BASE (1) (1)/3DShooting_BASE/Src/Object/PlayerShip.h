#pragma once
#include<DxLib.h>
#include"../Object/Common/Transform.h"
#include<vector>


class ParticleGenerator;
class SpriteAnimator;
class SpeechBalloon;
class PlayerShot;

class PlayerShip
{
public:
	enum class STATE
	{
		NONE,
		RUN,	//通常（走行）状態
		DESTROY	//破壊状態
	};

	//移動スピード
	static constexpr float SPEED_MOVE = 10.0f;

	//自機の回転量
	static constexpr float SPEED_ROT_DEG_X = 1.0f;
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;

	static constexpr float SPEED_ROT_Y = DX_PI_F / 180.0f * 1.0f;
	static constexpr float SPEED_ROT_X = DX_PI_F / 180.0f * 1.0f;

	//衝突
	static constexpr float COLLISION_RADIUS = 20.0f;

	//ブースとの間隔
	static constexpr float TIME_DELAY_BOOST = 3.0f;

	//ブースと加算スピード
	static constexpr float SPEED_BOOST = 6.0f;

	//ブーストの減速
	static constexpr float SUB_SPEED_BOOST = 2.0f;

	//弾の発射間隔
	static constexpr float TIME_DELAY_SHOT = 0.2f;

	// コンストラクタ
	PlayerShip(void);

	// デストラクタ
	~PlayerShip(void);

	void Init(void);
	void InitEffect(void);
	void Update(void);
	void UpdateRun(void);
	void UpdateDestroy(void);
	void Draw(void);
	void Release(void);

	//操作
	void ProcessTurn(void);
	void Turn(double deg, VECTOR axis);
	void ProcessBoost(void);
	void ProcessShot(void);

	const Transform& GetTransform(void) const;

	//自機破壊
	void Destroy(void);

	bool IsDestroy(void);

	//吹き出しクラスの取得
	SpeechBalloon* GetSpeechBalloon(void)const;

	std::vector<PlayerShot*>& GetShots(void);

private:

	//状態
	STATE mState;

	//パーティクルエフェクト
	ParticleGenerator* mParticleGenreator;

	Transform mTransform_;

	//爆発エフェクト
	SpriteAnimator* mExplosion;

	//吹き出し
	SpeechBalloon* mSpeechBalloon;

	//弾
	std::vector<PlayerShot*> mShots;

	//弾の発射間隔
	float mStepDelayShot;

	//爆発エフェクト(effekseer)
	int mExplosion3D;
	int mExplosion3DPlay;
	//噴射エフェクト
	int mEfectJet;
	int mEfectJetPlayT;
	int mEfectJetPlayM;
	int mEfectJetPlayD;

	int mEfectBoost;
	int mEfectBoostPlay;
	//ブースト間隔
	float mStepDelayBoost;

	//ブースト加速度
	float mSpeedBoost;

	//続きは、チェンジステートから
	void ChangeState(STATE state);

	//エフェクト(噴射)
	void SyncJetEffect(void);

	//エフェクト(加速)
	void SyncBoostEffect(void);

	//自機の弾を発射
	void CreateShot(void);
	
};

