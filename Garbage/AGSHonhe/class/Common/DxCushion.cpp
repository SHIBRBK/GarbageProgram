#include<DxLib.h>
#include "DxCushion.h"
#include<cassert>
#include<array>

int DxCushion::MyDxLib_Init()
{
	auto init = DxLib_Init();
	assert(init >= 0);
	return init;
}

int DxCushion::MyDxLib_End()
{
	auto end = DxLib_End();
	assert(end >= 0);
	return end;
}

int DxCushion::MyLoadGraph(const std::string path)
{
	auto load = LoadGraph(path.c_str());
	//assert(load >= 0);
	return load;
}

void DxCushion::MyDrawGraph(int x, int y, int handle, bool trans)
{
	DrawGraph(x, y, handle, trans);
}

int DxCushion::MyMakeScreen(int x, int y, int alpha)
{
	auto make=MakeScreen(x, y, alpha);
	assert(make >= 0);
	return make;
}

void DxCushion::MyScreenFlip()
{
	ScreenFlip();
}

int DxCushion::MySetDrawScreen(int screen)
{
	SetDrawScreen(screen);
	return screen;
}

void DxCushion::MyClsDrawScreen()
{
	ClsDrawScreen();
}

void DxCushion::MyClearDrawScreen()
{
	ClearDrawScreen();
}

int DxCushion::MySetDrawBlendMode(int BlendMode, int BlendParam)
{
	auto blend= SetDrawBlendMode(BlendMode, BlendParam);
	assert(blend >= 0);
	return blend;
}

int DxCushion::MyProcessMessage()
{
	auto check=ProcessMessage();
	return check;
}

int DxCushion::MyCheckHitKey(int keycode)
{
	auto key = CheckHitKey(keycode);
	assert(key>=0);
	return key;
}

int DxCushion::MySetGraphMode(int ScreenSizeX, int ScreenSizeY, int ColorBitDepth)
{
	auto setGraph = SetGraphMode(ScreenSizeX, ScreenSizeY, ColorBitDepth);
	assert(setGraph >= 0);
	return setGraph;
}

int DxCushion::MyChangeWindowMode(int Flag)
{
	auto change = ChangeWindowMode(Flag);
	assert(change >= 0);
	return change;
}

int DxCushion::MyInitGraph()
{
	auto initGraph = InitGraph();
	assert(initGraph>=0);
	return initGraph;
}

int DxCushion::MyDeleteGraph(int GrHandle, int LogOutFlag)
{
	auto deleteGraph = DeleteGraph(GrHandle, LogOutFlag);
	assert(deleteGraph >= deleteGraph);
	return deleteGraph;
}

int DxCushion::MyInitSoundMem(int LogOutFlag)
{
	auto InitSound = InitSoundMem(LogOutFlag);
	assert(InitSound >= 0);
	return InitSound;
}






int DxCushion::MyLoadSoundMem(const TCHAR* FileName)
{
	int loadSound = LoadSoundMem(FileName);
	assert(loadSound >= 0);
	return loadSound;
}

int DxCushion::MyDeleteSoundMem(int SoundHandle, int LogOutFlag)
{
	auto deleteSound = DeleteSoundMem(SoundHandle);
	assert(SoundHandle >= 0);
	return SoundHandle;
}

void DxCushion::MyMV1InitModel()
{
	MV1InitModel();
}

int DxCushion::MyMV1LoadModel(const TCHAR* FileName)
{
 	auto LoadMode = MV1LoadModel(FileName);
	assert(LoadMode >= 0);
	return LoadMode;
}

int DxCushion::MyMV1DeleteModel(int mHandle)
{
	auto deleteModel = MV1DeleteModel(mHandle);
	assert(deleteModel>=0);
	return deleteModel;
}

int DxCushion::MySetCameraNearFar(float Near, float Far)
{
	auto SetCamera = SetCameraNearFar(Near, Far);
	assert(SetCamera >= 0);
	return SetCamera;
}



int DxCushion::MyGetMousePoint(int* XBuf, int* YBuf)
{
	auto getMouse = GetMousePoint(XBuf, YBuf);
	assert(getMouse >= 0);
	return getMouse;
}
