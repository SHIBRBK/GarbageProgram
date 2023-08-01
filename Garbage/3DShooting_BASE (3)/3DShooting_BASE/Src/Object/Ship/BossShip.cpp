#include <vector>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/Camera.h"
#include "../../Utility/AsoUtility.h"
#include"../../Object/Ship/Turret.h"
#include"../Common/SpriteAnimator.h"
#include "EventShot.h"
#include "BossShip.h"

BossShip::BossShip(void)
{
}

BossShip::~BossShip(void)
{
}

void BossShip::Init()
{
	mStepEvent = TIME_EVENT;
	mTransform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::BOSS_SHIP));
		float scale = 2.0f;
		mTransform_.scl={ scale,scale ,scale };
		mTransform_.pos={0.0f,0.0f,4000.0f };
		mTransform_.quaRot = Quaternion::Euler(
			AsoUtility::Deg2RadF(20.0f),
			AsoUtility::Deg2RadF(210.0f),
			AsoUtility::Deg2RadF(0.0f)
		);

		mTransform_.quaRotLocal = Quaternion::Euler(
			AsoUtility::Deg2RadF(0.0f),
			AsoUtility::Deg2RadF(180.0f),
			AsoUtility::Deg2RadF(0.0f)
		);


		//���f������̊�{�����X�V
		mTransform_.Update();

		//�^���b�g�̑S�Ńt���O
		isAllDestroyTurrets = false;

		//�V�[���ɂ���ď�Ԃ�ς���
		SceneManager::SCENE_ID sceneID =
			SceneManager::GetInstance().GetmSceneID();

		if (sceneID == SceneManager::SCENE_ID::EVENT)
		{
			ChangeState(STATE::EVENT);
		}
		else
		{
			int ret = MV1SetupCollInfo(mTransform_.modelId, -1);
			ChangeState(STATE::BATTLE);
		}
}

void BossShip::Update()
{
	switch (mState)
		{
	case BossShip::STATE::EVENT:
		{
		mTransform_.pos =
			VAdd(mTransform_.pos, VScale(mTransform_.GetForward(), SPEED_MOVE));

		mEventshot->Update();
		if (!mEventshot->IsAlive())
			{
			auto& ins = SceneManager::GetInstance();
			mStepEvent -= ins.GetDeltaTime();
			if (mStepEvent < 0.0f)
				{
					ins.ChangeScene(SceneManager::SCENE_ID::BATTLE, true);
					return;
				}
			}
			mTransform_.Update();
			UpdateTurret();
		}
	break;
	case BossShip::STATE::BATTLE:
	{
		//���K�F�{�X��O�������Ɉړ������ĉ�����
		// �`������肢���܂�
		mTransform_.pos =
			VAdd(
				mTransform_.pos,
				VScale(mTransform_.GetForward(), SPEED_MOVE)
			);
		//�����͂�������



			//�f�O���[�p�����W�A���ɕϊ�
		float rad = AsoUtility::Deg2RadF(3.0f);
		float speed = rad * SceneManager::GetInstance().GetDeltaTime();
		//�w�莲(axis)�ɑ΂����]��(rad)�ɉ�������](Quaternion)�����
		Quaternion axis = Quaternion::AngleAxis(speed, AsoUtility::AXIS_Y);
		//���@�̉�]�ɁB���ō쐬������]��������B
		mTransform_.quaRot = mTransform_.quaRot.Mult(axis);

		//3D��b���̍X�V
		mTransform_.Update();
		UpdateTurret();

		//�����蔻��(�R���C�_)���X�V
		int ret = MV1RefreshCollInfo(mTransform_.modelId, -1);


		UpdateTurret();

		if (isAllDestroyTurrets)
		{
			ChangeState(STATE::DESTROY);
			return;
		}

		break;
	}
	case BossShip::STATE::DESTROY:
	{

		//�{�X�t�F�[�h
		auto& ins = SceneManager::GetInstance();
		mStepFade -= ins.GetDeltaTime();
		if (mStepFade < 0.0f)
		{
			ChangeState(STATE::END);
			return;
		}
		//�j��G�t�F�N�g
		mStepExplosion -= ins.GetDeltaTime();
		if (mStepExplosion < 0.0f)
		{
			CreateRandomExplosion();
			mStepExplosion = TIME_EXPLOSION;
		}

		//�����G�t�F�N�g�̍X�V
		for (auto e : mExplosions)
		{
			e->Update();
		}

		}
		break;
	}

}

void BossShip::UpdateTurret()
{
	//�S�Ŕ���
	int alived = 0;

	for (auto t : mTurrets)
	{
		t->Update();

		if (t->IsAlive())
		{
			alived++;


		}

		if (alived == 0)
		{
			isAllDestroyTurrets = true;
		}
	}
}

void BossShip::Draw()
{
	switch (mState)
	{
	case BossShip::STATE::EVENT:
		MV1DrawModel(mTransform_.modelId);
		mEventshot->Draw();
		DrawTurret();
		break;
	case BossShip::STATE::BATTLE:
		MV1DrawModel(mTransform_.modelId);
		DrawTurret();
		break;
	case BossShip::STATE::DESTROY:
	{
		float diff = TIME_FADE - mStepFade;
		auto color = AsoUtility::Lerp(FADE_FROM, FADE_TO, (diff / TIME_FADE));

		int num = MV1GetMaterialNum(mTransform_.modelId);
		for (int i = 0; i < num; i++)
		{
			MV1SetMaterialDifColor(mTransform_.modelId, i, color);
		}

		MV1DrawModel(mTransform_.modelId);

		//Z�\�[�g
		ZSortExplosion();

		//�����G�t�F�N�g�̕`��
		for (auto e : mExplosions)
		{
			e->Draw();
		}


	}

	break;
	
	case BossShip::STATE::END:
		//Z�\�[�g
		ZSortExplosion();

		//�����G�t�F�N�g�̕`��
		for (auto e : mExplosions)
		{
			e->Draw();
		}
	}
}

void BossShip::DrawTurret()
{
	for (auto t : mTurrets)
	{
		t->Draw();
	}
}

void BossShip::Release()
{
	if (mEventshot != nullptr)
	{
		mEventshot->Release();
		delete mEventshot;
	}

	for (auto t : mTurrets)
	{
		t->Release();
		delete t;
	}

	for (auto e : mExplosions)
	{
		e->Release();
		delete e;
	}
}

const Transform& BossShip::GetTransform(void) const
{
	return mTransform_;
}

bool BossShip::IsAlive(void) const
{
	return mState==STATE::BATTLE;
}


void BossShip::ChangeState(STATE state)
{
	//�����o�ϐ��ɕύX��������Ԃ������ĕێ�����
	mState = state;

	switch (mState)
	{
	case BossShip::STATE::EVENT:
	{
		mEventshot = new EventShot(&mTransform_);
		//�e���J�����Ɍ������Č���

		//�e��������(�P�ʃx�N�g��)
		auto cPos = SceneManager::GetInstance().GetCamera()->GetPos();
		auto bPos = mTransform_.pos;
		VECTOR dir = VNorm(VSub(cPos, bPos));
		mEventshot->Create(mTransform_.pos, dir);
	}
			break;
	case BossShip::STATE::BATTLE:
			break;
	case BossShip::STATE::DESTROY:
		mStepFade = TIME_FADE;
		mStepExplosion = 0.0f;
		break;
	}
}

void BossShip::MakeTurret(VECTOR localPos, VECTOR localAddAxis)
{
	Turret* turret = new Turret(
		&mTransform_,
		localPos,
		localAddAxis);

	mTurrets.emplace_back(turret);
}

void BossShip::CreateRandomExplosion(void)
{
	for (int i = 0; i < 5; i++)
	{
		float x =static_cast<float>(GetRand(360));
		float y =static_cast<float>(GetRand(360));
		float z =static_cast<float>(GetRand(360));

		x = AsoUtility::Deg2RadF(x);
		y = AsoUtility::Deg2RadF(y);
		z = AsoUtility::Deg2RadF(z);
		
		//���K�F�{�X���Ӎ��W��ȉ~�̂̉~�����W���g���č쐬���Ă݂܂��傤
		Quaternion tmpQ = Quaternion::Euler(x, y, z);
		//�����_���ȕ���(x,y,z)
		VECTOR dir = tmpQ.PosAxis(AsoUtility::DIR_F);

		//�����_���ȕ����ɑ΂��āA����(���̂̔��a�I��)����Z����B�x�N�g��*���a���Ƌ��̂ɂȂ�B
		//�J�v�Z���^�ɂ������̂Ȃ�Ax,y,z�̐��������ꂼ�ꕪ���Ă�����
		VECTOR pos{ 0.0f,0.0f,0.0f };
		pos = VAdd(pos, VScale({ dir.x,0.0f,0.0f }, EX_RADIUS_X));
		pos = VAdd(pos, VScale({ 0.0f,dir.y,0.0f }, EX_RADIUS_Y));
		pos = VAdd(pos, VScale({ 0.0f,0.0f,dir.z }, EX_RADIUS_Z));

		//�e�̉�]�ɍ��킹�āA�ʒu����]
		
		VECTOR rotLocalPos = mTransform_.quaRot.PosAxis(pos);
		
		//���΍��W�����΍��W�ɕϊ�
		pos = VAdd(mTransform_.pos, rotLocalPos);


		SpriteAnimator* ex = new SpriteAnimator(
			ResourceManager::SRC::SHIP_EXPLOSION, 200, 8.0f);
		ex->Create(pos, nullptr);
		mExplosions.push_back(ex);
	}
}

void BossShip::ZSortExplosion(void)
{

}


const std::vector<Turret*> BossShip::GetTurrets(void) const
{
	return std::vector<Turret*>();
}

bool BossShip::IsDestroy(void) const
{
	return mState== STATE::DESTROY;
}

bool BossShip::IsEnd(void) const
{
	return mState==STATE::END;
}

void BossShip::SyncParent(Transform& transform, VECTOR addAxis)
{
}
