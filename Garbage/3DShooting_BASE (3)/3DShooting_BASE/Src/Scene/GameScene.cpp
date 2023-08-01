#include <cmath>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
#include"../Object/Stage.h"
#include"../Object/Ship/PlayerShip.h"
#include"../Object/SpaceDome.h"
#include"../Object/RockManager.h"
#include"../Object/Ship/PlayerShot.h"
#include"../Object/Common/SpeechBalloon.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	mStage = nullptr;
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{

	mPlayerShip = new PlayerShip();
	mPlayerShip->Init();

	mSpaceDome = new SpaceDome(&mPlayerShip->GetTransform());//参照渡しとポインタ渡しでごちゃごちゃになってるからnplayerにアンパサンドで渡す
	mSpaceDome->Init();

	mStage = new Stage();
	mStage->Init();


	//カメラモード
	SceneManager::GetInstance().GetCamera()->SetFollowTarget(
		&mPlayerShip->GetTransform());


	SceneManager::GetInstance().GetCamera()->changeMode(
		Camera::MODE::FOLLOW_SPRING);

	//スペースデブリ
	mRockManager = new RockManager(&mPlayerShip->GetTransform());
	mRockManager->Init();

	mStepShipDestroy = 0.0f;
}

void GameScene::Update(void)
{


	// シーン遷移
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::EVENT, true);
	}




	//弾を更新
	auto shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{
		s->Update();
	}



	mPlayerShip->Update();
	mSpaceDome->Update();
	mStage->Update();
	mRockManager->Update();

	if (mPlayerShip->IsDestroy()) 
	{
		mStepShipDestroy += SceneManager::GetInstance().GetDeltaTime();
		if (mStepShipDestroy > TIME_RESTART)
		{
			SceneManager::GetInstance().ChangeScene(
				SceneManager::SCENE_ID::TITLE, true);
		}
	}
	else 
	{
		//自機とダンジョンの衝突判定
		auto info = MV1CollCheck_Sphere(
			mStage->GetModelDungeon(), -1,
			mPlayerShip->GetTransform().pos, PlayerShip::COLLISION_RADIUS);

		if (info.HitNum > 0)//イコールがないほうが早い。わかりやすさをとるなら>=とかでもい
		{
			//自機破壊
			mPlayerShip->Destroy();
			//遅延処理
			// 
			//衝突している
			SceneManager::GetInstance().ChangeScene(
				SceneManager::SCENE_ID::TITLE, true);
			//しばらくしてシーン遷移
		}
		//当たり判定用の結果情報の中で動的にメモリを確保しているようなので、
		//使い終わったら、必ずメモリの覇気、後始末をすること
		MV1CollResultPolyDimTerminate(info);
	}

	//演習：ボス戦艦の周囲5000の距離に入ったら、
	//イベントシーンに移行するように
	auto bossPos = mStage->GetPosBattleShip();
	auto playerPos = mPlayerShip->GetTransform().pos;
	if (AsoUtility::SqrMagnitude(bossPos,playerPos)<5000*5000)
	{
		//シーン遷移
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::EVENT, true);
	}
}

void GameScene::Draw(void)
{

	//背景・不透明なもの
	mSpaceDome->Draw();

	//オブジェクト(透明、半透明等)
	mStage->DrawGrid();
	mPlayerShip->Draw();
	mRockManager->Draw();

	//スクリーンショット

	//UI(2D)
	mPlayerShip->GetSpeechBalloon()->Draw();
	//

	//弾を描画
	auto shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{
		s->Draw();
	}

}

void GameScene::Release(void)
{


	mStage->Release();
	delete mStage;
	mSpaceDome->Release();
	delete mSpaceDome;
	mPlayerShip->Release();
	delete mPlayerShip;
	mRockManager->Release();
	delete mRockManager;
}


