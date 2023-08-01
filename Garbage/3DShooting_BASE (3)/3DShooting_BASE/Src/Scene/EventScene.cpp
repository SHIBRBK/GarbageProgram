#include<DxLib.h>
#include"../Application.h"
#include"../Manager/InputManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SceneManager.h"
#include"../Manager/Camera.h"
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
	SceneManager::GetInstance().GetCamera()->changeMode(
		Camera::MODE::FIXED
	);

	//�w�i(�X�J�C�h�[��)
	mSpaceDome = new SpaceDome(nullptr);
	mSpaceDome ->Init();
	//�{�X���
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
	//�w�i(�s����)
	mSpaceDome->Draw();

	//�I�u�W�F�N�g
	mBossShip->Draw();

}

void EventScene::DrawLogo(void)
{
}

void EventScene::Release(void)
{
	mSpaceDome->Release();
	delete mSpaceDome;
	mBossShip->Release();
	delete mBossShip;
}
