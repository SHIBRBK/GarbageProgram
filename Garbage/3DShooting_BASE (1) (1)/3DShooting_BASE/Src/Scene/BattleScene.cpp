#include"../Application.h"
#include"../Manager/SceneManager.h"
#include"../Manager/Camera.h"
#include "../Manager/RockManager.h"
#include "../Manager/ResourceManager.h"
#include"../Object/SpaceDome.h"
#include "../Object/PlayerShip.h"
#include "../Utility/AsoUtility.h"
#include"../Object/Ship/BossShip.h"
#include "../Object/Ship/PlayerShot.h"
#include "../Object/Ship/Turret.h"
#include "../Object/Ship/TurretShot2.h"
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
	mPlayerShip->Init();

	const auto& trans = mPlayerShip->GetTransform();

	mRockManager = new RockManager(&trans);
	mRockManager -> Init();

	mSpaceDome = new SpaceDome(&trans);
	mSpaceDome->Init();

	mBossShip = new BossShip();
	mBossShip->Init();

	//�J�������[�h
	auto& ins = SceneManager::GetInstance();
	ins.GetCamera()->SetFollowTarget(&trans);
	ins.GetCamera()->ChangeMode(Camera::MODE::FOLLOW_SPRING);


	mStepShipDestroy = 0.0f;

	mImgEndLogo = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::END_LOGO).mHandleId;

	mStepBossDestroy = TIME_RESTART_BOSS_DESTROY;
}

void BattleScene::Update(void)
{
	mBossShip->Update();
	mPlayerShip->Update();
	mRockManager->Update();
	mSpaceDome->Update();

	//���@�ƃ{�X�Ƃ̓����蔻��
	if (mPlayerShip->IsDestroy())
	{
		//���@���j�󂳂�Ă���
		mStepShipDestroy += SceneManager::GetInstance().GetDeltaTime();
		if (mStepShipDestroy > TIME_RESTART)
		{
			//�V�[���J��
			SceneManager::GetInstance().ChangeScene(
				SceneManager::SCENE_ID::BATTLE, true);
		}
	}
	else
	{
		//�{�X�̐�������
		if (mBossShip->IsAlive())
		{

			//���@�ƃ_���W�����̓����蔻��
			auto info = MV1CollCheck_Sphere(
				mBossShip->GetTransform().modelId, -1,
				mPlayerShip->GetTransform().pos, PlayerShip::COLLISION_RADIUS);
			if (info.HitNum > 0)
			{
				//�Փ˂��Ă���̂Ŏ��@��j��
				mPlayerShip->Destroy();

			}
			//�����蔻��p�̌��ʏ��̒��œ��I�Ƀ������m�ۂ��Ă���悤�Ȃ̂ŁA
			//�g�����������A�K���������̔j���E��n�������邱��
			MV1CollResultPolyDimTerminate(info);


			//���@�̒e�ƃ^���b�g�̏Փ˔���
			auto turrets = mBossShip->GetTurrets();
			for (auto t : turrets)
			{
				auto ts = t->GetShots();
				for (auto s : ts)
				{

					if (!s->IsAlive())
					{
						continue;
					}


					if (AsoUtility::IsHitSpheres(
						mPlayerShip->GetTransform().pos,
						PlayerShip::COLLISION_RADIUS,
						s->GetPos(),
						s->GetCollisionRadius()))
					{
						mPlayerShip->Destroy();
					}
				}
			}
		}
	}

	//�e�̍X�V
	auto shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{
		s->Update();

		if (!s->IsAlive())
		{
			//�������Ă��Ȃ�
			continue;
		}

		//�{�X�Ƃ̓����蔻��
		if (mBossShip->IsAlive())
		{
			//�e�ƃ{�X�̓����蔻��
			auto info = MV1CollCheck_Sphere(
				mBossShip->GetTransform().modelId, -1,
				s->GetPos(), s->GetCollisionRadius());

			if (info.HitNum > 0)
			{
				//�Փ˂��Ă�����A�e��j�󂷂�
				s->CreateExplosion(&mBossShip->GetTransform());
				continue;
			}

			//�����蔻��p�̌��ʏ��̒��œ��I�Ƀ������m�ۂ��Ă���悤�Ȃ̂ŁA
			//�g�����������A�K���������̔j���E��n�������邱��
			MV1CollResultPolyDimTerminate(info);
		}

		auto turrets = mBossShip->GetTurrets();
		for(auto t : turrets)
		{
			if (!t->IsAlive())
			{
				continue;
			}


			if (AsoUtility::IsHitSpheres(
				s->GetPos(), s->GetCollisionRadius(),
				t->GetPos(), Turret::COLLISION_RADIUS))
			{
				t->Damage();

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

	//�G�̋ʂƎ��@�̓����蔻��

}

void BattleScene::Draw(void)
{
	//�s�����i�w�i�j
	mSpaceDome->Draw();
	mRockManager->Draw();
	//�s�����i�I�u�W�F�N�g�j
	mBossShip->Draw();
	mPlayerShip->Draw();
	//�e�̕`��
	auto shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{
		s->Draw();
	}
	
	//����
	
	//mPlayerShip->GetSpeechBalloon()->Draw();
	if (mBossShip->IsDestroy()||mBossShip->IsEnd())
	{
		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2,
			1.0f, 0.0, mImgEndLogo, true);
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
