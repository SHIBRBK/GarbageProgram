#include<string>
#include<EffekseerForDXLib.h>
#include"../../Manager/ResourceManager.h"
#include"../../Manager/Resource.h"
#include"../../Manager/SceneManager.h"
#include"../../Utility/AsoUtility.h"
#include"./../Common/SpriteAnimator.h"
#include"./../Common/SpeechBalloon.h"
#include"../Common/SpeechBalloon.h"
#include"../../Object/Ship/PlayerShot.h"
#include"../ParticleGenerator.h"
#include "PlayerShip.h"



PlayerShip::PlayerShip(void)
{
}

PlayerShip::~PlayerShip(void)
{
}

void PlayerShip::Init(void)
{
	//モデル制御の基本情報
	mTransform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_SHIP));

	float SCALE = 10.0f;
	mTransform_.scl = { SCALE,SCALE,SCALE };
	mTransform_.quaRot = Quaternion();

	//mTransform_.quaRotLocal=Quaternion();

	mTransform_.quaRotLocal= Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);

	mTransform_.pos = { 0.0f,0.0f,0.0f };
	mTransform_.Update();

	//パーティクルエフェクト
	mParticleGenerator = new ParticleGenerator(mTransform_.pos, 10.0f);
	mParticleGenerator->Init();

	//エフェクトの初期化
	InitEffect();


	//吹き出し
	std::string msg = "追って！";

	SceneManager::SCENE_ID sceneID =
		SceneManager::GetInstance().GetmSceneID();

	if (sceneID == SceneManager::SCENE_ID::BATTLE)
	{
		msg = "倒して";
	}


	mSpeechBalloon = new SpeechBalloon(
		SpeechBalloon::TYPE::SPEECH, &mTransform_);


	mSpeechBalloon->SetText(msg.c_str());
	mSpeechBalloon->SetTime(15.0f);
	mSpeechBalloon->SetRelativePos({ 15.0f,15.0f,0.0f });

	//初期状態を走行状態にする。
	ChangeState(STATE::RUN);

	//ブースト関連
	mStepDelayBoost = 0.0f;
	mSpeedBoost = 0.0f;
	mEffectBoostPlay = -1;


	

}

void PlayerShip::InitEffect(void)
{
	mEffectBoost = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOOST).mHandleId;

	//自機の爆発エフェクト
	mExplosion = new SpriteAnimator(
		ResourceManager::SRC::SHIP_EXPLOSION, 20.0f, 8.0f
	);

	mExplosion3D = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::EXPLOSION).mHandleId;

	mExplosion3DPlay = -1;


	mEffectJet = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::JET).mHandleId;


	mEffectJetPlayT = PlayEffekseer3DEffect(mEffectJet);
	mEffectJetPlayM = PlayEffekseer3DEffect(mEffectJet);
	mEffectJetPlayD = PlayEffekseer3DEffect(mEffectJet);
	float SCALE = 5.0f;

	SetScalePlayingEffekseer3DEffect(mEffectJetPlayT, SCALE, SCALE, SCALE);
	SetScalePlayingEffekseer3DEffect(mEffectJetPlayM, SCALE, SCALE, SCALE);
	SetScalePlayingEffekseer3DEffect(mEffectJetPlayD, SCALE, SCALE, SCALE);

	//エフェクト制御
	SyncJetEffect();
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


	//ChangeState(STATE::RUN);
}

void PlayerShip::UpdateRun(void)
{
	//回転操作
	ProcessTurn();

	//加速処理
	ProcessBoost();

	//弾の処理
	ProcessShot();

	//前方移動
	VECTOR forward = mTransform_.GetForward();
	mTransform_.pos =
		VAdd(mTransform_.pos,
			VScale(forward,
				SPEED_MOVE+mSpeedBoost)
		);

	mTransform_.Update();



	//プレイヤーの位置とエフェクトの位置を同期
	//前に配置。
	VECTOR forwardPos = VAdd(mTransform_.pos, VScale(mTransform_.GetForward(), 30.0f));
	mParticleGenerator->SetPos(forwardPos);

	//プレイヤーの後ろ方向を向くように
	Quaternion pBackRot = mTransform_.quaRot;
	pBackRot = pBackRot.Mult(
		Quaternion::AngleAxis(AsoUtility::Deg2RadD(-90.0), AsoUtility::AXIS_X));



	mParticleGenerator->SetRot(pBackRot);
	mParticleGenerator->SetPos(forwardPos);


	mParticleGenerator->Update();

	//エフェクト制御
	SyncJetEffect();


}

void PlayerShip::UpdateDestroy(void)
{
	//A:自機が破壊されたらカメラを止める
	//if (IsDestroy)
		//B:爆発エフェクトを自機の手前に描画するようにする

	//爆発エフェクト

	VECTOR pos = VAdd(pos, VScale(mTransform_.GetForward(), SPEED_MOVE / 2.0f));
	//mExplosion->SetPos(mTransform_.pos);
	mExplosion->SetPos(mTransform_.pos);
	mExplosion->Update();
}


void PlayerShip::Draw(void)
{
	switch (mState)
	{

	case PlayerShip::STATE::RUN:
 		MV1DrawModel(mTransform_.modelId);

		mParticleGenerator->Draw();
		break;
	case PlayerShip::STATE::DESTROY:
		mExplosion->Draw();
		break;

	}

//自機の当たり判定
	//DrawSphere3D(
	// mTransform_.pos,COLLISION_RADIUS,5,0xffffff,0xffffff,false);
}


void PlayerShip::Release(void)
{

	mParticleGenerator->Release();
	delete mParticleGenerator;

	mExplosion->Release();
	delete mExplosion;

	if (mExplosion3D != 1)
	{
		StopEffekseer3DEffect(mExplosion3D);
	}
	StopEffekseer3DEffect(mEffectJetPlayT);
	StopEffekseer3DEffect(mEffectJetPlayM);
	StopEffekseer3DEffect(mEffectJetPlayD);
	StopEffekseer3DEffect(mEffectBoostPlay);

	mSpeechBalloon->Release();
	delete mSpeechBalloon;

	//弾の開放
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
		Turn(SPEED_ROT_DEG_Y, AsoUtility::AXIS_Y);
	}

	//左旋回
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		Turn(-SPEED_ROT_DEG_Y, AsoUtility::AXIS_Y);
	}

	//上旋回
	if (CheckHitKey(KEY_INPUT_UP))
	{
		Turn(-SPEED_ROT_DEG_X, AsoUtility::AXIS_X);
	}

	//下旋回
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		Turn(SPEED_ROT_DEG_X, AsoUtility::AXIS_X);
	}

}

void PlayerShip::Turn(double deg, VECTOR axis)
{
	//デグリー角をラジアンに変換
	float rad = AsoUtility::Deg2RadF(deg);
	//指定軸(axis)に対する回転量(rad)に加えた回転(Quaternion)を作る
	Quaternion axisQ=Quaternion::AngleAxis(rad,axis);

	//自機の回転に。↑で作成した回転を加える。
	mTransform_.quaRot = mTransform_.quaRot.Mult(axisQ);


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
	return mState==STATE::DESTROY;
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

	//状態遷移血の初期処理

	mState = state;
	switch (mState)
	{

	case PlayerShip::STATE::RUN:
		break;

	case PlayerShip::STATE::DESTROY:
	{
		mExplosion->Create(mTransform_.pos, nullptr);


		mExplosion3DPlay = PlayEffekseer3DEffect(mExplosion3D);
		VECTOR pos = mTransform_.pos;
		
		//ここ帰ってすぐ消して自分で考えよ・・・
		pos = VAdd(pos,VScale(mTransform_.GetForward(), 100.0f));
		
			
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

void PlayerShip::SyncJetEffect()
{
	VECTOR pos;
	const float Z = -15.0f;
	const float TY = 8.0f;
	const float DY = -5.0f;


	//クォータニオンからオイラーに変換
	const VECTOR angles = Quaternion::ToEuler(mTransform_.quaRot);
	//const VECTOR angles2 = mTransform_.quaRot.ToEuler();//こっちでもいい

	//ここ帰ってすぐ消して自分で考えよ・・・
	//相対座標を回転させる

	
	//方向から座標を作る
	
	
	//VECTOR localPos = VScale(mTransform_.GetBack(), 15.0f);


	////方向から座標を作る
	////const VECTOR localMPos = { 0.0f,0.0f,Z };
	//VECTOR localPos = VScale(mTransform_.GetBack(), 15.0f);
	//pos = VAdd(mTransform_.pos, localPos);
	//SetPosPlayingEffekseer3DEffect(mEffectJetL, pos.x, pos.y, pos.z);


	//VECTOR localLeftPos = VScale(mTransform_.GetBack(), 15.0f);
	//localLeftPos = VAdd(localLeftPos, VScale(mTransform_.GetLeft(),8.0f));
	//pos = VAdd(mTransform_.pos, localLeftPos);
	//SetPosPlayingEffekseer3DEffect(mEffectJetL, pos.x, pos.y, pos.z);

#pragma region 真ん中エフェクト


	//相対座標を作る
	VECTOR localMPos{ 0.0f,0.0f,-15.0f };
	//相対座標を回転させる
	localMPos = mTransform_.quaRot.PosAxis(localMPos);
	//相対座標からワールド座標に変換する
	pos = VAdd(mTransform_.pos, localMPos);
	//pos = VAdd(mTransform_.pos,localLPos );
	//エフェクトの位置設定
	SetPosPlayingEffekseer3DEffect(mEffectJetPlayM, pos.x, pos.y, pos.z);
	
#pragma endregion


#pragma region 上エフェクト
	//相対座標を作る
	VECTOR localTPos{ 8.0f,0.0f,-15.0f };
	//相対座標を回転させる
	localTPos = mTransform_.quaRot.PosAxis(localTPos);
	//相対座標からワールド座標に変換する
	pos = VAdd(mTransform_.pos, localTPos);
	//エフェクトの位置設定
	SetPosPlayingEffekseer3DEffect(mEffectJetPlayT, pos.x, pos.y, pos.z);

#pragma endregion

#pragma region 下エフェクト
	//相対座標を作る
	VECTOR localDPos{ 0.0f,-5.0f,-15.0f };
	//相対座標を回転させる
	localDPos = mTransform_.quaRot.PosAxis(localDPos);
	//相対座標からワールド座標に変換する
	pos = VAdd(mTransform_.pos, localDPos);
	//エフェクトの位置設定
	SetPosPlayingEffekseer3DEffect(mEffectJetPlayD, pos.x, pos.y, pos.z);

#pragma endregion



	//エフェクトの回転を設定する。
	SetRotationPlayingEffekseer3DEffect(
		mEffectJetPlayM, angles.x, angles.y ,angles.z
	);

	SetRotationPlayingEffekseer3DEffect(
		mEffectJetPlayT, angles.x, angles.y, angles.z
	);

	SetRotationPlayingEffekseer3DEffect(
		mEffectJetPlayD, angles.x, angles.y, angles.z
	);
		
	
}

void PlayerShip::SyncBoostEffect()
{
	const VECTOR angles = Quaternion::ToEuler(mTransform_.quaRot);

	SetRotationPlayingEffekseer3DEffect(
		mEffectBoostPlay, angles.x,angles.y, angles.z);

#pragma region 位置同期
	VECTOR localPos{ 0.0f,0.0f,100.0f };
	localPos = mTransform_.quaRot.PosAxis(localPos);
	VECTOR pos = VAdd(mTransform_.pos, localPos);

	SetPosPlayingEffekseer3DEffect(
		mEffectBoostPlay, pos.x, pos.y, pos.z
	);


#pragma endregion
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
		mEffectBoost =PlayEffekseer3DEffect(mEffectBoost);
		//大きさ
		float SCALE = 10.0f;
		SetScalePlayingEffekseer3DEffect(mEffectBoostPlay, SCALE, SCALE, SCALE);

		//エフェクト制御
		SyncBoostEffect();
	}

	if (mStepDelayBoost < TIME_DELAY_BOOST&&mEffectBoostPlay>0)
	{
		SyncBoostEffect();
	}
	//減速処理
	if (mSpeedBoost > 0.0f) 
	{
		mSpeedBoost -=
			(SUB_SPEED_BOOST * SceneManager::GetInstance().GetDeltaTime());

		if (mSpeedBoost < 0.0f) 
		{
			mSpeedBoost = 0.0f;
		}
	}
}

void PlayerShip::ProcessShot(void)
{
	//続きはここから
	//弾の生成ディレイ
	mStepDelayshot -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepDelayshot < 0.0f)
	{
		mStepDelayshot = 0.0f;
	}

	//キーチェック
	if (CheckHitKey(KEY_INPUT_N)&& mStepDelayshot<=0.0f)

	{
		//ディレイ時間をセット
		mStepDelayshot = TIME_DELAY_SHOT;

		//弾を生成
		CreateShot();
	}

	
}

void PlayerShip::CreateShot(void)
{
	bool isCreate = false;
	for (auto s : mShots)
	{
		if (!s->IsAlive())
		{
			s->Create(mTransform_.pos,mTransform_.GetForward());
			isCreate = true;
		}
	}
	if (!isCreate)
	{
		auto newShot = new PlayerShot(&mTransform_);
		newShot->Create(mTransform_.pos, mTransform_.GetForward());

		//可変長配列に追加
		mShots.emplace_back(newShot);
	}
}
