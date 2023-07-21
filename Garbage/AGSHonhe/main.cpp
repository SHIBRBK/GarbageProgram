#include<DxLib.h>
#include "Application.h"
//#include"class/vector2D.h"
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// インスタンスの生成
	Application::CreateInstance();

	// インスタンスの取得
	Application& instance = Application::GetInstance();

	if (instance.IsInitFail())
	{

		return -1;
	}

	// 実行
	instance.Run();

	// 解放
	instance.Release();

	if (instance.IsReleaseFail())
	{

		return -1;
	}

	return 0;
}