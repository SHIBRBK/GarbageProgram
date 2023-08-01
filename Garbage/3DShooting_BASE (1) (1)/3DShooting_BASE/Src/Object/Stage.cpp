#include<DxLib.h>
#include"../Manager/ResourceManager.h"
#include"../Manager/Resource.h"
#include "Stage.h"
#include"../Utility/AsoUtility.h"

namespace
{
	// グリッド線
	static constexpr float LEN_LINE = 1000.0f;
	// グリッド線
	static constexpr float TERM_LINE = 50.0f;
}

Stage::Stage(void)
{

}

Stage::~Stage(void)
{

}

void Stage::Init(void)
{
	mModelDungeon = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::DUNGEON
	);
	mPosDungeon = { 1000.0f,400.0f,1300.0f };
	MV1SetScale(mModelDungeon, AsoUtility::VECTOR_ONE);
	MV1SetPosition(mModelDungeon, mPosDungeon);
	MV1SetRotationXYZ(mModelDungeon, { 0.0f,180.0f,0.0f });
	MV1SetPosition(mModelDungeon, mPosDungeon);

	mModelCargoShip = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::BOSS_SHIP
	);
	mPosCargoShip = { 14500.0f,-800.0f,6500.0f };
	MV1SetScale(mModelCargoShip, { 2.0f,2.0f,2.0f });
	MV1SetPosition(mModelCargoShip, mPosCargoShip);
	MV1SetRotationXYZ(mModelCargoShip, { 0.0f,180.0f,0.0f });
	MV1SetPosition(mModelCargoShip, mPosCargoShip);

	//当たり判定
	int ret;
	ret = MV1SetupCollInfo(mModelDungeon,-1);
	ret = MV1SetupCollInfo(mModelCargoShip,-1);
}

void Stage::Update(void)
{
	
}

void Stage::Draw(void)
{
	//モデルの描画
	MV1DrawModel(mModelDungeon);
	MV1DrawModel(mModelCargoShip);


	DrawGrid();
	//デバッグ用：戦艦の周囲描画
	DrawSphere3D(
		mPosCargoShip, 5000.0f, 20, 0xff0000, 0xff0000, false);
}

void Stage::DrawGrid(void)
{
	const float RADIUS = 8.0f;
	const int R = 0xff0000;
	const int G = 0x00ff00;
	const int B = 0x0000ff;

	float x;
	float y;
	float z;

	VECTOR s;
	VECTOR e;

	// XYZ基本軸(グリッド)
	int cnt = LEN_LINE / TERM_LINE * 2 + 1;
	for (int i = 0; i < cnt; i++)
	{

		// X軸(赤)
		z = -LEN_LINE + i * TERM_LINE;
		s = VGet(-LEN_LINE, 0.0f, z);
		e = VGet(LEN_LINE, 0.0f, z);
		DrawLine3D(s, e, R);
		DrawSphere3D(e, RADIUS, 5, R, R, true);

		// Z軸(青)
		x = -LEN_LINE + i * TERM_LINE;
		s = VGet(x, 0.0f, -LEN_LINE);
		e = VGet(x, 0.0f, LEN_LINE);
		DrawLine3D(s, e, B);
		DrawSphere3D(e, RADIUS, 5, B, B, true);

	}

	// Y軸(緑)
	DrawLine3D(
		VGet(0.0f, -LEN_LINE, 0.0f),
		VGet(0.0f, LEN_LINE, 0.0f),
		G);
	DrawSphere3D(VGet(0.0f, LEN_LINE / 2, 0.0f), RADIUS, 5, G, G, true);
	DrawSphere3D(VGet(0.0f, LEN_LINE, 0.0f), RADIUS, 5, G, G, true);

}

void Stage::Release(void)
{

}

int Stage::GetModelDungeon(void)
{
	return mModelDungeon;
}

int Stage::GetModelCargoShip(void)
{
	return mModelCargoShip;
}


