#include<algorithm>
#include "GameScene.h"
#include "TitleScene.h"
#include"../Manager/SceneManager.h"
#include "../Object/PlayerShot.h"
#include "../Object/SkyDome.h"
#include "../../Resource.h"
#include "../Common/DxCushion.h"
#include"../../Application.h"


GameScene::GameScene() :BaseScene()
{
	mSkyDome = nullptr;
	bb = std::make_unique<BattleShip>();
	ec = std::make_unique<EnemyCannon>();
	em = std::make_unique<KinkyuNoTeki>();
	camera_ = std::make_unique<Camera>();
}

GameScene::~GameScene()
{
}

void GameScene::SetSelectedStageIndex(int stageIndex)
{
	selectedStageIndex_ = stageIndex;
}

void GameScene::SetSelectedShipIndex(int shipIndex)
{
	selectedShipIndex_ = shipIndex;
}

bool GameScene::IsHitSpheres(const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2)
{
	// ���̓��m�̏Փ˔���
	bool ret = false;

	// ���݂��̔��a�̍��v
	float radius = radius1 + radius2;

	// ���W�̍����炨�݂��̋��������
	VECTOR diff = VSub(pos2, pos1);

	// �O�����̒藝�Ŕ�r(SqrMagnitude�Ɠ���)
	float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
	if (dis < (radius * radius))
	{
		ret = true;
	}

	return ret;
}



void GameScene::Init(void)
{
	bb->Init();
	ec->Init();
	em->Init();
	// �X�J�C�h�[��
	mSkyDome = new SkyDome(&bb.get()->GetTransform());
	mSkyDome->Init();
	SceneManager::GetInstance().GetCamera()->SetPlayerPos(&bb.get()->GetTransform());
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);
	test = MV1LoadModel((Resource::PATH_MODEL+"Stage/plane4.mv1").c_str());
	//�v���C���[�̔j�󉉏o����
	mStepDestroy = 3.0f;
	time = 5000;
}

void GameScene::Update()
{
	mSkyDome->Update();
	bb->Update();
	ec->Update();
	em->Update();

	//�e�̍X�V
	auto shots = ec->GetShots();
	auto shotsTwo = em->GetShots();
	for (const auto s : shots)
	{
		s->Update();
		if (!s->IsAlive())
		{
			continue;
		}
		if (bb->IsAlive())
		{
			//��͂Ƃ̓����蔻��
			auto info = MV1CollCheck_Sphere(
				bb->GetTransform().modelId, -1,
				s->GetPos(), s->GetCollisionRadius());
			if (info.HitNum > 0)
			{
				continue;
			}
		
			MV1CollResultPolyDimTerminate(info);
		}

	}

	for (const auto s : shotsTwo)
	{
		s->Update();
		if (!s->IsAlive())
		{
			continue;
		}
		if (bb->IsAlive())
		{
			if (IsHitSpheres(s->GetPos(), s->GetCollisionRadius(),
				bb->GetPos(), bb->COLLISION_RADIUS))
			{
				bb->Damage();

				break;
			}

			
		}

	}

	//auto eshots = ec->GetShots();
	//for (auto s : spawn_)
	//{
	//	s->Update();
	//	for (auto e : eshots)
	//	{
	//		e->Update();

	//		if (!e->IsAlive())
	//		{
	//			continue;
	//		}

	//		if (!s->IsAlive())
	//		{
	//			continue;
	//		}

	//		if (IsHitSpheres(
	//			bb->GetTransform().pos,
	//			bb.get()->COLLISION_RADIUS,
	//			e->GetPos(), e->GetCollisionRadius()))
	//		{
	//			s->Damage();
	//			//e->ShotDead(&tank_->GetTransform());
	//			break;
	//		}

	//	}

	//	//MV1CollResultPolyDimTerminate(info);
	//}
	auto scene_ = SceneManager::GetInstance;
	mStepDestroy -= scene_().GetDeltaTime();

	time-= scene_().GetDeltaTime();

	if (time==0.0f)
	{
		scene_().ChangeScene(
			SceneManager::SCENE_ID::TITLE, true);
	}
}

void GameScene::Draw(void)
{
	
	mSkyDome->Draw();
	
	constexpr float LEN = 3000.0f;
	constexpr float TERM = 100.0f;
	const int NUM = static_cast<int>(LEN / TERM);


	// XYZ��{��
	VECTOR s;
	VECTOR e;

	for (int i = -NUM; i < NUM; i++)
	{
		float f = static_cast<float>(i);

		// X��
		s = VGet(-LEN, 0.0f, f * TERM);
		e = VGet(LEN, 0.0f, f * TERM);
		DrawLine3D(s, e, GetColor(255, 0, 0));  // �ԐF

		// Z��
		s = VGet(f * TERM, 0.0f, -LEN);
		e = VGet(f * TERM, 0.0f, LEN);
		DrawLine3D(s, e, GetColor(0, 0, 255));  // �F
	}

	// Y��
	s = VGet(0.0f, -LEN, 0.0f);
	e = VGet(0.0f, LEN, 0.0f);
	DrawLine3D(s, e, GetColor(0, 255, 0));  // �ΐF

	bb->Draw();
	ec->Draw();
	em->Draw();
	MV1DrawModel(test);


	auto eshots = ec->GetShots();
	for (const auto eshot : eshots)
	{
		eshot->Draw();
	}
	DrawFormatString(0, 300, 0xff0000, "%d", time);


}

void GameScene::Release(void)
{
	bb.reset();
	ec.reset();
	em.reset();
	mSkyDome->Release();
	delete mSkyDome;
}

