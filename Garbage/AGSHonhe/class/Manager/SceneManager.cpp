#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Scene/TitleScene.h"
#include "../../Resource.h"
#include "Camera.h"
#include "SceneManager.h"
#include "../../Application.h"
SceneManager* SceneManager::mInstance = nullptr;


using namespace std;
void SceneManager::MyDrawGraph(int x, int y, int width, int height)
{
	array<VERTEX2DSHADER, 4> verts;
	for (auto& v : verts) {
		v.rhw = 1.0f;
		v.dif = GetColorU8(0xff, 0xff, 0xff, 0xff);
		v.u = 0.0f;
		v.v = 0.0f;
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
		v.spc = GetColorU8(0, 0, 0, 0);
	}
	verts[0].pos.x = x;
	verts[0].pos.y = y;
	verts[1].pos.x = x + width;
	verts[1].pos.y = y;
	verts[1].u = 1.0f;
	verts[2].pos.x = x;
	verts[2].pos.y = y + height;
	verts[2].v = 1.0f;
	verts[3].pos.x = x + width;
	verts[3].pos.y = y + height;
	verts[3].u = 1.0f;
	verts[3].v = 1.0f;
	DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
}



void SceneManager::CreateInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new SceneManager();
	}
	mInstance->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *mInstance;
}

void SceneManager::Run(void)
{
	
	shadIdV = LoadVertexShader((Resource::PATH_SHADER + "VertexShader.vso").c_str());
	shadIdP = LoadPixelShader((Resource::PATH_SHADER + "PixelShader.vso").c_str());
	shadIdC = LoadPixelShader((Resource::PATH_SHADER + "calculation.pso").c_str());
	//int noiseps = LoadPixelShader("Post.pso");
	//int normalH = LoadGraph("NormalMap.png");
	fuck = LoadGraph("fucktra.png");
	test = MV1LoadModel((Resource::PATH_MODEL + "Stage/plane4.mv1").c_str());


	MV1SetPosition(test, { 0.0f,0.0f,0.0f });
	MV1SetScale(test, { 0.5f,0.5f,0.5f });
	MV1SetMaterialDifColor(test, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));

	int Prevscreen = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, FALSE);//前スクリーン兼計算前スクリーン 入力用スクリーン
	int screen = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, FALSE);//それをもとに計算したスクリーン
	//auto offscreen = MakeScreen(640, 480);

	constexpr int screenWidth = 0;
	constexpr int screenHeight = 1;
	constexpr int constant = 2;
	//GetScreenState(&screenWidth,&screenHeight,0);
	//X640 Y480
	int cbufferH = CreateShaderConstantBuffer(sizeof(float) * 4);
	float* params = static_cast<float*>(GetBufferShaderConstantBuffer(cbufferH));
	params[screenWidth] = 640;
	params[screenHeight] = 480;
	params[constant] = 1;
	UpdateShaderConstantBuffer(cbufferH);

	char keystate[256];


	for (auto& v : verts) {
		v.rhw = 1.0f;
		v.dif = GetColorU8(0xff, 0xff, 0xff, 0xff);
		v.u = 0.0f;
		v.v = 0.0f;
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
		v.spc = GetColorU8(0, 0, 0, 0);
	}
	verts[0].pos.x = 0;
	verts[0].pos.y = 0;
	verts[1].pos.x = 0 + params[screenWidth];
	verts[1].pos.y = 0;
	verts[1].u = 1.0f;
	verts[2].pos.x = 0;
	verts[2].pos.y = 0 + params[screenHeight];
	verts[2].v = 1.0f;
	verts[3].pos.x = 0 + params[screenWidth];
	verts[3].pos.y = 0 + params[screenHeight];
	verts[3].u = 1.0f;
	verts[3].v = 1.0f;

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		Update();

		Draw();
		
	}
}

void SceneManager::Init(void)
{


	

	sceneID_ = SCENE_ID::TITLE;
	waitsceneID_ = SCENE_ID::NONE;

	mFader = std::make_unique<Fader>();
	mFader->Init();

	camera_ = std::make_unique<Camera>();
	camera_->Init();

	scene_ = std::make_unique<TitleScene>();
	scene_->Init();

	mIsSceneChanging = false;

	// デルタタイム
	mPreTime = std::chrono::system_clock::now();

	// 3D用の設定
	Init3D();



}

void SceneManager::Init3D(void)
{

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	//SetCameraNearFar(0.0f, 50.0f);

	// ライトの設定
	//ChangeLightTypeDir({ 0.0f, -1.0f, 0.0f });
	SetUseLighting(false);

	// 背景色設定
	SetBackgroundColor(0, 0, 0);
	//SetBackgroundColor(0, 100, 100);


	//フォグの設定(カメラから遠い位置に霧をかける)
	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(3000.0f, 15000.0f);

}

void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	mDeltaTime = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - mPreTime).count() / 1000000000.0);
	mPreTime = nowTime;

	mFader->Update();
	if (mIsSceneChanging)
	{
		Fade();
	}
	else
	{
		scene_->Update();
	}

	// カメラ更新ステップ
	camera_->Update();

}

void SceneManager::Draw(void)
{
	
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();



	camera_->SetBeforeDraw();
	scene_->Draw();
	//主にポストエフェクト
	//camera_->Draw();
	
	
	// 要リファクタリング
	switch (sceneID_)
	{
	case SCENE_ID::TITLE:
		break;
	case SCENE_ID::GAME:
		// UI描画

		break;
	}

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();

	mFader->Draw();

}

void SceneManager::DrawShader(void)
{
	SetDrawScreen(Prevscreen);
	ClearDrawScreen();
	DrawGraph(0, 0, screen, true);
	if (CheckHitKey(KEY_INPUT_UP))
	{

		DrawCircle(320, 240, 100, 0xff0000, true);
	}

	SetDrawScreen(screen);
	ClearDrawScreen();

	camera_.get()->SetBeforeDraw();

	MV1SetUseOrigShader(TRUE);

	SetUsePixelShader(shadIdC);
	SetUseTextureToShader(0, Prevscreen);

	SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_PIXEL, 5);
	SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_VERTEX, 5);

	DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);

	MV1SetUseOrigShader(false);

	SetUsePixelShader(-1);
	SetUseTextureToShader(0, -1);

	SetShaderConstantBuffer(-1, DX_SHADERTYPE_PIXEL, 5);
	SetShaderConstantBuffer(-1, DX_SHADERTYPE_VERTEX, 5);


	SetDrawScreen(DX_SCREEN_BACK);
	camera_.get()->SetBeforeDraw();
	ClearDrawScreen();

	DrawExtendGraph(0, 0, 320, 240, Prevscreen, true);

	//---------------------------------------------------------
	DrawLine3D({ 0.0f,0.0f,0.0f }, { 100.0f,0.0f,0.0f }, 0xFF0000);//X
	DrawLine3D({ 0.0f,0.0f,0.0f }, { 0.0f,100.0f,0.0f }, 0x00FF00);//Y
	DrawLine3D({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,100.0f }, 0x0000FF);//Z
	//---------------------------------------------------------

	MV1SetUseOrigShader(true);

	SetUseVertexShader(shadIdV);;

	SetUsePixelShader(shadIdP);
	SetUseTextureToShader(0, screen);

	
	MV1DrawModel(test);
	scene_->Draw();

	SetUseVertexShader(-1);

	SetUsePixelShader(-1);
	SetUseTextureToShader(0, -1);

	MV1SetUseOrigShader(false);


	/*UpdateShaderConstantBuffer(cbufferH);
	SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_PIXEL, 0);*/
	//SetUsePixelShader(noiseps);
	//SetUseTextureToShader(0, offscreen);
	//SetUseTextureToShader(1, normalH);
	//MyDrawGraph(0, 0, 640, 480);
	DrawFormatString(10, 10, 0xffffffff, "FPS=%f", GetFPS());
	
	
}



void SceneManager::ChangeScene(SCENE_ID nextId, bool isFading)
{

	waitsceneID_ = nextId;

	if (isFading)
	{
		mFader->SetFade(Fader::STATE::FADE_OUT);
		mIsSceneChanging = true;
	}
	else
	{
		DoChangeScene();
	}

}

SceneManager::SCENE_ID SceneManager::GetmSceneID(void)
{
	return sceneID_;
}

float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	return mDeltaTime;
}

Camera* SceneManager::GetCamera(void) const
{
	return camera_.get();
}

SceneManager::SceneManager(void)
{

	sceneID_ = SCENE_ID::NONE;
	waitsceneID_ = SCENE_ID::NONE;

	scene_ = nullptr;
	mFader = nullptr;

	mIsSceneChanging = false;

	// デルタタイム
	mDeltaTime = 1.0f / 60.0f;

	camera_ = nullptr;

}

SceneManager::~SceneManager(void)
{
	delete mInstance;
}

void SceneManager::ResetDeltaTime(void)
{
	mDeltaTime = 0.016f;
	mPreTime = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(void)
{

	// リソースの解放


	sceneID_ = waitsceneID_;

	switch (sceneID_)
	{
	case SCENE_ID::TITLE:
		scene_ = std::make_unique<TitleScene>();
		break;
	case SCENE_ID::GAME:
		scene_ = std::make_unique<GameScene>();
		break;

	}

	scene_->Init();

	
	ResetDeltaTime();

	waitsceneID_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{

	Fader::STATE fState = mFader->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		if (mFader->IsEnd())
		{
			mFader->SetFade(Fader::STATE::NONE);
			mIsSceneChanging = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		if (mFader->IsEnd())
		{
			DoChangeScene();
			mFader->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}

