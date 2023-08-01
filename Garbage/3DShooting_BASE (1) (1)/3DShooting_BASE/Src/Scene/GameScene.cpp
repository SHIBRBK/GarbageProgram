#include <cmath>
#include <DxLib.h>
#include"../Object/Stage.h"
#include"../Object/SpaceDome.h"
#include"../Object/PlayerShip.h"
#include"../Object/Ship/PlayerShot.h"
#include"../Object/Common/SpeechBalloon.h"
#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/RockManager.h"
#include "../Utility/AsoUtility.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	

}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{

	mPlayerShip = new PlayerShip();
	mPlayerShip->Init();

	mSpaceDome = new SpaceDome(&mPlayerShip->GetTransform());
	mSpaceDome->Init();

	mStage = new Stage();
	mStage->Init();

	//カメラモード フリー
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);
	//カメラモード
	//SceneManager::GetInstance().GetCamera()->SetFollowTarget(&mPlayerShip->GetTransform());
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);
	//ばね付き
	SceneManager::GetInstance().GetCamera()->SetFollowTarget(
		&mPlayerShip->GetTransform());
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW_SPRING);

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
	mSpaceDome->Update();
	mStage->Update();
	mPlayerShip->Update();
	mRockManager->Update();

	if (mPlayerShip->IsDestroy())
	{
		//自機が破壊されている
		mStepShipDestroy += SceneManager::GetInstance().GetDeltaTime();
		if (mStepShipDestroy>TIME_RESTART)
		{
			//シーン遷移
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true);
		}
	}
	else
	{
		//自機とダンジョンの当たり判定
		auto info = MV1CollCheck_Sphere(
			mStage->GetModelDungeon(), -1,
			mPlayerShip->GetTransform().pos, PlayerShip::COLLISION_RADIUS);
		if (info.HitNum > 0)
		{
			//衝突しているので自機を破壊
			mPlayerShip->Destroy();

		}
		//当たり判定用の結果情報の中で動的にメモリ確保しているようなので、
		//使いおわったら、必ずメモリの破棄・後始末をすること
		MV1CollResultPolyDimTerminate(info);
	}

	//演習：ボス戦艦の周囲5000の距離に入ったら、
	//イベントシーンに遷移する
	auto bossPos = mStage->GetPosCargoShip();
	auto playerPos = mPlayerShip->GetTransform().pos;
	if (AsoUtility::SqrMagnitude(bossPos,playerPos)<5000*5000)
	{
		//シーン遷移
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::EVENT, true);
	}

	//弾の更新
	auto shots = mPlayerShip->GetShots();
	for ( auto s : shots)
	{
		s->Update();
	}

}

void GameScene::Draw(void)
{
	//背景・不透明のもの
	mSpaceDome->Draw();
	//オブジェクト（透明・半透明等）
	mStage->Draw();
	mRockManager->Draw();
	mPlayerShip->Draw();
	//弾の描画
	auto shots = mPlayerShip->GetShots();
	for (auto s : shots)
	{
		s->Draw();
	}
	//スクリーンエフェクト

	//UI（２D）
	mPlayerShip->GetSpeechBalloon()->Draw();
	//スクリーンエフェクト（UI混み）



}

void GameScene::Release(void)
{
	mSpaceDome->Release();
	delete mSpaceDome;
	mStage->Release();
	delete mStage;
	mPlayerShip->Release();
	delete mPlayerShip;
	mRockManager->Release();
	delete mRockManager;
}
