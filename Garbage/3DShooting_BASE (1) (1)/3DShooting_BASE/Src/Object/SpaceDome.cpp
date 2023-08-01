#include"../Manager/ResourceManager.h"
#include"../Manager/Resource.h"
#include"../Utility/AsoUtility.h"
#include "SpaceDome.h"

SpaceDome::SpaceDome(const Transform* target)
{
	mTarget = target;
}

SpaceDome::~SpaceDome(void)
{
}

void SpaceDome::Init(void)
{
	//モデル制御に必要な基本情報
	mTransform.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::SKYDOME)
	);
	mTransform.scl = { 1.0f,1.0f,1.0f };
	mTransform.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	mTransform.pos = { 0.0f,0.0f,0.0f };
	mTransform.Update();


	//Zbaffaを書き込まない
	MV1SetWriteZBuffer(mTransform.modelId, false);

	if (mTarget==nullptr)
	{
		ChangeState(STATE::STAY);
	}
	else
	{
		ChangeState(STATE::FOLLOW);
	}


}

void SpaceDome::Update(void)
{
	switch (mState)
	{
	case SpaceDome::STATE::STAY:
		break;
	case SpaceDome::STATE::FOLLOW:
		mTransform.pos = mTarget->pos;
		break;
	}

	mTransform.Update();
}

void SpaceDome::Draw(void)
{
	MV1DrawModel(mTransform.modelId);
}



void SpaceDome::Release(void)
{
}

void SpaceDome::ChangeState(STATE state)
{
	mState = state;
	switch (mState)
	{
	case SpaceDome::STATE::STAY:
		mTransform.pos = { 0.0f,0.0f,0.0f };
		break;
	case SpaceDome::STATE::FOLLOW:
		mTransform.pos = mTarget->pos;
		break;
	}

}
