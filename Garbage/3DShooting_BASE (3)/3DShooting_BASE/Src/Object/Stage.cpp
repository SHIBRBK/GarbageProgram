#include<DxLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Utility/AsoUtility.h"
#include "Stage.h"
Stage::Stage(void)
{
	
}

Stage::~Stage(void)
{

}

void Stage::Init()
{
	//mModelId_ = MV1LoadModel((Application::PATH_MODEL + "Stage.mv1").c_str());
	
	//MV1SetPosition(mModelId_, mPos);
	mModelDungeon = ResourceManager::GetInstance().
	LoadModelDuplicate(ResourceManager::SRC::DUNGEON
	);

	mPosDungeon = {14500.0f,-800.0f,6500.0f};

	MV1SetScale(mModelDungeon, AsoUtility::VECTOR_ONE);

	MV1SetRotationXYZ(mModelDungeon,{0.0f,180.0f,0.0f});

	MV1SetPosition(mModelDungeon, mPosDungeon);

	//戦艦
	mModelBattleShip = ResourceManager::GetInstance().
		LoadModelDuplicate(ResourceManager::SRC::BOSS_SHIP);
	mPosBattleShip = { 14500.0f,-800.0f,6500.0f };
	MV1SetScale(mModelBattleShip, AsoUtility::VECTOR_ONE);
	MV1SetRotationXYZ(mModelBattleShip, { 0.0f,180.0f,0.0f });
	MV1SetPosition(mModelBattleShip, mPosBattleShip);

	//当たり判定(コライダ)作成	unityでお世話になっております。
	int ret;
	ret = MV1SetupCollInfo(mModelDungeon, -1);
	ret = MV1SetupCollInfo(mModelBattleShip, -1);

}

void Stage::Draw()
{
	//3Dモデルの描画
	MV1DrawModel(mModelDungeon);
	MV1DrawModel(mModelBattleShip);

	//XYZ線の表示
	DrawGrid();
	DrawSphere3D(mPosBattleShip, 5000.0f, 20,
		0xff0000, 0xff0000, false);

}

void Stage::DrawGrid()
{
	constexpr float LEN = 3000.0f;


	constexpr float TERM = 100.0f;

	const int NUM = AsoUtility::Round(LEN / TERM);

	//XYZ基本軸
	float f;
	VECTOR s;
	VECTOR e;
	for (int i = -NUM; i < NUM; i++)
	{
		f = static_cast<float>(i);
		
		//X軸
		s = VGet(-LEN, 0.0f, f * TERM);
		e = VGet(LEN, 0.0f, f * TERM);
		DrawLine3D(s, e, 0xff0000);
		DrawSphere3D(e,10.0f,5, 0xff0000,0xff0000,true);

		//Z軸
		s = VGet(f * TERM, 0.0f, f * -LEN);
		e = VGet(f * TERM, 0.0f, f * LEN);
		DrawLine3D(s, e, 0xff0000);
		DrawSphere3D(e, 10.0f, 5, 0x0000ff, 0x0000ff, true);
	}
	//Y軸
	s = VGet(0.0f,-LEN,0.0f);
	e = VGet(0.0f, LEN, 0.0f);
	DrawLine3D(s, e, 0x00ff00);
	DrawSphere3D(e, 10.0f, 5, 0x00ff00, 0x00ff00, true);
}

void Stage::Update(void)
{
}



void Stage::Release()
{

}

int Stage::GetModelDungeon(void)
{
	return mModelDungeon;
}

int Stage::GetModelBattleShip(void)
{
	return mModelBattleShip;
}

VECTOR Stage::GetPosBattleShip(void)
{
	return mPosBattleShip;
}
