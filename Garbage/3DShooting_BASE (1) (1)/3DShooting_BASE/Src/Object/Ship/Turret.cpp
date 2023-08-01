#include<DxLib.h>
#include"../../Application.h"
#include"../../Manager/SceneManager.h"
#include"../../Manager/Resource.h"
#include"../../Manager/ResourceManager.h"
#include"../../Common/Quaternion.h"
#include"../../Utility/AsoUtility.h"
#include"../Common/Transform.h"
#include"../Common/SpriteAnimator.h"
#include"../../Object/Ship/TurretShot2.h"
#include "Turret.h"

Turret::Turret(const Transform* transformParent, VECTOR localPos, VECTOR localAddAxis):ShotBase(transformParent)
{
	//�e
	mTransformParent = transformParent;
	//���΍��W
	mLocalPos = localPos;
	//���Ή�]
	mLocalAddAxis = localAddAxis;
	//�C��
	mTransformStand.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::TURRET_STAND));
	//�C�g
	mTransformGun.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::TURRET_GUN));

	//�C��ƖC�g�̂��ꂼ��̈ړ���]
	mLocalAddAxisStand	= VECTOR();
	mLocalAddAxisGun	= VECTOR();

	ChangeState(STATE::ATTACK);



}

Turret::~Turret(void)
{
}

void Turret::Init(void)
{
	//���f������̊�{���i�C��j
	mTransformStand.scl = { SCALE,SCALE,SCALE };
	mTransformStand.quaRot = Quaternion();

	//���[�J��
	mTransformStand.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	//�e�Ƃ̃��f���Ƃ̓����i��ɍ��W�Ɖ�]�j
	SyncParent(mTransformStand,{0.0f,0.0f,0.0f});

	//���f������̊�{���i�C��j
	mTransformGun.scl = { SCALE,SCALE,SCALE };
	mTransformGun.quaRot = Quaternion();

	//���[�J��
	mTransformGun.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(-5.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	//�e�Ƃ̃��f���Ƃ̓����i��ɍ��W�Ɖ�]�j
	SyncParent(mTransformGun,{ 0.0f,0.0f,0.0f });
	

	//�ϋv��
	mHp = 2;

	//�C�䃍�[�J���ғ���(rad)
	mAnglePowStand = ANGLE_POW_STAND;

	//�C�g���[�J���ғ���(rad)
	mAnglePowGun = ANGLE_POW_GUN;

	//�����G�t�F�N�g
	mExplosion = new SpriteAnimator(
		ResourceManager::SRC::SHIP_EXPLOSION, 120.0f, 8.0f);

	mStepDamaged = -1;

}

void Turret::Update(void)
{

	switch (mState)
	{

	case Turret::STATE::ATTACK:
		UpdateAttack();
		break;
	case Turret::STATE::DESTROY:
		UpdateDestroy();
		break;
	}


}

void Turret::UpdateAttack(void)
{

	auto delta = SceneManager::GetInstance().GetDeltaTime();

	float deg;

	//�C��̉ғ���]
	mLocalAddAxisStand.y += mAnglePowStand * delta;

	//�C��̉��͈͂̎���

	deg = AsoUtility::Rad2DegF(mLocalAddAxisStand.y);
	if (mLocalAddAxisStand.y < ANGLE_Y_MIN_STAND || mLocalAddAxisStand.y > ANGLE_Y_MAX_STAND)
	{
		mAnglePowStand *= -1.0f;
	}



	//�C���e�Ƃ̃��f���Ƃ̓����i��ɍ��W�Ɖ�]�j
	SyncParent(mTransformStand,mLocalAddAxisStand);

	//�C�g�̉ғ���]
	mLocalAddAxisGun.x += mAnglePowGun * delta;

	deg = AsoUtility::Rad2DegF(mLocalAddAxisGun.x);
	if (mLocalAddAxisGun.x < ANGLE_Y_MIN_GUN || mLocalAddAxisGun.x > ANGLE_Y_MAX_GUN)
	{
		mAnglePowGun *= -1.0f;
	}

	//�C�䂪��]�������A�C�g����]����K�v������̂Őe�q��]
	Quaternion stand = Quaternion::Euler(mLocalAddAxisStand);
	Quaternion gun = Quaternion::Euler(mLocalAddAxisGun);
	Quaternion mix = stand.Mult(gun);



	//�C�g��e�Ƃ̃��f���Ƃ̓����i��ɍ��W�Ɖ�]�j
	SyncParent(mTransformGun, mix.ToEuler());


	// ���K�F�^���b�g�̒e(���̃V���b�g�N���X�ň�x����Ă���)
	// �@�C�g6���e�����˂����
	// �A�C�g�̐悠���肩��e�����˂����
	// �B�e��ShotBase���p������(���Ȃ��Ă��悢)
	mExplosion->Update();
	// �C�N�[���^�C����݂���
	// �D���@�Ƃ̓����蔻���݂���
	// �E���@�ƒe���Փ˂�����A�o�g���V�[���̎n�߂���
	for (auto s : mShots)
	{
		s->Update();
	}

	mDeleyShot = delta;
	if (mDeleyShot < 0.0f)
	{
		ShotBase::Create(mLocalAddAxisGun, AsoUtility::DIR_F);
	}

	//��_��
	if (mStepDamaged > 0.0f)
	{
		mStepDamaged -= delta;
	}


	//�e�̏���
	ProcessShot();
}

void Turret::UpdateDestroy(void)
{

	// �e�̍X�V
	ProcessShot();

	//�����G�t�F�N�g�̍X�V
	mExplosion->Update();
}

void Turret::Draw(void)
{
	switch (mState)
	{

	case Turret::STATE::ATTACK:
		DrawAttack();
		break;
	case Turret::STATE::DESTROY:
		DrawDestroy();
		break;
	}
}

void Turret::DrawAttack(void)
{

	if (mStepDamaged > 0.0f)
	{
		MV1SetMaterialDifColor(mTransformStand.modelId,0,GetColorF(0.8f,0.1f,0.1f,0.8f));
		MV1SetMaterialDifColor(mTransformGun.modelId,0,GetColorF(0.8f,0.1f,0.1f,0.8f));
	}
	else
	{
		MV1SetMaterialDifColor(mTransformStand.modelId, 0, GetColorF(0.48f, 0.52f, 0.4f, 1.0f));
		MV1SetMaterialDifColor(mTransformGun.modelId, 0, GetColorF(0.2f, 0.4f, 0.3f, 1.0f));
	}


	MV1DrawModel(mTransformStand.modelId);
	MV1DrawModel(mTransformGun.modelId);

	for (auto s : mShots)
	{
		s->Draw();
	}
}

void Turret::DrawDestroy(void)
{

	//�e�̕`��
	for (auto s : mShots)
	{
		s->Draw();
	}

	//����
	mExplosion->Draw();

}

void Turret::Release(void)
{	
	//�e�̉��
	for (auto s : mShots)
	{
		s->Release();
		delete s;
	}
	mShots.clear();

	mExplosion->Release();
	delete mExplosion;

}

void Turret::SyncParent(Transform& transform,VECTOR addAxis)
{
	//�{�X��͂̂RD�����擾
	Quaternion parentRot = mTransformParent->quaRot;
	//�C��E�C�g�̃��[�J����]
	//Unity Z��X��Y = Y*X*Z
	Quaternion localRot;
	Quaternion axis;

	// Y
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.y, AsoUtility::AXIS_Y);
	localRot = localRot.Mult(axis);

	// X
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.x, AsoUtility::AXIS_X);
	localRot = localRot.Mult(axis);

	// Z
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.z, AsoUtility::AXIS_Z);
	localRot = localRot.Mult(axis);

	//�����ғ����̉�]������ɉ�����
	// Y
	axis = Quaternion::AngleAxis(
		addAxis.y, AsoUtility::AXIS_Y);
	localRot = localRot.Mult(axis);

	// X
	axis = Quaternion::AngleAxis(
		addAxis.x, AsoUtility::AXIS_X);
	localRot = localRot.Mult(axis);

	// Z
	axis = Quaternion::AngleAxis(
		addAxis.z, AsoUtility::AXIS_Z);
	localRot = localRot.Mult(axis);



	//�e�̉�]�ɉ������C��E�C�g
	transform.quaRot =parentRot.Mult(localRot);

	//�e�̉�]���ɍ��킹���ʒu���v�Z
	VECTOR localPos = Quaternion::PosAxis(parentRot, mLocalPos);

	//���܂ł̂���
	//���΍��W���΍��W�ɕϊ�
	//transform.pos = VAdd(mTransformParent->pos, localPos);

	//���΍��W���΍��W�ɕϊ�
	transform.pos = VAdd(mTransformParent->pos, VScale(localPos, SCALE));

	//���f������̊�{���X�V
	transform.Update();

}

void Turret::ProcessShot(void)
{
	mStepDelayShot -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepDelayShot < 0.0f)
	{
		mStepDelayShot = 0.0f;
	}
	//�L�[�`�F�b�N
	if (mStepDelayShot <= 0.0f)
	{
		//�f�B���C���Ԃ��Z�b�g
		mStepDelayShot = TIME_DELAY_SHOT;

		//�e�𐶐�
		CreateShot();

	}
}

std::vector<TurretShot2*>& Turret::GetShots(void)
{
	return mShots;
}

bool Turret::IsAlive(void) const
{
	return mState == STATE::ATTACK;
}

VECTOR Turret::GetPos(void) const
{
	return mTransformStand.pos;
}

void Turret::Damage(void)
{

	mStepDamaged = TIME_DAMAGED_EFFECT;

	mHp -= 1;

	if (mHp <= 0)
	{
		ChangeState(STATE::DESTROY);
	}

}

void Turret::ChangeState(STATE state)
{
	 
}

void Turret::CreateShot(void)
{
	bool isCreate = false;
	for (auto s : mShots)
	{
		if (!s->IsAlive())
		{
			//�������Ă��Ȃ�������
			//�C���X�^���X���g���܂킷
			s->Create(mTransformGun.pos, mTransformGun.GetForward());
			isCreate = true;
		}
	}

	if (!isCreate)
	{
		//�C���X�^���X��V�������
		auto newShot = new TurretShot2(&mTransformGun);
		newShot->Create(mTransformGun.pos, mTransformGun.GetForward());

		//�ϒ��z��ɒǉ�
		mShots.emplace_back(newShot);
	}
}
