#include"MyDx/DxLib.h"
#include<vector>
#include<array>
#include<cassert>
#include<algorithm>

using namespace std;
constexpr float LEN = 300.0f;
constexpr float TERM = 50.0f;
const int NUM = static_cast<int>(LEN / TERM);

const int screenW=640;
const int screenH=480;


struct BitField
{
	char a : 1;
	char b: 2;
	char c : 3;
	char d : 2;
};


void DrawModel(int model,int vs,int ps,bool outline=false) {
	auto num=MV1GetMeshNum(model);
	DxLib::MV1SetUseOrigShader(true);
	SetUseVertexShader(vs);
	SetUsePixelShader(ps);
	for(int i=0;i<num;++i){
		MV1SetMeshBackCulling(model, i, DX_CULLING_NONE);
	}

	MV1DrawModel(model);

	if(outline){
		DxLib::MV1SetUseOrigShader(true);
		SetUseVertexShader(vs);
		SetUsePixelShader(ps);
		for (int i = 0; i < num; ++i) {
			MV1SetMeshBackCulling(model, i, DX_CULLING_RIGHT);
		}
		MV1DrawModel(model);
	}
}

void MyDrawGraph(int x, int y, int width, int height)
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


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	ChangeWindowMode(TRUE);

	SetBackgroundColor(200, 200, 200);
	SetGraphMode(640, 480, 32);

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	
	VECTOR pos = VGet(0.0f, 50.0f, -300.0f);
	VECTOR cameraUp_ = { 0.0f,1.0f,0.0f };
	SetCameraPositionAndTarget_UpVecY(pos, VGet(0, 0, 0));
	int vshandle = LoadVertexShader("Vertex.vso");
	int pshandle = LoadPixelShader("Pixel.pso");
	int Calchandle = LoadPixelShader("Calc.pso");
	//int noiseps = LoadPixelShader("Post.pso");
	//int normalH = LoadGraph("NormalMap.png");
	int testH = LoadGraph("fucktra.png");
	int MoHandle = MV1LoadModel("Model/plane4.mv1");
	int DebugAnother = MV1LoadModel("Model/dairi.mv1");

	MV1SetPosition(MoHandle, { 0.0f,0.0f,0.0f });
	MV1SetScale(MoHandle, { 0.01f,0.01f,0.01f });
	MV1SetMaterialDifColor(MoHandle,0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));


	MV1SetPosition(DebugAnother, { 120.0f,0.0f,-150.0f });
	MV1SetRotationXYZ(DebugAnother, { 0.0f,90.0f,0.0f });
	MV1SetScale(DebugAnother, { 0.05f,0.05f,0.05f });
	MV1SetMaterialDifColor(DebugAnother, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));


	int Prevscreen = MakeScreen(screenW, screenH, FALSE);//前スクリーン兼計算前スクリーン 入力用スクリーン
	int screen = MakeScreen(screenW, screenH, FALSE);//それをもとに計算したスクリーン
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
	params[constant] =1;
	char keystate[256];
	SetDrawScreen(DX_SCREEN_BACK);
	while (ProcessMessage() != -1 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();
		if (CheckHitKey(KEY_INPUT_U))
		{
		SetDrawScreen(Prevscreen);
		DrawGraph(0, 0, screen, true);
		DrawCircle(0, 0, 100, 0x000000, true, 0);
		}

	SetDrawScreen(screen);
	MV1SetUseOrigShader(TRUE);
	SetUsePixelShader(Calchandle);
	UpdateShaderConstantBuffer(cbufferH);
	SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_PIXEL, 5);
	SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_VERTEX, 5);
	SetUseTextureToShader(2, Prevscreen);
	
	SetCameraPositionAndTargetAndUpVec(pos, VGet(0, 0, 0), cameraUp_);
	
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
	DrawPolygon2DToShader(verts.data(), verts.size());

	SetDrawScreen(DX_SCREEN_BACK);
	SetCameraPositionAndTargetAndUpVec(pos, VGet(0, 0, 0), cameraUp_);
	ClearDrawScreen();

	DrawExtendGraph(0,0,150, 150, Prevscreen,true);
	SetCameraNearFar(10.0f, 500.0f);
	//---------------------------------------------------------
	DrawLine3D({ 0.0f,0.0f,0.0f }, { 100.0f,0.0f,0.0f }, 0xFF0000);//X
	DrawLine3D({ 0.0f,0.0f,0.0f }, { 0.0f,100.0f,0.0f }, 0x00FF00);//Y
	DrawLine3D({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,100.0f }, 0x0000FF);//Z
	//---------------------------------------------------------

	SetUseVertexShader(vshandle);
	SetUsePixelShader(pshandle);
	MV1DrawModel(MoHandle);
	MV1DrawModel(DebugAnother);
	SetUseTextureToShader(1, screen);
	//DrawGraph(0, 0, testH, false);

		/*UpdateShaderConstantBuffer(cbufferH);
		SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_PIXEL, 0);*/
		//SetUsePixelShader(noiseps);
		//SetUseTextureToShader(0, offscreen);
		//SetUseTextureToShader(1, normalH);
		//MyDrawGraph(0, 0, 640, 480);
	DrawFormatString(10, 10, 0xffffffff, "FPS=%f", GetFPS());

	ScreenFlip();
	}


	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}