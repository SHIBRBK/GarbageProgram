#pragma once
#include"../Common/Transform.h"

class EventShot;

class  Turret;

class SpriteAnimator;

class BossShip
{
public:
	//�ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 4.0f;

	//���e��̃C�x���g�ҋ@����
	static constexpr float TIME_EVENT = 4.0f;

	//�t�F�[�h����
	static constexpr float TIME_FADE = 8.0f;


	//�t�F�[�h�J�n�E�I�����̐F
	static constexpr COLOR_F FADE_FROM = {1.0f,1.0f,1.0f,1.0f};
	static constexpr COLOR_F FADE_TO = { 0.8f,1.0f,1.0f,0.0f };

	//�ȉ~�̔��a
	static constexpr float EX_RADIUS_X = 800.0f;
	static constexpr float EX_RADIUS_Y = 800.0f;
	static constexpr float EX_RADIUS_Z = 2000.0f;

	//�����Ԋu
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

	//��������
	bool IsAlive(void) const;

	const std::vector<Turret*> GetTurrets(void) const;

	//�{�X�̔j�󔻒�
	bool IsDestroy(void)const;

	//�{�X�̔j�󉉏o�I������
	bool IsEnd(void)const;


	//������������
	void SyncParent(Transform& transform, VECTOR addAxis);

private:

	EventShot* mEventshot;

	Transform mTransform_;

	 std::vector<Turret*> mTurrets;

	STATE mState;


	//���Ԍv��
	float mStepEvent;
	float mStepFade;
	float mStepExplosion;

	//�{�X�̑S�Ńt���O
	bool isAllDestroyTurrets;

	//�j�󎞂̔����G�t�F�N�g
	std::vector<SpriteAnimator*> mExplosions;


	//��ԑJ��
	void ChangeState(STATE state);

	//�^���b�g�̍쐬
	void MakeTurret(VECTOR localPos, VECTOR localAddAxis);

	//�{�X�̔j�󎞂̉��o(����)�쐬
	void CreateRandomExplosion(void);

	//�����G�t�F�N�g��Z�\�[�g
	void ZSortExplosion(void);
};

