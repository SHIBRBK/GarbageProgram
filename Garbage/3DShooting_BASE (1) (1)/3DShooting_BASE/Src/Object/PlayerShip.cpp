#include"../Manager/ResourceManager.h"
#include<EffekseerForDXLIB.h>
#include<string>
#include"../Manager/Resource.h"
#include"../Manager/Scenemanager.h"
#include"../Utility/AsoUtility.h"
#include"ParticleGenerator.h"
#include "../Object/Ship/PlayerShot.h"
#include "PlayerShip.h"
#include"./Common/SpriteAnimator.h"
#include"./Common/SpeechBalloon.h"



PlayerShip::PlayerShip(void)
{
}

PlayerShip::~PlayerShip(void)
{
}

void PlayerShip::Init(void)
{
	//モデル制御に必要な基本情報
	mTransform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_SHIP));

	float SCALE = 0.04f;
	mTransform_.scl = { 0.04f,0.04f,0.04f };
	mTransform_.quaRot = Quaternion();
	//mTransform.quaRotLocal = Quaternion();
	mTransform_.quaRotLocal = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	mTransform_.pos = { 0.0f,0.0f,0.0f };
	mTransform_.Update();


	//エフェクトの初期化
	InitEffect();

	//吹き出し
	std::string msg = "追って！";
	SceneManager::SCENE_ID sceneId =
		SceneManager::GetInstance().GetmSceneID();

	if (sceneId == SceneManager::SCENE_ID::BATTLE)
	{
		msg = "倒せ！！";
	}
	mSpeechBalloon = new SpeechBalloon(
		SpeechBalloon::TYPE::SPEECH, &mTransform_
	);

	mSpeechBalloon->SetText(msg.c_str());
	mSpeechBalloon->SetTime(15.0f);
	mSpeechBalloon->SetRelativePos({ 15.0f,15.0f,0.0f });

	//パーティクルエフェクト
	mParticleGenreator = new ParticleGenerator(mTransform_.pos, 12.0f);
	mParticleGenreator->Init();


	//初期状態を走行状態にする
	ChangeState(STATE::RUN);


	mStepDelayBoost = 0.0f;
	mSpeedBoost = 0.0f;

}

void PlayerShip::InitEffect(void)
{
	mExplosion = new SpriteAnimator(
		ResourceManager::SRC::SHIP_EXPLOSION, 20.0f, 8.0f
	);


	mExplosion3D = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::EXPLOSION).mHandleId;

	mExplosion3DPlay = -1;


	mEfectJet = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::JET).mHandleId;

	mEfectBoost= ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOOST).mHandleId;



	mEfectJetPlayT=PlayEffekseer3DEffect(mEfectJet);
	mEfectJetPlayM=PlayEffekseer3DEffect(mEfectJet);
	mEfectJetPlayD=PlayEffekseer3DEffect(mEfectJet);
	float SCALE = 5.0f;
	SetScalePlayingEffekseer3DEffect(mEfectJetPlayT, SCALE, SCALE, SCALE);
	SetScalePlayingEffekseer3DEffect(mEfectJetPlayM, SCALE, SCALE, SCALE);
	SetScalePlayingEffekseer3DEffect(mEfectJetPlayD, SCALE, SCALE, SCALE);


	

}

void PlayerShip::Update(void)
{
	switch (mState)
	{
	case PlayerShip::STATE::RUN:
		UpdateRun();
		break;
	case PlayerShip::STATE::DESTROY:
		UpdateDestroy();
		break;
	}
}

void PlayerShip::UpdateRun(void)
{

	//回転処理
	ProcessTurn();

	//加速処理
	ProcessBoost();

	//弾の処理
	ProcessShot();

	//前方移動
	VECTOR forward = mTransform_.GetForward();
	//位置を更新
	mTransform_.pos = 
		VAdd(mTransform_.pos,
			VScale(forward, SPEED_MOVE + mSpeedBoost));
	//mTransform.pos.z += SPEED_MOVE;
	mTransform_.Update();


	//プレイヤーの位置とエフェクトの位置を同期
	VECTOR forwardPos = VAdd(mTransform_.pos, VScale(mTransform_.GetForward(), 30.0f));
	mParticleGenreator->SetPos(forwardPos);

	//プレイヤーの後ろ方向を向くように
	//Quaternion pBackRot=mTransform.quaRot;
	//pBackRot= pBackRot.Mult()
	//mParticleGenerator->SetRot();
	//mParticleGenerator->Update();

		//エフェクト制御
	SyncJetEffect();
	

}

void PlayerShip::UpdateDestroy(void)
{
	//A：自機が破壊せれたらカメラを止める



	//B：爆発エフェクトを自機の目の前に描画する



	//爆発エフェクト
	mExplosion->SetPos(mTransform_.pos);
	mExplosion->Update();

}
void PlayerShip::Draw(void)
{
	switch (mState)
	{
	case PlayerShip::STATE::RUN:
		MV1DrawModel(mTransform_.modelId);
		mParticleGenreator->Draw();
		break;
	case PlayerShip::STATE::DESTROY:
		//爆発エフェクト
		mExplosion->Draw();
		break;
	}

	//自機の当たり判定
	DrawSphere3D(
		mTransform_.pos, COLLISION_RADIUS, 5, 0xffffff, 0xffffff, false);

}

void PlayerShip::Release(void)
{
	mParticleGenreator->Release();
	delete mParticleGenreator;


	if (mExplosion3DPlay!=-1)
	{
		StopEffekseer3DEffect(mExplosion3DPlay);
	}

	StopEffekseer3DEffect(mEfectJetPlayT);
	StopEffekseer3DEffect(mEfectJetPlayM);
	StopEffekseer3DEffect(mEfectJetPlayD);
	StopEffekseer3DEffect(mEfectBoostPlay);

	
	mSpeechBalloon->Release();
	delete mSpeechBalloon;

	//弾の解放
	for (auto s : mShots)
	{
		s->Release();
		delete s;
	}
	mShots.clear();

}

void PlayerShip::ProcessTurn(void)
{
	//右旋回
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		Turn(SPEED_ROT_Y,AsoUtility::AXIS_Y);
	}
	//左旋回
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		Turn(-SPEED_ROT_Y, AsoUtility::AXIS_Y);
	}
	//上旋回
	if (CheckHitKey(KEY_INPUT_UP))
	{
		Turn(-SPEED_ROT_X, AsoUtility::AXIS_X);
	}
	//下旋回
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		Turn(SPEED_ROT_X, AsoUtility::AXIS_X);
	}

	mExplosion->Release();

}

void PlayerShip::Turn(double deg, VECTOR axis)
{
	//デグリー角をラジアン角に変換
	float rad = AsoUtility::Deg2RadF(deg);
	//指定軸（AXIS）に対する回転量（rad）を加えた回転（Quaternion）を作る
	Quaternion axisQ= Quaternion::AngleAxis(deg, axis);
	//自機の回転に、↑で作成した回転を加える
	mTransform_.quaRot = mTransform_.quaRot.Mult(axisQ);
}

void PlayerShip::ProcessBoost(void)
{
	
	mStepDelayBoost += SceneManager::GetInstance().GetDeltaTime();



	if (CheckHitKey(KEY_INPUT_B)&&mStepDelayBoost>TIME_DELAY_BOOST)
	{
		//ディレイ(クールタイム)制御
		mStepDelayBoost = 0.0f;
		//加速する
		mSpeedBoost = SPEED_BOOST;
		//エフェクト再生
		mEfectBoostPlay = PlayEffekseer3DEffect(mEfectBoost);
		//大きさ
		float SCALE = 10.0f;
		SetScalePlayingEffekseer3DEffect(mEfectBoostPlay, SCALE, SCALE, SCALE);


	}

	if (mStepDelayBoost<TIME_DELAY_BOOST&& mEfectBoostPlay>0)
	{
		//エフェクト制御
		SyncBoostEffect();
	}

	if (mSpeedBoost>0.0f)
	{
		mSpeedBoost -= (SUB_SPEED_BOOST * SceneManager::GetInstance().GetDeltaTime());
		if (mSpeedBoost < 0.0f)
		{
			mSpeedBoost = 0.0f;
		}
	}
}

void PlayerShip::ProcessShot(void)
{
	mStepDelayShot -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepDelayShot < 0.0f)
	{
		mStepDelayShot = 0.0f;
	}
	//キーチェック
	if (CheckHitKey(KEY_INPUT_N) && mStepDelayShot <= 0.0f)
	{
		//ディレイ時間をセット
		mStepDelayShot = TIME_DELAY_SHOT;

		//弾を生成
		CreateShot();

	}
}

const Transform& PlayerShip::GetTransform(void) const
{
	return mTransform_;
}

void PlayerShip::Destroy(void)
{
	ChangeState(STATE::DESTROY);

}

bool PlayerShip::IsDestroy(void)
{
	return mState == STATE::DESTROY;
}

SpeechBalloon* PlayerShip::GetSpeechBalloon(void) const
{
	return mSpeechBalloon;
}

std::vector<PlayerShot*>& PlayerShip::GetShots(void)
{
	return mShots;
}

void PlayerShip::ChangeState(STATE state)
{

	//状態変化（遷移）時の初期処理

	mState = state;
	switch (mState)
	{
	case PlayerShip::STATE::RUN:
		break;
	case PlayerShip::STATE::DESTROY:
	{
		//爆発エフェクトの生成
		mExplosion->Create(mTransform_.pos, nullptr);
		//Effekseerのエフェクト生成
		mExplosion3DPlay = PlayEffekseer3DEffect(mExplosion3D);
		VECTOR pos = mTransform_.pos;
		pos = VAdd(pos, VScale(mTransform_.GetForward(), 80.0f));
		//位置
		SetPosPlayingEffekseer3DEffect(mExplosion3DPlay, pos.x, pos.y, pos.z);
		//大きさ
		SetScalePlayingEffekseer3DEffect(mExplosion3DPlay, 10.0f, 10.0f, 10.0f);
		//回転
		SetRotationPlayingEffekseer3DEffect(mExplosion3DPlay, 0.0f, 0.0f, 0.0f);
	}
		break;
	}
}

void PlayerShip::SyncJetEffect(void)
{

	VECTOR pos;
	const float Z = -15.0f;
	const float TY = 8.0f;
	const float DY = -5.0f;
	//クォータニオンからオイラー角煮変換
	const VECTOR angles = Quaternion::ToEuler(mTransform_.quaRot);


	//方向から座標を作る
	//VECTOR localPos = VScale(mTransform.GetBack(), 15.0f);
	//pos = VAdd(mTransform.pos, localPos);
	//SetPosPlayingEffekseer3DEffect(mEfectJetPlayM, pos.x, pos.y, pos.z);

	//VECTOR localTOPPos = VScale(mTransform.GetBack(), 15.0f);
	//localTOPPos = VAdd(localTOPPos,VScale(mTransform.GetUp(),8.0f));
	//pos = VAdd(mTransform.pos, localPos);
	//SetPosPlayingEffekseer3DEffect(mEfectJetPlayT, pos.x, pos.y, pos.z);


		//相対座標を回転させる
#pragma region 真ん中噴射
	//相対座標を作る
	VECTOR localMPos = { 0.0f,0.0f,Z };
	//相対座標を回転させる
	localMPos = mTransform_.quaRot.PosAxis(localMPos);
	//相対座標からワールド座標に変換する
	pos = VAdd(mTransform_.pos, localMPos);
	//位置設定
	SetPosPlayingEffekseer3DEffect(mEfectJetPlayM, pos.x, pos.y, pos.z);
#pragma endregion
#pragma region 上噴射
	//相対座標を作る
	VECTOR localTPos = { 0.0f,8.0f,Z };
	//相対座標を回転させる
	localTPos = mTransform_.quaRot.PosAxis(localTPos);
	//相対座標からワールド座標に変換する
	pos = VAdd(mTransform_.pos, localTPos);
	//位置設定
	SetPosPlayingEffekseer3DEffect(mEfectJetPlayT, pos.x, pos.y, pos.z);
#pragma endregion
#pragma region 下噴射
	//相対座標を作る
	VECTOR localDPos = { 0.0f,-8.0f,Z };
	//相対座標を回転させる
	localDPos = mTransform_.quaRot.PosAxis(localDPos);
	//相対座標からワールド座標に変換する
	pos = VAdd(mTransform_.pos, localDPos);
	//位置設定
	SetPosPlayingEffekseer3DEffect(mEfectJetPlayD, pos.x, pos.y, pos.z);
#pragma endregion
	

	//回転
	SetRotationPlayingEffekseer3DEffect(mEfectJetPlayT, angles.x, angles.y, angles.z);
	SetRotationPlayingEffekseer3DEffect(mEfectJetPlayM, angles.x, angles.y, angles.z);
	SetRotationPlayingEffekseer3DEffect(mEfectJetPlayD, angles.x, angles.y, angles.z);


}

void PlayerShip::SyncBoostEffect(void)
{
	int Z = 300.0f;
	VECTOR pos;
	//オイラーに変換
	const VECTOR angles = Quaternion::ToEuler(mTransform_.quaRot);
	//回転
	SetRotationPlayingEffekseer3DEffect(mEfectBoostPlay, angles.x, angles.y, angles.z);

	//相対座標を作る
	VECTOR localPos = { 0.0f,0.0f,Z };
	//相対座標を回転させる
	localPos = mTransform_.quaRot.PosAxis(localPos);
	//相対座標からワールド座標に変換する
	pos = VAdd(mTransform_.pos, localPos);

	//位置
	SetPosPlayingEffekseer3DEffect(mEfectBoostPlay, pos.x, pos.y, pos.z);
	//大きさ
	//SetScalePlayingEffekseer3DEffect(mEfectBoostPlay, 10.0f, 10.0f, 10.0f);
	
}

void PlayerShip::CreateShot(void)
{
	
	bool isCreate = false;
	for (auto s : mShots)
	{
		if (!s->IsAlive())
		{
			//生存していなかったら
			//インスタンスを使いまわす
			s->Create(mTransform_.pos, mTransform_.GetForward());
			isCreate = true;
		}
	}

	if (!isCreate)
	{
		//インスタンスを新しく作る
		auto newShot = new PlayerShot(&mTransform_);
		newShot->Create(mTransform_.pos, mTransform_.GetForward());

		//可変長配列に追加
		mShots.emplace_back(newShot);
	}
}
