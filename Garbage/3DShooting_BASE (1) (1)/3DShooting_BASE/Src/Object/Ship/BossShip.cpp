#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/Camera.h"
#include "../../Utility/AsoUtility.h"
#include "../../Object/Ship/Turret.h"
#include "../Common/SpriteAnimator.h"
#include"EventShot.h"
#include "BossShip.h"


BossShip::BossShip(void)
{
}

BossShip::~BossShip(void)
{
}

bool BossShip::IsAlive(void) const
{
	return mState == STATE::BATTLE;
}

std::vector<Turret*> BossShip::GetTurrets(void) const
{
	return mTurrets;
}

bool BossShip::IsDestroy(void) const
{
	return mState==STATE::DESTROY;
}

bool BossShip::IsEnd(void) const
{
	return mState==STATE::END;
}

void BossShip::Init(void)
{
	//�V�[���J�ڂ̑ҋ@���ԏ�����
	mStepEvent = 0.0f;

	//���f������̊�{���
	mTransform.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::BOSS_SHIP));
	float scale = 2.0f;
	mTransform.scl = { scale,scale,scale };
	mTransform.pos = { 0.0f,0.0f,4000.0f };
	mTransform.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(20.0f),
		AsoUtility::Deg2RadF(210.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	mTransform.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	//���f������̊�{�����X�V
	mTransform.Update();



	//// �O��
	MakeTurret(
		{ 4.5f, 5.5f, 7.8f },
		{ 0.0f, 0.0f, AsoUtility::Deg2RadF(-18.0f) });
	MakeTurret(
		{ -4.5f, 5.5f, 7.8f },
		{ 0.0f, 0.0f, AsoUtility::Deg2RadF(18.0f) });

	// ��
	MakeTurret(
		{ 4.5f, 5.5f, 0.0f },
		{ AsoUtility::Deg2RadF(20.0f), AsoUtility::Deg2RadF(90.0f), 0.0f });
	MakeTurret(
		{ -4.5f, 5.5f, 0.0f },
		{ AsoUtility::Deg2RadF(20.0f), AsoUtility::Deg2RadF(-90.0f), 0.0f });

	// ���
	MakeTurret(
		{ 3.5f, 5.0f, -17.8f },
		{ 0.0f, AsoUtility::Deg2RadF(180.0f), AsoUtility::Deg2RadF(18.0f) });
	MakeTurret(
		{ -3.5f, 5.0f, -17.8f },
		{ 0.0f, AsoUtility::Deg2RadF(180.0f), AsoUtility::Deg2RadF(-18.0f) });


	//�^���b�g�̃[�����c�t���O
	mIsAllDestroyTurrets = false;


	//�V�[���ɂ���ď�Ԃ�ς���
	SceneManager::SCENE_ID sceneId =
		SceneManager::GetInstance().GetmSceneID();
	if (sceneId ==SceneManager::SCENE_ID::EVENT)
	{
		ChangeState(STATE::EVENT);
	}
	else
	{
		ChangeState(STATE::BATTLE);
		//�����蔻��
		int ret = MV1SetupCollInfo(mTransform.modelId, -1);

	}

	//ChangeState(STATE::EVENT);

	
}

void BossShip::Update(void)
{
	switch (mState)
	{
	case BossShip::STATE::EVENT:
	{
		//�O�������ɂ������ړ�
		mTransform.pos=
			VAdd(mTransform.pos,VScale(mTransform.GetForward(), 5.0f));
	//�C�x���g�p�̒e
		mEventShot->Update();
		if (!mEventShot -> IsAlive())
		{
			auto& ins = SceneManager::GetInstance();
			mStepEvent -= ins.GetDeltaTime();
			if (mStepEvent < 0.0f)
			{
				//�o�g���V�[���֑J��
				ins.ChangeScene(SceneManager::SCENE_ID::BATTLE, true);
				return;
			}
		}
		mTransform.Update();
		//�^���b�g
		UpdateTurret();
		

		break;
	}
	case BossShip::STATE::BATTLE:
	{
		//�{�X�̕`��@�ړ�
		mTransform.pos =
			VAdd(mTransform.pos,
				VScale(mTransform.GetForward(), SPEED_MOVE)
			);
		float rad = AsoUtility::Deg2RadF(3.0f);
		float speed = rad * SceneManager::GetInstance().GetDeltaTime();
		//Y���i��]�ʂ�1�b������R�x�j�̉�]��������
		Quaternion axisQ = Quaternion::AngleAxis(speed, AsoUtility::AXIS_Y);
		//�쐬������]�����@�ɍ�������
		mTransform.quaRot = mTransform.quaRot.Mult(axisQ);

		mTransform.Update();

		int ret = MV1RefreshCollInfo(mTransform.modelId, -1);

		//�^���b�g
		UpdateTurret();

		//--------
		if (mIsAllDestroyTurrets)
		{
			ChangeState(STATE::DESTROY);
			return;
		}
	}
		break;

	case BossShip::STATE::DESTROY:
	{
		auto& ins = SceneManager::GetInstance();

		mStepFade -= ins.GetDeltaTime();
		if (mStepFade < 0.0f)
		{
			ChangeState(STATE::END);
			return;
		}
		mStepExplosion -= ins.GetDeltaTime();
		if (mStepExplosion < 0.0f)
		{
			CreateRandomExplosion();
			mStepExplosion = TIME_EXPLOSION;
		}

		for (auto e : mExplosions)
		{
			e->Update();
		}

	}

		break;
	}

}

void BossShip::UpdateTurret(void)
{
	int alived = 0;
	for (auto t : mTurrets)
	{
		t->Update();
		if (t->IsAlive())
		{
			alived++;
		}
	}
	if (alived == 0)
	{
		mIsAllDestroyTurrets = true;
	}
}

void BossShip::Draw(void)
{
	switch (mState)
	{
	case BossShip::STATE::EVENT:
		MV1DrawModel(mTransform.modelId);
		mEventShot->Draw();
		DrawTurret();
		break;
	case BossShip::STATE::BATTLE:
		MV1DrawModel(mTransform.modelId);
		DrawTurret();
		break;
	case BossShip::STATE::DESTROY:
		{
		float diff = TIME_FADE - mStepFade;
		auto color = AsoUtility::Lerp(FADE_FROM, FADE_TO, (diff / TIME_FADE));

		int num = MV1GetMaterialNum(mTransform.modelId);
		for (int i = 0; i < num; i++)
		{
			MV1SetMaterialDifColor(mTransform.modelId, i, color);
		}

		MV1DrawModel(mTransform.modelId);

		ZSortExplosion();

		for (auto e : mExplosions)
			{
				e->Draw();
			}

		}
		break;
	case BossShip::STATE::END:

		ZSortExplosion();

		for (auto e : mExplosions)
		{
			e->Draw();
		}


		break;
	}
}

void BossShip::DrawTurret(void)
{
	for (auto t : mTurrets)
	{
		t->Draw();
	}
}

void BossShip::Release(void)
{
	if (mEventShot != nullptr)
	{
		mEventShot->Release();
		delete mEventShot;
	}

	for (auto t : mTurrets)
	{
		t->Release();
		delete t;
	}
	mTurrets.clear();


	for (auto e : mExplosions)
	{
		e->Release();
		delete e;
	}
	mExplosions.clear();



}

const Transform& BossShip::GetTransform(void) const
{
	return mTransform;
}



void BossShip::MakeTurret(VECTOR localPos, VECTOR localAddAxis)
{
	Turret* turret = new Turret(&mTransform, localPos, localAddAxis);
	turret->Init();

	mTurrets.emplace_back(turret);
}

void BossShip::CreateRandomExplosion(void)
{
	for (int i = 0; i < 5; i++)
	{
		float x = static_cast<float>(GetRand(360));
		float y = static_cast<float>(GetRand(360));
		float z = static_cast<float>(GetRand(360));

		x = AsoUtility::Deg2RadF(x);
		y = AsoUtility::Deg2RadF(y);
		z = AsoUtility::Deg2RadF(z);


		Quaternion tmpQ = Quaternion::Euler(x, y, z);
		//�����_���ȕ���(x,y,z)
		VECTOR dir = tmpQ.PosAxis(AsoUtility::DIR_F);


		VECTOR pos = { 0.0f,0.0f,0.0f };
		pos = VAdd(pos, VScale({ dir.x,0.0f,0.0f }, EX_RADIUS_X));
		pos = VAdd(pos, VScale({ 0.0f,dir.y,0.0f }, EX_RADIUS_Y));
		pos = VAdd(pos, VScale({ 0.0f,0.0f,dir.z }, EX_RADIUS_Z));


		//�e�̉�]�ɍ��킹�āA�ʒu����]

		VECTOR rotLocalPos = mTransform.quaRot.PosAxis(pos);

		//���΍��W�����΍��W�ɕϊ�
		pos = VAdd(mTransform.pos, rotLocalPos);


		SpriteAnimator* ex = new SpriteAnimator(
			ResourceManager::SRC::SHIP_EXPLOSION, 200, 8.0f);
		ex->Create(pos, nullptr);
		mExplosions.push_back(ex);
	}
}

void BossShip::ZSortExplosion(void)
{
}

void BossShip::ChangeState(STATE state)
{
	mState = state;

	//��ԕύX���̏���������
	switch (mState)
	{
	case BossShip::STATE::EVENT:
	{

		mStepEvent = TIME_EVENT;

		mEventShot = new EventShot(&mTransform);

		//�e���J�����Ɍ������đł�

		//�e��ł����i�P�ʃx�N�g���j
		auto cPos = SceneManager::GetInstance().GetCamera()->GetPos();
		auto bPos = mTransform.pos;
		VECTOR dir = VNorm(VSub(cPos, bPos));
		mEventShot->Create(mTransform.pos, dir);

	}
	break;
	case BossShip::STATE::BATTLE:
		break;

	case BossShip::STATE::DESTROY:
		mStepFade = TIME_FADE;
		mStepExplosion = 0.0f;
		break;
	}


}