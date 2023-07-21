#include <DxLib.h>
#include "../Common/DxCushion.h"
#include <memory>
#include "TitleScene.h"
#include "GameScene.h"
#include "../Manager/Camera.h"
#include "../Manager/SceneManager.h"
TitleScene::TitleScene() :BaseScene()
{

}

TitleScene::~TitleScene()
{
}

void TitleScene::Init(void)
{

	//ƒJƒƒ‰ƒ‚[ƒh
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);

}

void TitleScene::Update()
{

	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME,true);
	}


}

void TitleScene::Draw(void)
{

	DrawBox(0, 0, 200, 200, GetColor(255, 255, 255), true);

}
