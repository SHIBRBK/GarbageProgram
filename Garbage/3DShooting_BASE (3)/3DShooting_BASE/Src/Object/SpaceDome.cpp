#include"../Manager/ResourceManager.h"
#include"../Manager/Resource.h"
#include"../Utility/AsoUtility.h"
#include "SpaceDome.h"

SpaceDome::SpaceDome(const Transform* target)
{
	mTarget_= target;
}

SpaceDome::~SpaceDome()
{
}

void SpaceDome::Init()
{
	//モデル制御の基本情報
	mTransform_.SetModel(
	ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::SKYDOME));

	mTransform_.scl = { 1.0f,1.0f,1.0f };
	mTransform_.quaRot = Quaternion::Euler(
	0.0f,
	AsoUtility::Deg2RadF(180.0f),
	0.0f
	);
	mTransform_.pos = { 0.0f,0.0f,0.0f };
	mTransform_.Update();

	//Zバッファを書き込まない。
	MV1SetWriteZBuffer(mTransform_.modelId, false);


	if (mTarget_ == nullptr)
	{
		ChangeState(STATE::STAY);
	}
	else
	{
		ChangeState(STATE::FOLLOW);
	}
}

void SpaceDome::Draw()
{
	MV1DrawModel(mTransform_.modelId);
}


void SpaceDome::Update()
{
	switch (mState)
	{
	case SpaceDome::STATE::STAY:
		break;
	case SpaceDome::STATE::FOLLOW:
		mTransform_.pos = mTarget_->pos;
		break;
	default:
		break;
	}






	mTransform_.Update();
}

void SpaceDome::Release()
{
	
}

void SpaceDome::ChangeState(STATE state)
{
	mState = state;
	switch (mState)
	{
	case SpaceDome::STATE::STAY:
		mTransform_.pos = { 0.0f,0.0f,0.0f };
		break;
	case SpaceDome::STATE::FOLLOW:
		mTransform_.pos = mTarget_->pos;
		break;
	}

	mTransform_.Update();

}
