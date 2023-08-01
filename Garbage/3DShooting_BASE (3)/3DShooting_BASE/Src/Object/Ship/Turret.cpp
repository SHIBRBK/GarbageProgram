#include<DxLib.h>
#include"../../Application.h"
#include"../../Manager/SceneManager.h"
#include"../../Manager/ResourceManager.h"
#include"../../Manager/Resource.h"
#include"../../Common/Quaternion.h"
#include"../../Utility/AsoUtility.h"
#include"../Common/Transform.h"
#include"../Common/SpriteAnimator.h"
#include"TurretShot.h"
#include"Turret.h"

Turret::Turret(
	const Transform* transformParent, 
	VECTOR localPos, VECTOR localAddAxis)
	:ShotBase(transformParent)//���������B�v�}�[�N�ȁB
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
	mTransformStand.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::TURRET_GUN));


	//�C��ƖC�g�̂��ꂼ��̈ړ���]
	mLocalAddAxisStand = VECTOR();
	mLocalAddAxisGun = VECTOR();

	//�������
	ChangeState(STATE::ATTACK);

}

Turret::~Turret(void)
{



}

void Turret::Init(void)
{
	//���f������̊�{���
	mTransformStand.scl = { SCALE,SCALE,SCALE };
	mTransformStand.quaRot = Quaternion();

	//���[�J����]
	mTransformStand.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	//�e���f���Ƃ̓���(��ɍ��W�Ɖ�])
	SyncParent(mTransformStand,{0.0f,0.0f,0.0f});





	//���f������̊�{���(�C�g)
	mTransformGun.scl = { SCALE,SCALE,SCALE };
	mTransformGun.quaRot = Quaternion();

	//���[�J����]
	mTransformGun.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(-5.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)

	);

	//�e���f���Ƃ̓���(��ɍ��W�Ɖ�])
	SyncParent(mTransformGun, { 0.0f,0.0f,0.0f });

	//�ϋv��
	mHp = 2;


	//�C��ƖC�g�̉ғ���(rad)
	mAnglePowStand = ANGLE_POWER_STAND;
	mAnglePowGun = ANGLE_POWER_GUN;

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

	//�C��̉ғ���]
	mLocalAddAxisStand.y += mAnglePowStand * delta;//Y�����ǂ�ǂ�E���


	//���͈͂̎���
	//�C�䂪�I�������C�g���Ċ����B

	float deg;
	deg = AsoUtility::Rad2DegF(mLocalAddAxisStand.y);
	if (deg<ANGLE_Y_MIN_STAND||deg>ANGLE_Y_MAX_STAND)
	{
		mAnglePowStand *= -1.0f;
	}


	//�C��e���f���Ƃ̓���������
	SyncParent(mTransformStand,mLocalAddAxisStand);



	//�C�g�̉ғ���]
	mLocalAddAxisGun.x += mAnglePowGun * delta;

	deg=AsoUtility::Rad2DegF(mLocalAddAxisGun.x);
	if (deg<ANGLE_X_MIN_GUN || deg>ANGLE_X_MAX_GUN)
	{
		mAnglePowGun *= -1.0f;
	}

	//�C�䂪��]�������A�C�g����]����K�v������̂Őe�q��]
	Quaternion stand = Quaternion::Euler(mLocalAddAxisStand);
	Quaternion gun = Quaternion::Euler(mLocalAddAxisGun);
	Quaternion mix = stand.Mult(gun);




	//�C�g��e���f���Ɠ���������
	SyncParent(mTransformGun,mix.ToEuler());



	//�@�C�g6��狅�����˂����
	//�A�C�g�̐擖���肩��e�����˂����(�ʒu����)
	// 
	//�B�e��ShotBase���p������(���Ȃ��Ă��ǂ�)
	
	mExplosion->Update();
	//�C�N�[���^�C����݂���
	

	//�D���@�Ƃ̓����蔻���݂���
	 
	//�E���@�ƒe���Փ˂�����A�o�g���V�[���̎n�߂���B

	



	//�e�̔��ˊԊu
	mDeleyShot = delta;
	if (mDeleyShot < 0.0f)
	{
		ShotBase::Create(mLocalAddAxisGun,AsoUtility::DIR_F);
	}



	//�e�̍X�V
	UpdateShot();

	//��_������
	if (mStepDamaged > 0.0f)
	{
		mStepDamaged -= delta;
	}
}

void Turret::UpdateDestroy(void)
{

	//�e�̍X�V
	UpdateShot();

	//�����G�t�F�N�g�̍X�V
	mExplosion->Update();



}

void Turret::UpdateShot(void)
{
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
		MV1SetMaterialDifColor(
			mTransformStand.modelId,0,GetColorF(0.8f,0.1f,0.1f,0.8f));
		MV1SetMaterialDifColor(
			mTransformGun.modelId, 0, GetColorF(0.8f, 0.1f, 0.1f, 0.8f));
	}
	else
	{
		MV1SetMaterialDifColor(
			mTransformStand.modelId, 0, GetColorF(0.48f, 0.52f, 0.4f, 1.0f));
		MV1SetMaterialDifColor(
			mTransformGun.modelId, 0, GetColorF(0.2f, 0.4f, 0.3f, 1.0f));
	}


	MV1DrawModel(mTransformStand.modelId);
	MV1DrawModel(mTransformGun.modelId);

	//�e�̕`��
	DrawShot();

}

void Turret::DrawDestroy(void)
{
	//�e�̕`��
	DrawShot();

	//�����G�t�F�N�g�̕`��
	mExplosion->Draw();


}

void Turret::DrawShot(void)
{
	for (const auto s : mShots)
	{
		s->Draw();
	}
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

	//�����G�t�F�N�g�̉��
	mExplosion->Release();
	delete mExplosion;
}

void Turret::SyncParent(Transform& transform,VECTOR addAxis)
{
	//�{�X��͂�3D���
	Quaternion parentRot = mTransformParent->quaRot;

	//�C��A�C�g�̃��[�J����]��Quaternion�ō��B
	//Unity Z->X->Y=Y*X*Z

	Quaternion localRot;
	Quaternion axis;

	//Y
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.y,AsoUtility::AXIS_Y);
	localRot=localRot.Mult(axis);


	//X
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.x, AsoUtility::AXIS_X);
	localRot = localRot.Mult(axis);

	//Z
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.z, AsoUtility::AXIS_Z);
	localRot = localRot.Mult(axis);




	//�����ғ����̉�]������ɉ�����B


		//�A�i�U�[Y
	axis = Quaternion::AngleAxis(
		addAxis.y, AsoUtility::AXIS_Y);
	localRot = localRot.Mult(axis);


	//�A�i�U�[X
	axis = Quaternion::AngleAxis(
		addAxis.x, AsoUtility::AXIS_X);
	localRot = localRot.Mult(axis);

	//�A�i�U�[Z
	axis = Quaternion::AngleAxis(
		addAxis.z, AsoUtility::AXIS_Z);
	localRot = localRot.Mult(axis);










	//�e�̉�]�ɉ����ĖC��E�C�g����]������B
	transform.quaRot = localRot.Mult(localRot);


	//�e�̉�]���ɍ��킹���ʒu���v�Z
	VECTOR localPos = Quaternion::PosAxis(parentRot,mLocalPos);

	//���܂ŏK�����̂���
	////���΍��W���΍��W�ɕϊ�
	//transform.pos = VAdd(
	//	mTransformParent->pos, localPos);


	//�e�̑傫�����A�X�P�[���������΍��W���΍��W�ɕϊ�
	transform.pos = VAdd(
		mTransformParent->pos, VScale(localPos,SCALE));//�X�P�[���Ŋ|���Z���Ă�����̂��{���̂���



		
		//���f������̊�{���X�V
	transform.Update();


}

std::vector<TurretShot*>& Turret::GetShot(void)
{
	return mShots;
}

bool Turret::IsAlive(void) const
{
	return mState==STATE::ATTACK;
}

VECTOR Turret::GetPos(void) const
{
	return mTransformStand.pos;
}

void Turret::Damaged(void)
{
	mStepDamaged = TIME_DAMAGED_EFFECT;
	mHp -= 1;
	if (mHp<=0) 
	{
		ChangeState(STATE::DESTROY);
	}
}



void Turret::ChangeState(STATE state)
{
	//��Ԃ��X�V
	mState = state;
	//��ԑJ�ڎ�������
	switch (mState)
	{

	case Turret::STATE::ATTACK:
		break;
	case Turret::STATE::DESTROY:
	{
		mExplosion->Create(mTransformGun.pos, nullptr);
	}
		break;

	}


}


float Turret::GetSpeed(void) const
{
	return 20.0f;
}

float Turret::GetTimeAlive(void) const
{
	return 10.0f;
}

float Turret::GetCollisionRadius(void) const
{
	return mCollisionRadius;
}
