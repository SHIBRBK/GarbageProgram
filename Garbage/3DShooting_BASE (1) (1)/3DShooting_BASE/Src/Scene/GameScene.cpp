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

	//�J�������[�h �t���[
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);
	//�J�������[�h
	//SceneManager::GetInstance().GetCamera()->SetFollowTarget(&mPlayerShip->GetTransform());
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);
	//�΂˕t��
	SceneManager::GetInstance().GetCamera()->SetFollowTarget(
		&mPlayerShip->GetTransform());
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW_SPRING);

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
	mSpaceDome->Update();
	mStage->Update();
	mPlayerShip->Update();
	mRockManager->Update();

	if (mPlayerShip->IsDestroy())
	{
		//���@���j�󂳂�Ă���
		mStepShipDestroy += SceneManager::GetInstance().GetDeltaTime();
		if (mStepShipDestroy>TIME_RESTART)
		{
			//�V�[���J��
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true);
		}
	}
	else
	{
		//���@�ƃ_���W�����̓����蔻��
		auto info = MV1CollCheck_Sphere(
			mStage->GetModelDungeon(), -1,
			mPlayerShip->GetTransform().pos, PlayerShip::COLLISION_RADIUS);
		if (info.HitNum > 0)
		{
			//�Փ˂��Ă���̂Ŏ��@��j��
			mPlayerShip->Destroy();

		}
		//�����蔻��p�̌��ʏ��̒��œ��I�Ƀ������m�ۂ��Ă���悤�Ȃ̂ŁA
		//�g�����������A�K���������̔j���E��n�������邱��
		MV1CollResultPolyDimTerminate(info);
	}

	//���K�F�{�X��͂̎���5000�̋����ɓ�������A
	//�C�x���g�V�[���ɑJ�ڂ���
	auto bossPos = mStage->GetPosCargoShip();
	auto playerPos = mPlayerShip->GetTransform().pos;
	if (AsoUtility::SqrMagnitude(bossPos,playerPos)<5000*5000)
	{
		//�V�[���J��
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::EVENT, true);
	}

	//�e�̍X�V
	auto shots = mPlayerShip->GetShots();
	for ( auto s : shots)
	{
		s->Update();
	}

}

void GameScene::Draw(void)
{
	//�w�i�E�s�����̂���
	mSpaceDome->Draw();
	//�I�u�W�F�N�g�i�����E���������j
	mStage->Draw();
	mRockManager->Draw();
	mPlayerShip->Draw();
	//�e�̕`��
	auto shots = mPlayerShip->GetShots();
	for (auto s : shots)
	{
		s->Draw();
	}
	//�X�N���[���G�t�F�N�g

	//UI�i�QD�j
	mPlayerShip->GetSpeechBalloon()->Draw();
	//�X�N���[���G�t�F�N�g�iUI���݁j



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
