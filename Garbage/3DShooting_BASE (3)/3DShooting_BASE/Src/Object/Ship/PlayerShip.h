#pragma once
#include"../Common/Transform.h"
class ParticleGenerator;
class SpriteAnimator;
class SpeechBalloon;
class PlayerShot;

class PlayerShip
{
public:

	//���
	enum class STATE
	{
		NONE,
		RUN,	//�ʏ���
		DESTROY//�j����
	};
	static constexpr float SPEED_MOVE = 10.0f;

	static constexpr float SPEED_ROT_DEG_X = 1.0f;
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;
	//static constexpr float SPEED_ROT_Y = DX_PI_F / 180.0f;

	//�Փ˔���p�̋��̔��a
	static constexpr float COLLISION_RADIUS = 20.0f;

	//�u�[�X�g�̊Ԋu
	static constexpr float TIME_DELAY_BOOST = 3.0f;

	//�u�[�X�g�̉��Z�X�s�[�h
	static constexpr float SPEED_BOOST = 6.0f;

	//�u�[�X�g�̌���
	static constexpr float SUB_SPEED_BOOST = 2.0f;

	//�e�̔��ˊ��o
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
	//����
	void ProcessTurn(void);
	void Turn(double deg, VECTOR axis);
	void ProcessBoost(void);
	void ProcessShot(void);


	const Transform& GetTransform(void) const;

	//���@�j��
	void Destroy(void);

	bool IsDestroy(void);

	SpeechBalloon* GetSpeechBalloon(void) const;
	//GameScene��stage�N���X��錾���āA���s�ł���悤�ɂ��Ă��������B
	//DrawGrid��XYZ��`�悵�Ă�������

	
	std::vector<PlayerShot*>& GetShots(void);

private:

	STATE mState;

	//�p�[�e�B�N���G�t�F�N�g
	ParticleGenerator* mParticleGenerator;


	Transform mTransform_;

	//�����G�t�F�N�g
	SpriteAnimator* mExplosion;

	//�����o��
	SpeechBalloon* mSpeechBalloon;

	//�e
	std::vector<PlayerShot*> mShots;
	float mStepDelayshot;

	//�����G�t�F�N�g(Effelseer)
	int mExplosion3D;
	int mExplosion3DPlay;

	//���˃G�t�F�N�g
	int mEffectJet;
	int mEffectJetPlayT;
	int mEffectJetPlayM;
	int mEffectJetPlayD;

	//�����G�t�F�N�g
	int mEffectBoost;
	int mEffectBoostPlay;
	 
	//�u�[�X�g�Ԋu
	float mStepDelayBoost;

	
	float mSpeedBoost;

	//�����G�t�F�N�g(Effelseer)
	int mExplosionBoost;
	int mExplosionBoostPlay;

	//�����̓`�F���W�X�e�[�g������
	void ChangeState(STATE state);

	//�G�t�F�N�g����
	void SyncJetEffect(void);

	void SyncBoostEffect(void);
	

	void CreateShot(void);
};

