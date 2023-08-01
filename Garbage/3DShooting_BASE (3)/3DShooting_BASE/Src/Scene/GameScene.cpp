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

	mSpaceDome = new SpaceDome(&mPlayerShip->GetTransform());//�Q�Ɠn���ƃ|�C���^�n���ł����Ⴒ����ɂȂ��Ă邩��nplayer�ɃA���p�T���h�œn��
	mSpaceDome->Init();

	mStage = new Stage();
	mStage->Init();


	//�J�������[�h
	SceneManager::GetInstance().GetCamera()->SetFollowTarget(
		&mPlayerShip->GetTransform());


	SceneManager::GetInstance().GetCamera()->changeMode(
		Camera::MODE::FOLLOW_SPRING);

	//�X�y�[�X�f�u��
	mRockManager = new RockManager(&mPlayerShip->GetTransform());
	mRockManager->Init();

	mStepShipDestroy = 0.0f;
}

void GameScene::Update(void)
{


	// �V�[���J��
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::EVENT, true);
	}




	//�e���X�V
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
		//���@�ƃ_���W�����̏Փ˔���
		auto info = MV1CollCheck_Sphere(
			mStage->GetModelDungeon(), -1,
			mPlayerShip->GetTransform().pos, PlayerShip::COLLISION_RADIUS);

		if (info.HitNum > 0)//�C�R�[�����Ȃ��ق��������B�킩��₷�����Ƃ�Ȃ�>=�Ƃ��ł���
		{
			//���@�j��
			mPlayerShip->Destroy();
			//�x������
			// 
			//�Փ˂��Ă���
			SceneManager::GetInstance().ChangeScene(
				SceneManager::SCENE_ID::TITLE, true);
			//���΂炭���ăV�[���J��
		}
		//�����蔻��p�̌��ʏ��̒��œ��I�Ƀ��������m�ۂ��Ă���悤�Ȃ̂ŁA
		//�g���I�������A�K���������̔e�C�A��n�������邱��
		MV1CollResultPolyDimTerminate(info);
	}

	//���K�F�{�X��͂̎���5000�̋����ɓ�������A
	//�C�x���g�V�[���Ɉڍs����悤��
	auto bossPos = mStage->GetPosBattleShip();
	auto playerPos = mPlayerShip->GetTransform().pos;
	if (AsoUtility::SqrMagnitude(bossPos,playerPos)<5000*5000)
	{
		//�V�[���J��
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::EVENT, true);
	}
}

void GameScene::Draw(void)
{

	//�w�i�E�s�����Ȃ���
	mSpaceDome->Draw();

	//�I�u�W�F�N�g(�����A��������)
	mStage->DrawGrid();
	mPlayerShip->Draw();
	mRockManager->Draw();

	//�X�N���[���V���b�g

	//UI(2D)
	mPlayerShip->GetSpeechBalloon()->Draw();
	//

	//�e��`��
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


