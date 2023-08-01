#pragma once
#include"../Common/Transform.h"
class ParticleGenerator;
class SpriteAnimator;
class SpeechBalloon;
class PlayerShot;

class PlayerShip
{
public:

	//状態
	enum class STATE
	{
		NONE,
		RUN,	//通常状態
		DESTROY//破壊状態
	};
	static constexpr float SPEED_MOVE = 10.0f;

	static constexpr float SPEED_ROT_DEG_X = 1.0f;
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;
	//static constexpr float SPEED_ROT_Y = DX_PI_F / 180.0f;

	//衝突判定用の球体半径
	static constexpr float COLLISION_RADIUS = 20.0f;

	//ブーストの間隔
	static constexpr float TIME_DELAY_BOOST = 3.0f;

	//ブーストの加算スピード
	static constexpr float SPEED_BOOST = 6.0f;

	//ブーストの減速
	static constexpr float SUB_SPEED_BOOST = 2.0f;

	//弾の発射感覚
	static constexpr float TIME_DELAY_SHOT = 0.2f;




	PlayerShip(void);
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

	SpeechBalloon* GetSpeechBalloon(void) const;
	//GameSceneにstageクラスを宣言して、実行できるようにしてください。
	//DrawGridでXYZを描画してください

	
	std::vector<PlayerShot*>& GetShots(void);

private:

	STATE mState;

	//パーティクルエフェクト
	ParticleGenerator* mParticleGenerator;


	Transform mTransform_;

	//爆発エフェクト
	SpriteAnimator* mExplosion;

	//吹き出し
	SpeechBalloon* mSpeechBalloon;

	//弾
	std::vector<PlayerShot*> mShots;
	float mStepDelayshot;

	//爆発エフェクト(Effelseer)
	int mExplosion3D;
	int mExplosion3DPlay;

	//噴射エフェクト
	int mEffectJet;
	int mEffectJetPlayT;
	int mEffectJetPlayM;
	int mEffectJetPlayD;

	//加速エフェクト
	int mEffectBoost;
	int mEffectBoostPlay;
	 
	//ブースト間隔
	float mStepDelayBoost;

	
	float mSpeedBoost;

	//爆発エフェクト(Effelseer)
	int mExplosionBoost;
	int mExplosionBoostPlay;

	//続きはチェンジステートから作る
	void ChangeState(STATE state);

	//エフェクト制御
	void SyncJetEffect(void);

	void SyncBoostEffect(void);
	

	void CreateShot(void);
};

