#include <cassert>
#include <DxLib.h>
#include "GameScene.h"
#include "TitleScene.h"
#include"../Manager/SceneManager.h"
#include "../Object/PlayerShot.h"
#include "../Object/SkyDome.h"
#include "../../Resource.h"
#include "../Common/DxCushion.h"
GameScene::GameScene() :BaseScene()
{
	mSkyDome = nullptr;
	bb = std::make_unique<BattleShip>();
	ec = std::make_unique<EnemyCannon>();
	camera_ = std::make_unique<Camera>();
}

GameScene::~GameScene()
{
}

void GameScene::SetSelectedStageIndex(int stageIndex)
{
	selectedStageIndex_ = stageIndex;
}

void GameScene::SetSelectedShipIndex(int shipIndex)
{
	selectedShipIndex_ = shipIndex;
}

bool GameScene::IsHitSpheres(const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2)
{
	// 球体同士の衝突判定
	bool ret = false;

	// お互いの半径の合計
	float radius = radius1 + radius2;

	// 座標の差からお互いの距離を取る
	VECTOR diff = VSub(pos2, pos1);

	// 三平方の定理で比較(SqrMagnitudeと同じ)
	float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
	if (dis < (radius * radius))
	{
		ret = true;
	}

	return ret;
}



void GameScene::Init(void)
{
	bb->Init();
	ec->Init();
	// スカイドーム
	mSkyDome = new SkyDome(&bb.get()->GetTransform());
	mSkyDome->Init();
	SceneManager::GetInstance().GetCamera()->SetPlayerPos(&bb.get()->GetTransform());
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);
	camera_.get()->GetInstance();
	shadIdP = LoadPixelShader("../../PixelShader.pso");
	shadIdV = LoadVertexShader("../../VertexShader.vso");
	shadIdC = LoadVertexShader("../../calculation.pso");
	test = Resource::LoadModel(Resource::PATH_MODEL + "Stage/plane4.mv1");
	//プレイヤーの破壊演出時間
	mStepDestroy = 3.0f;
	time = 5000;
}

void GameScene::Update()
{
	mSkyDome->Update();
	bb->Update();
	ec->Update();
	auto shots = bb->GetShots();
	for (auto e : spawn_)
	{
		e->Update();
		if (!e->IsAlive())
		{
			continue;

		}
		for (auto s : shots)
		{
			s->Update();

			if (!s->IsAlive())
			{
				continue;
			}
			if (IsHitSpheres(
				e->GetTransform().pos,
				e->COLLISION_RADIUS,
				s->GetPos(), s->GetCollisionRadius()))
			{
				e->Damage();
				break;
			}

		}

	}

	auto eshots = ec->GetShots();
	for (auto s : spawn_)
	{
		s->Update();
		for (auto e : eshots)
		{
			e->Update();

			if (!e->IsAlive())
			{
				continue;
			}

			if (!s->IsAlive())
			{
				continue;
			}

			if (IsHitSpheres(
				bb->GetTransform().pos,
				bb.get()->COLLISION_RADIUS,
				e->GetPos(), e->GetCollisionRadius()))
			{
				s->Damage();
				//e->ShotDead(&tank_->GetTransform());
				break;
			}

		}

		//MV1CollResultPolyDimTerminate(info);
	}
	auto scene_ = SceneManager::GetInstance;
	mStepDestroy -= scene_().GetDeltaTime();

	time-= scene_().GetDeltaTime();

	if (time==0.0f)
	{
		scene_().ChangeScene(
			SceneManager::SCENE_ID::TITLE, true);
	}
}

void GameScene::Draw(void)
{


	//int Prevscreen = MakeScreen(1024, 640);//前スクリーン兼計算前スクリーン 入力用スクリーン
	//int screen = MakeScreen(1024, 640);//それをもとに計算したスクリーン
	//constexpr int screenWidth = 0;
	//constexpr int screenHeight = 1;
	//constexpr int constant = 2;
	//int cbufferH = CreateShaderConstantBuffer(sizeof(float) * 4);
	//float* params = static_cast<float*>(GetBufferShaderConstantBuffer(cbufferH));
	//params[screenWidth] = 1024;
	//params[screenHeight] = 640;
	//params[constant] = 10;
	//
	//if (CheckHitKey(KEY_INPUT_U))
	//{
	//	SetDrawScreen(Prevscreen);
	//	DrawGraph(0, 0, screen, true);
	//	DrawCircle(0, 0, 100, 0x000000, true, 0);
	//}
	//MV1SetUseOrigShader(TRUE);
	//SetUsePixelShader(shadIdC);
	//UpdateShaderConstantBuffer(cbufferH);
	//SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_PIXEL, 5);
	//SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_VERTEX, 5);
	//SetUseTextureToShader(1, Prevscreen);
	//SetDrawScreen(screen);
	//VERTEX2DSHADER Vert[6] = { 0.0f };
	//Vert[0].pos = VGet(0.0f, params[screenHeight], 0.0f);
	//Vert[0].u = 0.0f;
	//Vert[0].v = 0.0f;

	//Vert[1].pos = VGet(params[screenWidth], params[screenHeight], 0.0f);
	//Vert[1].u = 1.0f;
	//Vert[1].v = 0.0f;
	//Vert[2].pos = VGet(0.0f, 0.0f, 0.0f);
	//Vert[2].u = 0.0f;
	//Vert[2].v = 1.0f;
	//Vert[3].pos = VGet(params[screenWidth], 0.0f, 0.0f);
	//Vert[3].u = 1.0f;
	//Vert[3].v = 1.0f;
	//Vert[4] = Vert[2];
	//Vert[5] = Vert[1];
	//DrawPolygon2DToShader(Vert, 6);

	//SetDrawScreen(DX_SCREEN_BACK);
	//ClearDrawScreen();
	//
	//DrawLine3D({ 0.0f,0.0f,0.0f }, { 100.0f,0.0f,0.0f }, 0xFF0000);//X
	//DrawLine3D({ 0.0f,0.0f,0.0f }, { 0.0f,100.0f,0.0f }, 0x00FF00);//Y
	//DrawLine3D({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,100.0f }, 0x0000FF);//Z
	//SetUseVertexShader(shadIdV);
	//SetUsePixelShader(shadIdP);
	//SetUseTextureToShader(0, screen);


	//shadIdP = Resource::MyLoadPixelShader(Resource::PATH_SHADER + "Post.pso");
	//normal = Resource::LoadTexture(Resource::PATH_TEXTURE + "gruss_n.png");
	//fuck = Resource::LoadTexture(Resource::PATH_TEXTURE + "fucktra.png");
	//offscreen = MakeScreen(1024, 640);
	//cbufferH = CreateShaderConstantBuffer(sizeof(float) * 4);
	//params = static_cast<float*>(GetBufferShaderConstantBuffer(cbufferH));
	//id_time = 0;
	//id_cut = 1;
	//SetDrawScreen(offscreen);
	//// カメラ設定
	//camera_->SetBeforeDraw();
	//ClearDrawScreen();
	//DrawGraph(0, 0, fuck, false);

	//SetDrawScreen(DX_SCREEN_BACK);
	//ClearDrawScreen();
	//// this->SetDefault();

	//UpdateShaderConstantBuffer(cbufferH);
	//SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_PIXEL, 0);
	//SetUsePixelShader(shadIdP);
	//SetUseTextureToShader(0, offscreen);
	//SetUseTextureToShader(1, normal);
	//DxCushion::MyDrawGraph(0, 0, 1024, 640);
	//DrawFormatString(0, 210, 0xffffffff, "FPS=%f", GetFPS());



	//params[id_time] = 0;
	//params[id_cut] = 250.0f;


	mSkyDome->Draw();
	MV1DrawModel(test);

	constexpr float LEN = 3000.0f;
	constexpr float TERM = 100.0f;
	const int NUM = static_cast<int>(LEN / TERM);


	// XYZ基本軸
	VECTOR s;
	VECTOR e;

	for (int i = -NUM; i < NUM; i++)
	{
		float f = static_cast<float>(i);

		// X軸
		s = VGet(-LEN, 0.0f, f * TERM);
		e = VGet(LEN, 0.0f, f * TERM);
		DrawLine3D(s, e, GetColor(255, 0, 0));  // 赤色

		// Z軸
		s = VGet(f * TERM, 0.0f, -LEN);
		e = VGet(f * TERM, 0.0f, LEN);
		DrawLine3D(s, e, GetColor(0, 0, 255));  // 青色
	}

	// Y軸
	s = VGet(0.0f, -LEN, 0.0f);
	e = VGet(0.0f, LEN, 0.0f);
	DrawLine3D(s, e, GetColor(0, 255, 0));  // 緑色

	bb->Draw();
	ec->Draw();
	


	auto shots = bb->GetShots();
	for (auto shot : shots)
	{
		shot->Draw();
	}
	auto eshots = ec->GetShots();
	for (auto eshot : eshots)
	{
		eshot->Draw();
	}
	DrawFormatString(0, 300, 0xff0000, "%d", time);
	
}

void GameScene::Release(void)
{
	bb.get_deleter();
	mSkyDome->Release();
	delete mSkyDome;
}

