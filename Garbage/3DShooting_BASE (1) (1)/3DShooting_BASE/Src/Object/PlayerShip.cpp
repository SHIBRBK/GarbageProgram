#include"../Manager/ResourceManager.h"
#include<EffekseerForDXLIB.h>
#include<string>
#include"../Manager/Resource.h"
#include"../Manager/Scenemanager.h"
#include"../Utility/AsoUtility.h"
#include"ParticleGenerator.h"
#include "../Object/Ship/PlayerShot.h"
#include "PlayerShip.h"
#include"./Common/SpriteAnimator.h"
#include"./Common/SpeechBalloon.h"



PlayerShip::PlayerShip(void)
{
}

PlayerShip::~PlayerShip(void)
{
}

void PlayerShip::Init(void)
{
	//���f������ɕK�v�Ȋ�{���
	mTransform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_SHIP));

	float SCALE = 0.04f;
	mTransform_.scl = { 0.04f,0.04f,0.04f };
	mTransform_.quaRot = Quaternion();
	//mTransform.quaRotLocal = Quaternion();
	mTransform_.quaRotLocal = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	mTransform_.pos = { 0.0f,0.0f,0.0f };
	mTransform_.Update();


	//�G�t�F�N�g�̏�����
	InitEffect();

	//�����o��
	std::string msg = "�ǂ��āI";
	SceneManager::SCENE_ID sceneId =
		SceneManager::GetInstance().GetmSceneID();

	if (sceneId == SceneManager::SCENE_ID::BATTLE)
	{
		msg = "�|���I�I";
	}
	mSpeechBalloon = new SpeechBalloon(
		SpeechBalloon::TYPE::SPEECH, &mTransform_
	);

	mSpeechBalloon->SetText(msg.c_str());
	mSpeechBalloon->SetTime(15.0f);
	mSpeechBalloon->SetRelativePos({ 15.0f,15.0f,0.0f });

	//�p�[�e�B�N���G�t�F�N�g
	mParticleGenreator = new ParticleGenerator(mTransform_.pos, 12.0f);
	mParticleGenreator->Init();


	//������Ԃ𑖍s��Ԃɂ���
	ChangeState(STATE::RUN);


	mStepDelayBoost = 0.0f;
	mSpeedBoost = 0.0f;

}

void PlayerShip::InitEffect(void)
{
	mExplosion = new SpriteAnimator(
		ResourceManager::SRC::SHIP_EXPLOSION, 20.0f, 8.0f
	);


	mExplosion3D = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::EXPLOSION).mHandleId;

	mExplosion3DPlay = -1;


	mEfectJet = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::JET).mHandleId;

	mEfectBoost= ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOOST).mHandleId;



	mEfectJetPlayT=PlayEffekseer3DEffect(mEfectJet);
	mEfectJetPlayM=PlayEffekseer3DEffect(mEfectJet);
	mEfectJetPlayD=PlayEffekseer3DEffect(mEfectJet);
	float SCALE = 5.0f;
	SetScalePlayingEffekseer3DEffect(mEfectJetPlayT, SCALE, SCALE, SCALE);
	SetScalePlayingEffekseer3DEffect(mEfectJetPlayM, SCALE, SCALE, SCALE);
	SetScalePlayingEffekseer3DEffect(mEfectJetPlayD, SCALE, SCALE, SCALE);


	

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
	//�ʒu���X�V
	mTransform_.pos = 
		VAdd(mTransform_.pos,
			VScale(forward, SPEED_MOVE + mSpeedBoost));
	//mTransform.pos.z += SPEED_MOVE;
	mTransform_.Update();


	//�v���C���[�̈ʒu�ƃG�t�F�N�g�̈ʒu�𓯊�
	VECTOR forwardPos = VAdd(mTransform_.pos, VScale(mTransform_.GetForward(), 30.0f));
	mParticleGenreator->SetPos(forwardPos);

	//�v���C���[�̌������������悤��
	//Quaternion pBackRot=mTransform.quaRot;
	//pBackRot= pBackRot.Mult()
	//mParticleGenerator->SetRot();
	//mParticleGenerator->Update();

		//�G�t�F�N�g����
	SyncJetEffect();
	

}

void PlayerShip::UpdateDestroy(void)
{
	//A�F���@���j�󂹂ꂽ��J�������~�߂�



	//B�F�����G�t�F�N�g�����@�̖ڂ̑O�ɕ`�悷��



	//�����G�t�F�N�g
	mExplosion->SetPos(mTransform_.pos);
	mExplosion->Update();

}
void PlayerShip::Draw(void)
{
	switch (mState)
	{
	case PlayerShip::STATE::RUN:
		MV1DrawModel(mTransform_.modelId);
		mParticleGenreator->Draw();
		break;
	case PlayerShip::STATE::DESTROY:
		//�����G�t�F�N�g
		mExplosion->Draw();
		break;
	}

	//���@�̓����蔻��
	DrawSphere3D(
		mTransform_.pos, COLLISION_RADIUS, 5, 0xffffff, 0xffffff, false);

}

void PlayerShip::Release(void)
{
	mParticleGenreator->Release();
	delete mParticleGenreator;


	if (mExplosion3DPlay!=-1)
	{
		StopEffekseer3DEffect(mExplosion3DPlay);
	}

	StopEffekseer3DEffect(mEfectJetPlayT);
	StopEffekseer3DEffect(mEfectJetPlayM);
	StopEffekseer3DEffect(mEfectJetPlayD);
	StopEffekseer3DEffect(mEfectBoostPlay);

	
	mSpeechBalloon->Release();
	delete mSpeechBalloon;

	//�e�̉��
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
		Turn(SPEED_ROT_Y,AsoUtility::AXIS_Y);
	}
	//������
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		Turn(-SPEED_ROT_Y, AsoUtility::AXIS_Y);
	}
	//�����
	if (CheckHitKey(KEY_INPUT_UP))
	{
		Turn(-SPEED_ROT_X, AsoUtility::AXIS_X);
	}
	//������
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		Turn(SPEED_ROT_X, AsoUtility::AXIS_X);
	}

	mExplosion->Release();

}

void PlayerShip::Turn(double deg, VECTOR axis)
{
	//�f�O���[�p�����W�A���p�ɕϊ�
	float rad = AsoUtility::Deg2RadF(deg);
	//�w�莲�iAXIS�j�ɑ΂����]�ʁirad�j����������]�iQuaternion�j�����
	Quaternion axisQ= Quaternion::AngleAxis(deg, axis);
	//���@�̉�]�ɁA���ō쐬������]��������
	mTransform_.quaRot = mTransform_.quaRot.Mult(axisQ);
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
		mEfectBoostPlay = PlayEffekseer3DEffect(mEfectBoost);
		//�傫��
		float SCALE = 10.0f;
		SetScalePlayingEffekseer3DEffect(mEfectBoostPlay, SCALE, SCALE, SCALE);


	}

	if (mStepDelayBoost<TIME_DELAY_BOOST&& mEfectBoostPlay>0)
	{
		//�G�t�F�N�g����
		SyncBoostEffect();
	}

	if (mSpeedBoost>0.0f)
	{
		mSpeedBoost -= (SUB_SPEED_BOOST * SceneManager::GetInstance().GetDeltaTime());
		if (mSpeedBoost < 0.0f)
		{
			mSpeedBoost = 0.0f;
		}
	}
}

void PlayerShip::ProcessShot(void)
{
	mStepDelayShot -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepDelayShot < 0.0f)
	{
		mStepDelayShot = 0.0f;
	}
	//�L�[�`�F�b�N
	if (CheckHitKey(KEY_INPUT_N) && mStepDelayShot <= 0.0f)
	{
		//�f�B���C���Ԃ��Z�b�g
		mStepDelayShot = TIME_DELAY_SHOT;

		//�e�𐶐�
		CreateShot();

	}
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
	return mState == STATE::DESTROY;
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

	//��ԕω��i�J�ځj���̏�������

	mState = state;
	switch (mState)
	{
	case PlayerShip::STATE::RUN:
		break;
	case PlayerShip::STATE::DESTROY:
	{
		//�����G�t�F�N�g�̐���
		mExplosion->Create(mTransform_.pos, nullptr);
		//Effekseer�̃G�t�F�N�g����
		mExplosion3DPlay = PlayEffekseer3DEffect(mExplosion3D);
		VECTOR pos = mTransform_.pos;
		pos = VAdd(pos, VScale(mTransform_.GetForward(), 80.0f));
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

void PlayerShip::SyncJetEffect(void)
{

	VECTOR pos;
	const float Z = -15.0f;
	const float TY = 8.0f;
	const float DY = -5.0f;
	//�N�H�[�^�j�I������I�C���[�p�ϕϊ�
	const VECTOR angles = Quaternion::ToEuler(mTransform_.quaRot);


	//����������W�����
	//VECTOR localPos = VScale(mTransform.GetBack(), 15.0f);
	//pos = VAdd(mTransform.pos, localPos);
	//SetPosPlayingEffekseer3DEffect(mEfectJetPlayM, pos.x, pos.y, pos.z);

	//VECTOR localTOPPos = VScale(mTransform.GetBack(), 15.0f);
	//localTOPPos = VAdd(localTOPPos,VScale(mTransform.GetUp(),8.0f));
	//pos = VAdd(mTransform.pos, localPos);
	//SetPosPlayingEffekseer3DEffect(mEfectJetPlayT, pos.x, pos.y, pos.z);


		//���΍��W����]������
#pragma region �^�񒆕���
	//���΍��W�����
	VECTOR localMPos = { 0.0f,0.0f,Z };
	//���΍��W����]������
	localMPos = mTransform_.quaRot.PosAxis(localMPos);
	//���΍��W���烏�[���h���W�ɕϊ�����
	pos = VAdd(mTransform_.pos, localMPos);
	//�ʒu�ݒ�
	SetPosPlayingEffekseer3DEffect(mEfectJetPlayM, pos.x, pos.y, pos.z);
#pragma endregion
#pragma region �㕬��
	//���΍��W�����
	VECTOR localTPos = { 0.0f,8.0f,Z };
	//���΍��W����]������
	localTPos = mTransform_.quaRot.PosAxis(localTPos);
	//���΍��W���烏�[���h���W�ɕϊ�����
	pos = VAdd(mTransform_.pos, localTPos);
	//�ʒu�ݒ�
	SetPosPlayingEffekseer3DEffect(mEfectJetPlayT, pos.x, pos.y, pos.z);
#pragma endregion
#pragma region ������
	//���΍��W�����
	VECTOR localDPos = { 0.0f,-8.0f,Z };
	//���΍��W����]������
	localDPos = mTransform_.quaRot.PosAxis(localDPos);
	//���΍��W���烏�[���h���W�ɕϊ�����
	pos = VAdd(mTransform_.pos, localDPos);
	//�ʒu�ݒ�
	SetPosPlayingEffekseer3DEffect(mEfectJetPlayD, pos.x, pos.y, pos.z);
#pragma endregion
	

	//��]
	SetRotationPlayingEffekseer3DEffect(mEfectJetPlayT, angles.x, angles.y, angles.z);
	SetRotationPlayingEffekseer3DEffect(mEfectJetPlayM, angles.x, angles.y, angles.z);
	SetRotationPlayingEffekseer3DEffect(mEfectJetPlayD, angles.x, angles.y, angles.z);


}

void PlayerShip::SyncBoostEffect(void)
{
	int Z = 300.0f;
	VECTOR pos;
	//�I�C���[�ɕϊ�
	const VECTOR angles = Quaternion::ToEuler(mTransform_.quaRot);
	//��]
	SetRotationPlayingEffekseer3DEffect(mEfectBoostPlay, angles.x, angles.y, angles.z);

	//���΍��W�����
	VECTOR localPos = { 0.0f,0.0f,Z };
	//���΍��W����]������
	localPos = mTransform_.quaRot.PosAxis(localPos);
	//���΍��W���烏�[���h���W�ɕϊ�����
	pos = VAdd(mTransform_.pos, localPos);

	//�ʒu
	SetPosPlayingEffekseer3DEffect(mEfectBoostPlay, pos.x, pos.y, pos.z);
	//�傫��
	//SetScalePlayingEffekseer3DEffect(mEfectBoostPlay, 10.0f, 10.0f, 10.0f);
	
}

void PlayerShip::CreateShot(void)
{
	
	bool isCreate = false;
	for (auto s : mShots)
	{
		if (!s->IsAlive())
		{
			//�������Ă��Ȃ�������
			//�C���X�^���X���g���܂킷
			s->Create(mTransform_.pos, mTransform_.GetForward());
			isCreate = true;
		}
	}

	if (!isCreate)
	{
		//�C���X�^���X��V�������
		auto newShot = new PlayerShot(&mTransform_);
		newShot->Create(mTransform_.pos, mTransform_.GetForward());

		//�ϒ��z��ɒǉ�
		mShots.emplace_back(newShot);
	}
}
