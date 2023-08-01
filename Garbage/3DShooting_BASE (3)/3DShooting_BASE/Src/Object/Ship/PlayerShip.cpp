#include<string>
#include<EffekseerForDXLib.h>
#include"../../Manager/ResourceManager.h"
#include"../../Manager/Resource.h"
#include"../../Manager/SceneManager.h"
#include"../../Utility/AsoUtility.h"
#include"./../Common/SpriteAnimator.h"
#include"./../Common/SpeechBalloon.h"
#include"../Common/SpeechBalloon.h"
#include"../../Object/Ship/PlayerShot.h"
#include"../ParticleGenerator.h"
#include "PlayerShip.h"



PlayerShip::PlayerShip(void)
{
}

PlayerShip::~PlayerShip(void)
{
}

void PlayerShip::Init(void)
{
	//���f������̊�{���
	mTransform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_SHIP));

	float SCALE = 10.0f;
	mTransform_.scl = { SCALE,SCALE,SCALE };
	mTransform_.quaRot = Quaternion();

	//mTransform_.quaRotLocal=Quaternion();

	mTransform_.quaRotLocal= Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);

	mTransform_.pos = { 0.0f,0.0f,0.0f };
	mTransform_.Update();

	//�p�[�e�B�N���G�t�F�N�g
	mParticleGenerator = new ParticleGenerator(mTransform_.pos, 10.0f);
	mParticleGenerator->Init();

	//�G�t�F�N�g�̏�����
	InitEffect();


	//�����o��
	std::string msg = "�ǂ��āI";

	SceneManager::SCENE_ID sceneID =
		SceneManager::GetInstance().GetmSceneID();

	if (sceneID == SceneManager::SCENE_ID::BATTLE)
	{
		msg = "�|����";
	}


	mSpeechBalloon = new SpeechBalloon(
		SpeechBalloon::TYPE::SPEECH, &mTransform_);


	mSpeechBalloon->SetText(msg.c_str());
	mSpeechBalloon->SetTime(15.0f);
	mSpeechBalloon->SetRelativePos({ 15.0f,15.0f,0.0f });

	//������Ԃ𑖍s��Ԃɂ���B
	ChangeState(STATE::RUN);

	//�u�[�X�g�֘A
	mStepDelayBoost = 0.0f;
	mSpeedBoost = 0.0f;
	mEffectBoostPlay = -1;


	

}

void PlayerShip::InitEffect(void)
{
	mEffectBoost = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOOST).mHandleId;

	//���@�̔����G�t�F�N�g
	mExplosion = new SpriteAnimator(
		ResourceManager::SRC::SHIP_EXPLOSION, 20.0f, 8.0f
	);

	mExplosion3D = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::EXPLOSION).mHandleId;

	mExplosion3DPlay = -1;


	mEffectJet = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::JET).mHandleId;


	mEffectJetPlayT = PlayEffekseer3DEffect(mEffectJet);
	mEffectJetPlayM = PlayEffekseer3DEffect(mEffectJet);
	mEffectJetPlayD = PlayEffekseer3DEffect(mEffectJet);
	float SCALE = 5.0f;

	SetScalePlayingEffekseer3DEffect(mEffectJetPlayT, SCALE, SCALE, SCALE);
	SetScalePlayingEffekseer3DEffect(mEffectJetPlayM, SCALE, SCALE, SCALE);
	SetScalePlayingEffekseer3DEffect(mEffectJetPlayD, SCALE, SCALE, SCALE);

	//�G�t�F�N�g����
	SyncJetEffect();
}




void PlayerShip::Update(void)
{

	
	switch (mState)
	{
	case PlayerShip::STATE::RUN:
		UpdateRun();
		break;
	case PlayerShip::STATE::DESTROY:
		UpdateDestroy();
		break;
	}


	//ChangeState(STATE::RUN);
}

void PlayerShip::UpdateRun(void)
{
	//��]����
	ProcessTurn();

	//��������
	ProcessBoost();

	//�e�̏���
	ProcessShot();

	//�O���ړ�
	VECTOR forward = mTransform_.GetForward();
	mTransform_.pos =
		VAdd(mTransform_.pos,
			VScale(forward,
				SPEED_MOVE+mSpeedBoost)
		);

	mTransform_.Update();



	//�v���C���[�̈ʒu�ƃG�t�F�N�g�̈ʒu�𓯊�
	//�O�ɔz�u�B
	VECTOR forwardPos = VAdd(mTransform_.pos, VScale(mTransform_.GetForward(), 30.0f));
	mParticleGenerator->SetPos(forwardPos);

	//�v���C���[�̌������������悤��
	Quaternion pBackRot = mTransform_.quaRot;
	pBackRot = pBackRot.Mult(
		Quaternion::AngleAxis(AsoUtility::Deg2RadD(-90.0), AsoUtility::AXIS_X));



	mParticleGenerator->SetRot(pBackRot);
	mParticleGenerator->SetPos(forwardPos);


	mParticleGenerator->Update();

	//�G�t�F�N�g����
	SyncJetEffect();


}

void PlayerShip::UpdateDestroy(void)
{
	//A:���@���j�󂳂ꂽ��J�������~�߂�
	//if (IsDestroy)
		//B:�����G�t�F�N�g�����@�̎�O�ɕ`�悷��悤�ɂ���

	//�����G�t�F�N�g

	VECTOR pos = VAdd(pos, VScale(mTransform_.GetForward(), SPEED_MOVE / 2.0f));
	//mExplosion->SetPos(mTransform_.pos);
	mExplosion->SetPos(mTransform_.pos);
	mExplosion->Update();
}


void PlayerShip::Draw(void)
{
	switch (mState)
	{

	case PlayerShip::STATE::RUN:
 		MV1DrawModel(mTransform_.modelId);

		mParticleGenerator->Draw();
		break;
	case PlayerShip::STATE::DESTROY:
		mExplosion->Draw();
		break;

	}

//���@�̓����蔻��
	//DrawSphere3D(
	// mTransform_.pos,COLLISION_RADIUS,5,0xffffff,0xffffff,false);
}


void PlayerShip::Release(void)
{

	mParticleGenerator->Release();
	delete mParticleGenerator;

	mExplosion->Release();
	delete mExplosion;

	if (mExplosion3D != 1)
	{
		StopEffekseer3DEffect(mExplosion3D);
	}
	StopEffekseer3DEffect(mEffectJetPlayT);
	StopEffekseer3DEffect(mEffectJetPlayM);
	StopEffekseer3DEffect(mEffectJetPlayD);
	StopEffekseer3DEffect(mEffectBoostPlay);

	mSpeechBalloon->Release();
	delete mSpeechBalloon;

	//�e�̊J��
	for (auto s : mShots)
	{
		s->Release();
		delete s;
	}
	mShots.clear();
}

void PlayerShip::ProcessTurn(void)
{

	//�E����
	if (CheckHitKey(KEY_INPUT_RIGHT)) 
	{
		Turn(SPEED_ROT_DEG_Y, AsoUtility::AXIS_Y);
	}

	//������
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		Turn(-SPEED_ROT_DEG_Y, AsoUtility::AXIS_Y);
	}

	//�����
	if (CheckHitKey(KEY_INPUT_UP))
	{
		Turn(-SPEED_ROT_DEG_X, AsoUtility::AXIS_X);
	}

	//������
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		Turn(SPEED_ROT_DEG_X, AsoUtility::AXIS_X);
	}

}

void PlayerShip::Turn(double deg, VECTOR axis)
{
	//�f�O���[�p�����W�A���ɕϊ�
	float rad = AsoUtility::Deg2RadF(deg);
	//�w�莲(axis)�ɑ΂����]��(rad)�ɉ�������](Quaternion)�����
	Quaternion axisQ=Quaternion::AngleAxis(rad,axis);

	//���@�̉�]�ɁB���ō쐬������]��������B
	mTransform_.quaRot = mTransform_.quaRot.Mult(axisQ);


}



const Transform& PlayerShip::GetTransform(void) const
{
	return mTransform_;
}

void PlayerShip::Destroy(void)
{
	ChangeState(STATE::DESTROY);
}

bool PlayerShip::IsDestroy(void)
{
	return mState==STATE::DESTROY;
}

SpeechBalloon* PlayerShip::GetSpeechBalloon(void) const
{
	return mSpeechBalloon;
}

std::vector<PlayerShot*>& PlayerShip::GetShots(void)
{
	return mShots;
}

void PlayerShip::ChangeState(STATE state)
{

	//��ԑJ�ڌ��̏�������

	mState = state;
	switch (mState)
	{

	case PlayerShip::STATE::RUN:
		break;

	case PlayerShip::STATE::DESTROY:
	{
		mExplosion->Create(mTransform_.pos, nullptr);


		mExplosion3DPlay = PlayEffekseer3DEffect(mExplosion3D);
		VECTOR pos = mTransform_.pos;
		
		//�����A���Ă��������Ď����ōl����E�E�E
		pos = VAdd(pos,VScale(mTransform_.GetForward(), 100.0f));
		
			
			//�ʒu
		SetPosPlayingEffekseer3DEffect(mExplosion3DPlay, pos.x, pos.y, pos.z);
		//�傫��
		SetScalePlayingEffekseer3DEffect(mExplosion3DPlay, 10.0f, 10.0f, 10.0f);
		//��]
		SetRotationPlayingEffekseer3DEffect(mExplosion3DPlay, 0.0f, 0.0f, 0.0f);

	}
		break;

	}
}

void PlayerShip::SyncJetEffect()
{
	VECTOR pos;
	const float Z = -15.0f;
	const float TY = 8.0f;
	const float DY = -5.0f;


	//�N�H�[�^�j�I������I�C���[�ɕϊ�
	const VECTOR angles = Quaternion::ToEuler(mTransform_.quaRot);
	//const VECTOR angles2 = mTransform_.quaRot.ToEuler();//�������ł�����

	//�����A���Ă��������Ď����ōl����E�E�E
	//���΍��W����]������

	
	//����������W�����
	
	
	//VECTOR localPos = VScale(mTransform_.GetBack(), 15.0f);


	////����������W�����
	////const VECTOR localMPos = { 0.0f,0.0f,Z };
	//VECTOR localPos = VScale(mTransform_.GetBack(), 15.0f);
	//pos = VAdd(mTransform_.pos, localPos);
	//SetPosPlayingEffekseer3DEffect(mEffectJetL, pos.x, pos.y, pos.z);


	//VECTOR localLeftPos = VScale(mTransform_.GetBack(), 15.0f);
	//localLeftPos = VAdd(localLeftPos, VScale(mTransform_.GetLeft(),8.0f));
	//pos = VAdd(mTransform_.pos, localLeftPos);
	//SetPosPlayingEffekseer3DEffect(mEffectJetL, pos.x, pos.y, pos.z);

#pragma region �^�񒆃G�t�F�N�g


	//���΍��W�����
	VECTOR localMPos{ 0.0f,0.0f,-15.0f };
	//���΍��W����]������
	localMPos = mTransform_.quaRot.PosAxis(localMPos);
	//���΍��W���烏�[���h���W�ɕϊ�����
	pos = VAdd(mTransform_.pos, localMPos);
	//pos = VAdd(mTransform_.pos,localLPos );
	//�G�t�F�N�g�̈ʒu�ݒ�
	SetPosPlayingEffekseer3DEffect(mEffectJetPlayM, pos.x, pos.y, pos.z);
	
#pragma endregion


#pragma region ��G�t�F�N�g
	//���΍��W�����
	VECTOR localTPos{ 8.0f,0.0f,-15.0f };
	//���΍��W����]������
	localTPos = mTransform_.quaRot.PosAxis(localTPos);
	//���΍��W���烏�[���h���W�ɕϊ�����
	pos = VAdd(mTransform_.pos, localTPos);
	//�G�t�F�N�g�̈ʒu�ݒ�
	SetPosPlayingEffekseer3DEffect(mEffectJetPlayT, pos.x, pos.y, pos.z);

#pragma endregion

#pragma region ���G�t�F�N�g
	//���΍��W�����
	VECTOR localDPos{ 0.0f,-5.0f,-15.0f };
	//���΍��W����]������
	localDPos = mTransform_.quaRot.PosAxis(localDPos);
	//���΍��W���烏�[���h���W�ɕϊ�����
	pos = VAdd(mTransform_.pos, localDPos);
	//�G�t�F�N�g�̈ʒu�ݒ�
	SetPosPlayingEffekseer3DEffect(mEffectJetPlayD, pos.x, pos.y, pos.z);

#pragma endregion



	//�G�t�F�N�g�̉�]��ݒ肷��B
	SetRotationPlayingEffekseer3DEffect(
		mEffectJetPlayM, angles.x, angles.y ,angles.z
	);

	SetRotationPlayingEffekseer3DEffect(
		mEffectJetPlayT, angles.x, angles.y, angles.z
	);

	SetRotationPlayingEffekseer3DEffect(
		mEffectJetPlayD, angles.x, angles.y, angles.z
	);
		
	
}

void PlayerShip::SyncBoostEffect()
{
	const VECTOR angles = Quaternion::ToEuler(mTransform_.quaRot);

	SetRotationPlayingEffekseer3DEffect(
		mEffectBoostPlay, angles.x,angles.y, angles.z);

#pragma region �ʒu����
	VECTOR localPos{ 0.0f,0.0f,100.0f };
	localPos = mTransform_.quaRot.PosAxis(localPos);
	VECTOR pos = VAdd(mTransform_.pos, localPos);

	SetPosPlayingEffekseer3DEffect(
		mEffectBoostPlay, pos.x, pos.y, pos.z
	);


#pragma endregion
}

void PlayerShip::ProcessBoost(void)
{
	
	mStepDelayBoost += SceneManager::GetInstance().GetDeltaTime();

	if (CheckHitKey(KEY_INPUT_B)&&mStepDelayBoost>TIME_DELAY_BOOST)
	{
		//�f�B���C(�N�[���^�C��)����
		mStepDelayBoost = 0.0f;
		
		//��������
		mSpeedBoost = SPEED_BOOST;

		//�G�t�F�N�g�Đ�
		mEffectBoost =PlayEffekseer3DEffect(mEffectBoost);
		//�傫��
		float SCALE = 10.0f;
		SetScalePlayingEffekseer3DEffect(mEffectBoostPlay, SCALE, SCALE, SCALE);

		//�G�t�F�N�g����
		SyncBoostEffect();
	}

	if (mStepDelayBoost < TIME_DELAY_BOOST&&mEffectBoostPlay>0)
	{
		SyncBoostEffect();
	}
	//��������
	if (mSpeedBoost > 0.0f) 
	{
		mSpeedBoost -=
			(SUB_SPEED_BOOST * SceneManager::GetInstance().GetDeltaTime());

		if (mSpeedBoost < 0.0f) 
		{
			mSpeedBoost = 0.0f;
		}
	}
}

void PlayerShip::ProcessShot(void)
{
	//�����͂�������
	//�e�̐����f�B���C
	mStepDelayshot -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepDelayshot < 0.0f)
	{
		mStepDelayshot = 0.0f;
	}

	//�L�[�`�F�b�N
	if (CheckHitKey(KEY_INPUT_N)&& mStepDelayshot<=0.0f)

	{
		//�f�B���C���Ԃ��Z�b�g
		mStepDelayshot = TIME_DELAY_SHOT;

		//�e�𐶐�
		CreateShot();
	}

	
}

void PlayerShip::CreateShot(void)
{
	bool isCreate = false;
	for (auto s : mShots)
	{
		if (!s->IsAlive())
		{
			s->Create(mTransform_.pos,mTransform_.GetForward());
			isCreate = true;
		}
	}
	if (!isCreate)
	{
		auto newShot = new PlayerShot(&mTransform_);
		newShot->Create(mTransform_.pos, mTransform_.GetForward());

		//�ϒ��z��ɒǉ�
		mShots.emplace_back(newShot);
	}
}
