#include<DxLib.h>
#include "Rock.h"

Rock::Rock(void)
{
}

Rock::~Rock(void)
{

}

void Rock::Init(int modelId, VECTOR pos, VECTOR angle, VECTOR scale)
{
	mTransform.SetModel(modelId);
	mTransform.scl = scale;
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal = Quaternion::Euler(angle);
	mTransform.pos = pos;
	mTransform.Update();
}

void Rock::Update(void)
{
}

void Rock::Draw(void)
{
	//カメラクリップ外になったら描画しない
	if (!CheckCameraViewClip(mTransform.pos))
	{
		MV1DrawModel(mTransform.modelId);
	}

}

void Rock::DrawGrid(void)
{
}

void Rock::Release(void)
{
}
