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




	//カメラモードを追従型にする
	auto& ins = SceneManager::GetInstance();
	ins.GetCamera()->SetFollowTarget(&trans);

	mStepShipDestroy = 0.0f;


	//ゲームクリアロゴ
	mImageEndLog = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::END_LOGO).mHandleId;


	//ボスの破壊演出時間
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
		//ボスの生存判定
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
			//生存していない
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
					//衝突していたら、弾を破壊する
					s->CreateExplosion(&mBossShip->GetTransform());
					continue;
				}


			//当たり判定の結果情報の中に動的にメモリ確保しているようなので、
			//使い終わったら、必ずメモリの破棄、後始末をすること
			MV1CollResultPolyDimTerminate(info);
		}
	

	//自機の弾とタレットの衝突判定(球体と球体)
	auto turrets = mBossShip->GetTurrets();
	for (auto t : turrets)
	{
		if (!t->IsAlive())
		{
			continue;
		}

		//AsoUtillity::IsHitSpheres(衝突判定(球体と球体))

		AsoUtility::IsHitSpheres(
			s->GetPos(),s->GetCollisionRadius(),
			t->GetPos(), Turret::COLLISON_RADIUS);

		if (false)
			{
			//タレットにダメージ
			t->Damaged();

			//弾を破壊(貫通させない)
			s->CreateExplosion(nullptr);
			break;


			}
		}
	}
	//ボスの破壊判定
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

	//不透明(背景)
	mSpaceDome->Draw();
	mRockManager->Draw();




	// /不透明(オブジェクト)
	mBossShip->Draw();
	mPlayerShip->Draw();


	//弾を描画
	auto shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{
		s->Draw();
	}



	//UI(2D)
	mPlayerShip->GetSpeechBalloon()->Draw();

	if(mBossShip->IsDestroy()||mBossShip->IsEnd())
	{
	//クリアロゴ
		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2,
			1.0f, 0.0f, mImageEndLog, true);
	
	}

	//スクリーンエフェクト
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
