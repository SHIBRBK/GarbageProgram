#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "class/Manager/InputManager.h"
#include "class/Manager/SceneManager.h"
#include "Application.h"

Application* Application::mInstance = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";

void Application::CreateInstance(void)
{
	if (mInstance == nullptr)
	{
		mInstance = new Application();
	}
	mInstance->Init();
}

Application& Application::GetInstance(void)
{
	return *mInstance;
}

void Application::Init(void)
{

	// アプリケーションの初期設定
	SetWindowText("3DShooting");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	mIsInitFail = false;
	if (DxLib_Init() == -1)
	{
		mIsInitFail = true;
		return;
	}

	// Effekseerの初期化
	InitEffekseer();

	// キー制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();


	// シーン管理初期化
	SceneManager::CreateInstance();

}

void Application::Run(void)
{

	auto& intpuManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{


		intpuManager.Update();
		sceneManager.Update();

		sceneManager.Draw();



#ifdef _DEBUG

#endif

		ScreenFlip();

	}

}

void Application::Release(void)
{

	// Effekseerを終了する。
	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		mIsReleaseFail = true;
	}

}

bool Application::IsInitFail(void) const
{
	return mIsInitFail;
}

bool Application::IsReleaseFail(void) const
{
	return mIsReleaseFail;
}

Application::Application(void)
{
	mIsInitFail = false;
	mIsReleaseFail = false;
}

Application::~Application(void)
{
	delete mInstance;
}

void Application::InitEffekseer(void)
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}
