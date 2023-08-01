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
	mTransform_.SetModel(modelId);
	mTransform_.scl = scale;
	mTransform_.quaRot = Quaternion();
	mTransform_.quaRotLocal = Quaternion::Euler(angle);
	mTransform_.pos = pos;
	mTransform_.Update();
}

void Rock::Update()
{
}

void Rock::Draw()
{
	if (!CheckCameraViewClip(mTransform_.pos));
	{
		MV1DrawModel(mTransform_.modelId);
	}
}

void Rock::Release()
{
}
