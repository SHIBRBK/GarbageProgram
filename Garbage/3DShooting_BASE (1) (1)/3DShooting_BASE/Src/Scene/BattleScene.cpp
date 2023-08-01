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

	//カメラモード
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

	//自機とボスとの当たり判定
	if (mPlayerShip->IsDestroy())
	{
		//自機が破壊されている
		mStepShipDestroy += SceneManager::GetInstance().GetDeltaTime();
		if (mStepShipDestroy > TIME_RESTART)
		{
			//シーン遷移
			SceneManager::GetInstance().ChangeScene(
				SceneManager::SCENE_ID::BATTLE, true);
		}
	}
	else
	{
		//ボスの生存判定
		if (mBossShip->IsAlive())
		{

			//自機とダンジョンの当たり判定
			auto info = MV1CollCheck_Sphere(
				mBossShip->GetTransform().modelId, -1,
				mPlayerShip->GetTransform().pos, PlayerShip::COLLISION_RADIUS);
			if (info.HitNum > 0)
			{
				//衝突しているので自機を破壊
				mPlayerShip->Destroy();

			}
			//当たり判定用の結果情報の中で動的にメモリ確保しているようなので、
			//使いおわったら、必ずメモリの破棄・後始末をすること
			MV1CollResultPolyDimTerminate(info);


			//自機の弾とタレットの衝突判定
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

	//弾の更新
	auto shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{
		s->Update();

		if (!s->IsAlive())
		{
			//生存していない
			continue;
		}

		//ボスとの当たり判定
		if (mBossShip->IsAlive())
		{
			//弾とボスの当たり判定
			auto info = MV1CollCheck_Sphere(
				mBossShip->GetTransform().modelId, -1,
				s->GetPos(), s->GetCollisionRadius());

			if (info.HitNum > 0)
			{
				//衝突していたら、弾を破壊する
				s->CreateExplosion(&mBossShip->GetTransform());
				continue;
			}

			//当たり判定用の結果情報の中で動的にメモリ確保しているようなので、
			//使いおわったら、必ずメモリの破棄・後始末をすること
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

	//敵の玉と自機の当たり判定

}

void BattleScene::Draw(void)
{
	//不透明（背景）
	mSpaceDome->Draw();
	mRockManager->Draw();
	//不透明（オブジェクト）
	mBossShip->Draw();
	mPlayerShip->Draw();
	//弾の描画
	auto shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{
		s->Draw();
	}
	
	//透明
	
	//mPlayerShip->GetSpeechBalloon()->Draw();
	if (mBossShip->IsDestroy()||mBossShip->IsEnd())
	{
		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2,
			1.0f, 0.0, mImgEndLogo, true);
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
