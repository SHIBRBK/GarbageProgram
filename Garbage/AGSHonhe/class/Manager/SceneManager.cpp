#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Scene/TitleScene.h"
#include "../../Resource.h"
#include "Camera.h"
#include "SceneManager.h"
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

	/*shadIdP = Resource::MyLoadPixelShader(Resource::PATH_SHADER+"Post.pso");
	normal = Resource::LoadTexture(Resource::PATH_TEXTURE+"gruss_n.png");
	fuck = Resource::LoadTexture(Resource::PATH_TEXTURE+"fucktra.png");
	offscreen = MakeScreen(1024, 640);

	cbufferH = CreateShaderConstantBuffer(sizeof(float) * 4);
	params = static_cast<float*>(GetBufferShaderConstantBuffer(cbufferH));
	id_time = 0;
	id_cut = 1;

	params[id_time] = 0;
	params[id_cut] = 250.0f;*/

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		Update();

		Draw();
		//SetDrawScreen(offscreen);
		//// カメラ設定
		//camera_->SetBeforeDraw();
		//ClearDrawScreen();
		//DrawGraph(0, 0, fuck, false);

		//SetDrawScreen(DX_SCREEN_BACK);
		//ClearDrawScreen();
		//UpdateShaderConstantBuffer(cbufferH);
		//SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_PIXEL, 0);
		//SetUsePixelShader(shadIdP);
		//SetUseTextureToShader(0, offscreen);
		//SetUseTextureToShader(1, normal);
		//MyDrawGraph(0, 0, 1024, 640);
		//DrawFormatString(0, 210, 0xffffffff, "FPS=%f", GetFPS());
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

	// カメラ設定
	// 初期状態では視点の位置が、 
	// x = 320.0f, y = 240.0f, z = (画面のサイズによって変化)
	// 注視点の位置は、
	// x = 320.0f, y = 240.0f, z = 1.0f、
	// カメラの上方向は
	// x = 0.0f, y = 1.0f, z = 0.0f
	// つまり画面のＸＹ平面上の中心に居てＺ軸のプラス方向を見るようなカメラになっています。
	//SetCameraPositionAndAngle(
	//	{ 0.0f, 200, -500.0f },
	//	30.0f * (DX_PI_F / 180.0f), 0.0f, 0.0f
	//);

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
	//if (sceneID_ == SCENE_ID::GAME)
	//{
	//	shadeIdV = LoadVertexShader("../../VertexShader.vso");
	//	shadIdP = LoadPixelShader("../../PixelShader.pso");
	//	shadIdC = LoadPixelShader("../../calculation.pso");
	//	MoHandle = Resource::LoadModel(Resource::PATH_MODEL + "Stage/plane4.mv1");
	//	SetCameraPositionAndTarget_UpVecY(camera_->DEFAULT_CAMERA_POS, VGet(0, 0, 0));

	//	constexpr int screenWidth = 0;
	//	constexpr int screenHeight = 1;
	//	constexpr int constant = 2;
	//	cbufferH = CreateShaderConstantBuffer(sizeof(float) * 4);
	//	params = static_cast<float*>(GetBufferShaderConstantBuffer(cbufferH));
	//	params[screenWidth] = 1024;
	//	params[screenHeight] = 640;
	//	params[constant] = 3;

	//	ClearDrawScreen();
	//	if (CheckHitKey(KEY_INPUT_UP))
	//	{
	//		SetDrawScreen(Prevscreen);

	//		DrawGraph(0, 0, screen, true);
	//		DrawCircle(0, 0, 100, 0xFFFFFF, true, 0);
	//	}
	//	MV1SetUseOrigShader(TRUE);
	//	UpdateShaderConstantBuffer(cbufferH);
	//	SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_PIXEL, 5);
	//	SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_VERTEX, 5);
	//	SetUseTextureToShader(1, Prevscreen);
	//	SetDrawScreen(screen);
	//	VERTEX2DSHADER Vert[6] = { 0.0f };
	//	Vert[0].pos = VGet(0.0f, params[screenHeight], 0.0f);
	//	Vert[0].u = 0.0f;
	//	Vert[0].v = 0.0f;

	//	Vert[1].pos = VGet(params[screenWidth], params[screenHeight], 0.0f);
	//	Vert[1].u = 1.0f;
	//	Vert[1].v = 0.0f;

	//	Vert[2].pos = VGet(0.0f, 0.0f, 0.0f);
	//	Vert[2].u = 0.0f;
	//	Vert[2].v = 1.0f;

	//	Vert[3].pos = VGet(params[screenWidth], 0.0f, 0.0f);
	//	Vert[3].u = 1.0f;
	//	Vert[3].v = 1.0f;

	//	Vert[4] = Vert[2];
	//	Vert[5] = Vert[1];
	//	DrawPolygon2DToShader(Vert, 6);

	//	SetDrawScreen(DX_SCREEN_BACK);
	//	ClearDrawScreen();
	//	SetCameraPositionAndTargetAndUpVec(pos, VGet(0, 0, 0), {0.0f,1.0f,0.0f});
	//	SetCameraNearFar(camera_.get()->CAMERA_NEAR, camera_.get()->CAMERA_FAR);

	//	SetUseTextureToShader(0, screen);

	//	MV1DrawModel(MoHandle);
	//}

	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)



		//params[id_time] += 0.01f;

	
	camera_->SetBeforeDraw();

	

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();


	

	// 描画
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

