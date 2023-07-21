#include "../../Resource.h"
#include "SkyDome.h"

SkyDome::SkyDome(const Transform* syncTransform)
{
	mSyncTransform = syncTransform;
	mState = STATE::NONE;
}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Init(void)
{

	// ���f������̊�{���
	transform_.SetModel(
		Resource::LoadModel(Resource::PATH_MODEL+"SkyDome/Skydome.mv1"));
	transform_.scl = SCALES;
	transform_.pos = {0.0f,0.0f,0.0f};
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		Deg2RadF(180.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	// Z�o�b�t�@����(�˂������΍�)
	MV1SetUseZBuffer(transform_.modelId, false);
	MV1SetWriteZBuffer(transform_.modelId, false);

	// ��ԑJ��
	if (mSyncTransform == nullptr)
	{
		ChangeState(STATE::STAY);
	}
	else
	{
		ChangeState(STATE::FOLLOW);
	}

}

void SkyDome::Update(void)
{

	switch (mState)
	{
	case SkyDome::STATE::STAY:
		break;
	case SkyDome::STATE::FOLLOW:
		transform_.pos = mSyncTransform->pos;
		break;
	default:
		break;
	}

	transform_.Update();

}

void SkyDome::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void SkyDome::Release(void)
{
}

void SkyDome::ChangeState(STATE state)
{

	mState = state;
	switch (mState)
	{
	case SkyDome::STATE::STAY:
		break;
	case SkyDome::STATE::FOLLOW:
		transform_.pos = mSyncTransform->pos;
		break;
	}

	transform_.Update();

}

float SkyDome::Deg2RadF(float deg)
{
	return deg * (DX_PI_F / 180.0f);
}
