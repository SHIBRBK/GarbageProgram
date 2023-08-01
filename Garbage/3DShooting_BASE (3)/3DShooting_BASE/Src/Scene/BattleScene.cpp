#include"../Application.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/Camera.h"
#include"../Utility/AsoUtility.h"
#include"../Object/RockManager.h"
#include"../Object/Ship/BossShip.h"
#include"../Object/Ship/PlayerShip.h"
#include"../Object/Ship/PlayerShot.h"
#include"../Object/SpaceDome.h"
#include"../Object/Common/SpeechBalloon.h"
#include"../Object/Ship/Turret.h"
//#include"../Object/BossShip.h"

#include "BattleScene.h"

BattleScene::BattleScene(void)
{
}

BattleScene::~BattleScene(void)
{
}

void BattleScene::Init(void)
{
	mPlayerShip = new PlayerShip();
	mPlayerShip ->Init();

	const auto& trans = mPlayerShip->GetTransform();
	


	mRockManager = new RockManager(&trans);
	mRockManager ->Init();

	mSpaceDome = new SpaceDome(&trans);
	mSpaceDome ->Init();

	mBossShip = new BossShip();
	mBossShip ->Init();




	//�J�������[�h��Ǐ]�^�ɂ���
	auto& ins = SceneManager::GetInstance();
	ins.GetCamera()->SetFollowTarget(&trans);

	mStepShipDestroy = 0.0f;


	//�Q�[���N���A���S
	mImageEndLog = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::END_LOGO).mHandleId;


	//�{�X�̔j�󉉏o����
	mStepBossDestroy = TIME_RESTART_BOSS_D;
}

void BattleScene::Update(void)
{
	mBossShip->Update();
	mPlayerShip->Update();
	mRockManager->Update();
	mSpaceDome->Update();

	if (mPlayerShip->IsDestroy())
	{
		mStepShipDestroy += SceneManager::GetInstance().GetDeltaTime();
		if (mStepShipDestroy > TIME_RESTART)
		{
			SceneManager::GetInstance().ChangeScene(
				SceneManager::SCENE_ID::BATTLE, true);
		}
	}
	else
	{
		//�{�X�̐�������
		if (mBossShip->IsAlive())
		{
			auto info = MV1CollCheck_Sphere(
				mBossShip->GetTransform().modelId, -1,
				mPlayerShip->GetTransform().pos, PlayerShip::COLLISION_RADIUS);
			if (info.HitNum > 0)
			{
				mPlayerShip->Destroy();
			}
			MV1CollResultPolyDimTerminate(info);
		}
	}

	auto shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{
		s->Update();

		if (!s->IsAlive())
		{
			//�������Ă��Ȃ�
			continue;
		}
		if (mBossShip->IsAlive())
		{

			auto info = MV1CollCheck_Sphere(
				mBossShip->GetTransform().modelId, -1,
				s->GetPos(), s->GetCollisionRadius());

			if (AsoUtility::IsHitSpheres)



				if (info.HitNum > 0)
				{
					//�Փ˂��Ă�����A�e��j�󂷂�
					s->CreateExplosion(&mBossShip->GetTransform());
					continue;
				}


			//�����蔻��̌��ʏ��̒��ɓ��I�Ƀ������m�ۂ��Ă���悤�Ȃ̂ŁA
			//�g���I�������A�K���������̔j���A��n�������邱��
			MV1CollResultPolyDimTerminate(info);
		}
	

	//���@�̒e�ƃ^���b�g�̏Փ˔���(���̂Ƌ���)
	auto turrets = mBossShip->GetTurrets();
	for (auto t : turrets)
	{
		if (!t->IsAlive())
		{
			continue;
		}

		//AsoUtillity::IsHitSpheres(�Փ˔���(���̂Ƌ���))

		AsoUtility::IsHitSpheres(
			s->GetPos(),s->GetCollisionRadius(),
			t->GetPos(), Turret::COLLISON_RADIUS);

		if (false)
			{
			//�^���b�g�Ƀ_���[�W
			t->Damaged();

			//�e��j��(�ђʂ����Ȃ�)
			s->CreateExplosion(nullptr);
			break;


			}
		}
	}
	//�{�X�̔j�󔻒�
	if (mBossShip->IsEnd())
	{
		mStepBossDestroy -= SceneManager::GetInstance().GetDeltaTime();
		if (mStepBossDestroy < 0.0)
		{
			SceneManager::GetInstance().ChangeScene(
				SceneManager::SCENE_ID::TITLE,true);
		}
	}
}

void BattleScene::Draw(void)
{

	//�s����(�w�i)
	mSpaceDome->Draw();
	mRockManager->Draw();




	// /�s����(�I�u�W�F�N�g)
	mBossShip->Draw();
	mPlayerShip->Draw();


	//�e��`��
	auto shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{
		s->Draw();
	}



	//UI(2D)
	mPlayerShip->GetSpeechBalloon()->Draw();

	if(mBossShip->IsDestroy()||mBossShip->IsEnd())
	{
	//�N���A���S
		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2,
			1.0f, 0.0f, mImageEndLog, true);
	
	}

	//�X�N���[���G�t�F�N�g
}

void BattleScene::Release(void)
{
	mBossShip->Release();
	delete mBossShip;
	mPlayerShip->Release();
	delete mPlayerShip;
	mRockManager->Release();
	delete mRockManager;
	mSpaceDome->Release();
	delete mSpaceDome;
}
