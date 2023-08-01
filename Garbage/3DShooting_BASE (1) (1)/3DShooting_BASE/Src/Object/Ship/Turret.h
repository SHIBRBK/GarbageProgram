#pragma once
#include <vector>
#include "../Common/Transform.h"
#include "ShotBase.h"

class TurretShot2;
class SpriteAnimator;

class Turret:public ShotBase
{

public:

	//�Փ˔���G���̂̔��a
	static constexpr float COLLISION_RADIUS = 200.0f;

	//��_���[�W�G�t�F�N�g
	static constexpr float TIME_DAMAGED_EFFECT = 2.0f;

	//�C�䃍�[�J����]�ғ��p
	static constexpr float ANGLE_Y_MIN_STAND = (DX_PI_F) / 180 * -30.0f;
	static constexpr float ANGLE_Y_MAX_STAND = (DX_PI_F) / 180 * 30.0f;

	//�C�䃍�[�J����]��
	static constexpr float ANGLE_POW_STAND = 0.2f;

	//�C�g���[�J����]�ғ��p
	static constexpr float ANGLE_Y_MIN_GUN = (DX_PI_F) / 180 * -10.0f;
	static constexpr float ANGLE_Y_MAX_GUN = (DX_PI_F) / 180 * 20.0f;

	//�e�̔��ˊԊu
	static constexpr float TIME_DELAY_SHOT = 0.2f;

	//�C�g�̃��[�J����]��
	static constexpr float ANGLE_POW_GUN = 0.2f;

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

	void Update(void);
	void UpdateAttack(void);
	void UpdateDestroy(void);

	void Draw(void);
	void DrawAttack(void);
	void DrawDestroy(void);

	void Release(void);

	// �{�X���(�e���f��)�Ɖ�]�ƈʒu�̓���
	void SyncParent(Transform& transform,VECTOR addAxis);

	void ProcessShot(void);

	std::vector<TurretShot2*>& GetShots(void);

	//�e�̔��ˊԊu
	float mStepDelayShot;

	//��������
	bool IsAlive(void) const;

	VECTOR GetPos(void) const;

	//�^���b�g�Ƀ_���[�W��^����
	void Damage(void);

private:

	//��_���[�W�G�t�F�N�g�̎��Ԍv���p
	float mStepDamaged;

	// �{�X�̊�{���
	const Transform* mTransformParent;

	//���g�̔����G�t�F�N�g�̃X�v���C�g
	SpriteAnimator* mExplosion;

	// ���f������̊�{���(�C��)
	Transform mTransformStand;

	// ���f������̊�{���(�C�g)
	Transform mTransformGun;

	std::vector<TurretShot2*> mShots;

	float mDeleyShot;

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

	//�C��̃��[�J���ғ��� (deg)
	float mAnglePowStand;

	//�C�g�̃��[�J���ғ��� (deg)
	float mAnglePowGun;

	// ��ԑJ��
	void ChangeState(STATE state);
	//���@�̒e�𔭎�
	void CreateShot(void);

};
