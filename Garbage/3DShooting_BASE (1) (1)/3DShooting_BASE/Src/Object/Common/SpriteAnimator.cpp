#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../../Utility/AsoUtility.h"
#include "Transform.h"
#include "SpriteAnimator.h"

SpriteAnimator::SpriteAnimator(ResourceManager::SRC src, float size, float speed)
{

	// 画像の読み込み
	Resource res = ResourceManager::GetInstance().Load(src);
	mCntTotalAnim = res.mNumX * res.mNumY;
	mImages = res.mHandleIds;

	// 大きさ
	mSize = size;

	// アニメーションスピード
	mSpeedAnim = speed;

	ChangeState(STATE::NONE);

}

SpriteAnimator::~SpriteAnimator(void)
{
}

void SpriteAnimator::Create(VECTOR pos, const Transform* transform)
{
	mStepAnim = 0.0f;
	mPos = pos;
	mTrackingTransform = transform;
	if (mTrackingTransform != nullptr)
	{
		mTrackingPrePos = transform->pos;
	}
	ChangeState(STATE::PLAY);
}

void SpriteAnimator::Update(void)
{

	switch (mState)
	{
	case SpriteAnimator::STATE::NONE:
		break;
	case SpriteAnimator::STATE::PLAY:
	{

		mStepAnim += SceneManager::GetInstance().GetDeltaTime();
		int cnt = AsoUtility::Round(mStepAnim * mSpeedAnim);
		mIdxAnim = cnt % mCntTotalAnim;

		if (cnt >= mCntTotalAnim)
		{
			ChangeState(STATE::END);
			return;
		}

		if (mTrackingTransform != nullptr)
		{
			auto diff = VSub(mTrackingTransform->pos, mTrackingPrePos);
			mPos = VAdd(mPos, diff);
			mTrackingPrePos = mTrackingTransform->pos;
		}

	}
	break;

	case SpriteAnimator::STATE::END:
		break;
	default:
		break;
	}

}

void SpriteAnimator::Draw(void)
{

	switch (mState)
	{
	case SpriteAnimator::STATE::NONE:
		break;
	case SpriteAnimator::STATE::PLAY:

		DrawModiBillboard3D(
			mPos,
			// 左上
			-mSize,
			mSize,
			// 右上
			mSize,
			mSize,
			// 右下
			mSize,
			-mSize,
			// 左下
			-mSize,
			-mSize,
			mImages[mIdxAnim],
			true
		);

#ifdef _DEBUG
		//DrawSphere3D(mPos, 10.0f, 3, 0x00ff00, 0x222222, true);
#endif

		break;
	case SpriteAnimator::STATE::END:
		break;
	default:
		break;
	}

}

void SpriteAnimator::Release(void)
{
}

VECTOR SpriteAnimator::GetPos(void) const
{
	return mPos;
}

void SpriteAnimator::SetPos(VECTOR pos)
{
	mPos = pos;
}

float SpriteAnimator::GetZLen(void) const
{
	return mZLen;
}

void SpriteAnimator::SetZLen(float len)
{
	mZLen = len;
}

bool SpriteAnimator::IsEnd(void) const
{
	return mState == STATE::END;
}

void SpriteAnimator::ChangeState(STATE state)
{

	mState = state;
	switch (mState)
	{
	case SpriteAnimator::STATE::NONE:
		break;
	case SpriteAnimator::STATE::PLAY:
		mStepAnim = 0.0f;
		break;
	case SpriteAnimator::STATE::END:
		break;
	}

}
