#pragma once
#include <vector>
#include "../Common/Transform.h"
#include"ShotBase.h"

class TurretShot;
class SpriteAnimator;

class Turret: public ShotBase
{

public:

	//�Փ˔���F���̂̔���
	static constexpr float COLLISON_RADIUS = 200.0f;

	//��_���[�W�G�t�F�N�g
	static constexpr float TIME_DAMAGED_EFFECT = 2.0f;


	// �C��̃��[�J����]�ғ��p�x
	static constexpr float ANGLE_Y_MIN_STAND = -30.0f;
	static constexpr float ANGLE_Y_MAX_STAND = 30.0f;


	// �C��̃��[�J����]��
	static constexpr float ANGLE_POWER_STAND =0.2f;


	// �C�g�̃��[�J����]�ғ��p�x
	static constexpr float ANGLE_X_MIN_GUN = -10.0f;
	static constexpr float ANGLE_X_MAX_GUN = 20.0f;


	//�C�g�̃��[�J����]��
	static constexpr float ANGLE_POWER_GUN =0.2f;


	// �^���b�g�̑傫��
	static constexpr float SCALE = 80.0f;

	// ���
	enum class STATE
	{
		NONE,
		ATTACK,
		DESTROY
	};

	// �R���X�g���N�^
	Turret(
		const Transform* transformParent,
		VECTOR localPos,
		VECTOR localAddAxis);

	// �f�X�g���N�^
	~Turret(void);

	void Init(void);
	
	void Update(void) ;
	void UpdateAttack(void);
	void UpdateDestroy(void);

	void UpdateShot(void);

	void Draw(void);
	void DrawAttack(void);
	void DrawDestroy(void);

	void DrawShot(void);
	
	void Release(void);

	// �{�X���(�e���f��)�Ɖ�]�ƈʒu�̓���
	void SyncParent(Transform& transform,VECTOR addAxis);


	//�e�̎擾
	std::vector<TurretShot*>& GetShot(void);

	//��������
	bool IsAlive(void) const;

	//���W�̎擾
	VECTOR GetPos(void)const;

	//�^���b�g�ɒe��
	void Damaged(void);

	//�e�̑��x
	float GetSpeed(void) const override;

	//��������
	float GetTimeAlive(void)const;


	float GetCollisionRadius(void) const;

private:

	// �{�X�̊�{���
	const Transform* mTransformParent;

	//���g�̔����G�t�F�N�g
	SpriteAnimator* mExplosion;

	// ���f������̊�{���(�C��)
	Transform mTransformStand;

	// ���f������̊�{���(�C�g)
	Transform mTransformGun;

	// ���
	STATE mState;

	// ��͂���̑��΍��W
	VECTOR mLocalPos;

	// �ǉ��̃��[�J����]
	VECTOR mLocalAddAxis;

	// �X�ɒǉ��̖C�g���[�J����]
	VECTOR mLocalAddAxisStand;

	// �X�ɒǉ��̖C�g���[�J����]
	VECTOR mLocalAddAxisGun;

	// �ϋv��
	int mHp;

	//�C��̃��[�J���ғ���(deg)
	float mAnglePowStand;


	//�C�g�̃��[�J���ғ���(deg)
	float mAnglePowGun;

	std::vector<TurretShot*> mShots;

	float mDeleyShot;

	//��_���[�W�G�t�F�N�g�̎��Ԍv��
	float mStepDamaged;
	// ��ԑJ��
	void ChangeState(STATE state);





};
