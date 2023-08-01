#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include"../Object/SpaceDome.h"
#include"../Object/Ship/BossShip.h"
#include "EventScene.h"

EventScene::EventScene(void)
{
}

EventScene::~EventScene(void)
{
}

void EventScene::Init(void)
{

	SceneManager::GetInstance().GetCamera()->ChangeMode(
	Camera::MODE::FIXED_POINT);

	mSpaceDome = new SpaceDome(nullptr);
	mSpaceDome->Init();
	mBossShip = new BossShip();
	mBossShip->Init();
}

void EventScene::Update(void)
{
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::BATTLE, true);
	}

	mSpaceDome->Update();
	mBossShip->Update();
}

void EventScene::Draw(void)
{
	mSpaceDome->Draw();
	mBossShip->Draw();
}

void EventScene::Release(void)
{
	mSpaceDome->Release();
	delete mSpaceDome;
	mBossShip->Release();
	delete mBossShip;
}
