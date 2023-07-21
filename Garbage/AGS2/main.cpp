#include<DxLib.h>
#include "Application.h"
//#include"class/vector2D.h"
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// �C���X�^���X�̐���
	Application::CreateInstance();

	// �C���X�^���X�̎擾
	Application& instance = Application::GetInstance();

	if (instance.IsInitFail())
	{

		return -1;
	}

	// ���s
	instance.Run();

	// ���
	instance.Release();

	if (instance.IsReleaseFail())
	{

		return -1;
	}

	return 0;
}