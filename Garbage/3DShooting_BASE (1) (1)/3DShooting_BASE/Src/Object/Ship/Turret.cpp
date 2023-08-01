#include<DxLib.h>
#include"../../Application.h"
#include"../../Manager/SceneManager.h"
#include"../../Manager/Resource.h"
#include"../../Manager/ResourceManager.h"
#include"../../Common/Quaternion.h"
#include"../../Utility/AsoUtility.h"
#include"../Common/Transform.h"
#include"../Common/SpriteAnimator.h"
#include"../../Object/Ship/TurretShot2.h"
#include "Turret.h"

Turret::Turret(const Transform* transformParent, VECTOR localPos, VECTOR localAddAxis):ShotBase(transformParent)
{
	//親
	mTransformParent = transformParent;
	//相対座標
	mLocalPos = localPos;
	//相対回転
	mLocalAddAxis = localAddAxis;
	//砲台
	mTransformStand.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::TURRET_STAND));
	//砲身
	mTransformGun.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::TURRET_GUN));

	//砲台と砲身のそれぞれの移動回転
	mLocalAddAxisStand	= VECTOR();
	mLocalAddAxisGun	= VECTOR();

	ChangeState(STATE::ATTACK);



}

Turret::~Turret(void)
{
}

void Turret::Init(void)
{
	//モデル制御の基本情報（砲台）
	mTransformStand.scl = { SCALE,SCALE,SCALE };
	mTransformStand.quaRot = Quaternion();

	//ローカル
	mTransformStand.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	//親とのモデルとの同期（主に座標と回転）
	SyncParent(mTransformStand,{0.0f,0.0f,0.0f});

	//モデル制御の基本情報（砲台）
	mTransformGun.scl = { SCALE,SCALE,SCALE };
	mTransformGun.quaRot = Quaternion();

	//ローカル
	mTransformGun.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(-5.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	//親とのモデルとの同期（主に座標と回転）
	SyncParent(mTransformGun,{ 0.0f,0.0f,0.0f });
	

	//耐久力
	mHp = 2;

	//砲台ローカル稼働量(rad)
	mAnglePowStand = ANGLE_POW_STAND;

	//砲身ローカル稼働量(rad)
	mAnglePowGun = ANGLE_POW_GUN;

	//爆発エフェクト
	mExplosion = new SpriteAnimator(
		ResourceManager::SRC::SHIP_EXPLOSION, 120.0f, 8.0f);

	mStepDamaged = -1;

}

void Turret::Update(void)
{

	switch (mState)
	{

	case Turret::STATE::ATTACK:
		UpdateAttack();
		break;
	case Turret::STATE::DESTROY:
		UpdateDestroy();
		break;
	}


}

void Turret::UpdateAttack(void)
{

	auto delta = SceneManager::GetInstance().GetDeltaTime();

	float deg;

	//砲台の稼働回転
	mLocalAddAxisStand.y += mAnglePowStand * delta;

	//砲台の可動範囲の実装

	deg = AsoUtility::Rad2DegF(mLocalAddAxisStand.y);
	if (mLocalAddAxisStand.y < ANGLE_Y_MIN_STAND || mLocalAddAxisStand.y > ANGLE_Y_MAX_STAND)
	{
		mAnglePowStand *= -1.0f;
	}



	//砲台を親とのモデルとの同期（主に座標と回転）
	SyncParent(mTransformStand,mLocalAddAxisStand);

	//砲身の稼働回転
	mLocalAddAxisGun.x += mAnglePowGun * delta;

	deg = AsoUtility::Rad2DegF(mLocalAddAxisGun.x);
	if (mLocalAddAxisGun.x < ANGLE_Y_MIN_GUN || mLocalAddAxisGun.x > ANGLE_Y_MAX_GUN)
	{
		mAnglePowGun *= -1.0f;
	}

	//砲台が回転した分、砲身も回転する必要があるので親子回転
	Quaternion stand = Quaternion::Euler(mLocalAddAxisStand);
	Quaternion gun = Quaternion::Euler(mLocalAddAxisGun);
	Quaternion mix = stand.Mult(gun);



	//砲身を親とのモデルとの同期（主に座標と回転）
	SyncParent(mTransformGun, mix.ToEuler());


	// 演習：タレットの弾(他のショットクラスで一度やっている)
	// ①砲身6基から弾が発射される
	// ②砲身の先あたりから弾が発射される
	// ③弾はShotBaseを継承する(しなくてもよい)
	mExplosion->Update();
	// ④クールタイムを設ける
	// ⑤自機との当たり判定を設ける
	// ⑥自機と弾が衝突したら、バトルシーンの始めから
	for (auto s : mShots)
	{
		s->Update();
	}

	mDeleyShot = delta;
	if (mDeleyShot < 0.0f)
	{
		ShotBase::Create(mLocalAddAxisGun, AsoUtility::DIR_F);
	}

	//被ダメ
	if (mStepDamaged > 0.0f)
	{
		mStepDamaged -= delta;
	}


	//弾の処理
	ProcessShot();
}

void Turret::UpdateDestroy(void)
{

	// 弾の更新
	ProcessShot();

	//爆発エフェクトの更新
	mExplosion->Update();
}

void Turret::Draw(void)
{
	switch (mState)
	{

	case Turret::STATE::ATTACK:
		DrawAttack();
		break;
	case Turret::STATE::DESTROY:
		DrawDestroy();
		break;
	}
}

void Turret::DrawAttack(void)
{

	if (mStepDamaged > 0.0f)
	{
		MV1SetMaterialDifColor(mTransformStand.modelId,0,GetColorF(0.8f,0.1f,0.1f,0.8f));
		MV1SetMaterialDifColor(mTransformGun.modelId,0,GetColorF(0.8f,0.1f,0.1f,0.8f));
	}
	else
	{
		MV1SetMaterialDifColor(mTransformStand.modelId, 0, GetColorF(0.48f, 0.52f, 0.4f, 1.0f));
		MV1SetMaterialDifColor(mTransformGun.modelId, 0, GetColorF(0.2f, 0.4f, 0.3f, 1.0f));
	}


	MV1DrawModel(mTransformStand.modelId);
	MV1DrawModel(mTransformGun.modelId);

	for (auto s : mShots)
	{
		s->Draw();
	}
}

void Turret::DrawDestroy(void)
{

	//弾の描画
	for (auto s : mShots)
	{
		s->Draw();
	}

	//爆発
	mExplosion->Draw();

}

void Turret::Release(void)
{	
	//弾の解放
	for (auto s : mShots)
	{
		s->Release();
		delete s;
	}
	mShots.clear();

	mExplosion->Release();
	delete mExplosion;

}

void Turret::SyncParent(Transform& transform,VECTOR addAxis)
{
	//ボス戦艦の３D情報を取得
	Quaternion parentRot = mTransformParent->quaRot;
	//砲台・砲身のローカル回転
	//Unity Z→X→Y = Y*X*Z
	Quaternion localRot;
	Quaternion axis;

	// Y
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.y, AsoUtility::AXIS_Y);
	localRot = localRot.Mult(axis);

	// X
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.x, AsoUtility::AXIS_X);
	localRot = localRot.Mult(axis);

	// Z
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.z, AsoUtility::AXIS_Z);
	localRot = localRot.Mult(axis);

	//自立稼働分の回転をさらに加える
	// Y
	axis = Quaternion::AngleAxis(
		addAxis.y, AsoUtility::AXIS_Y);
	localRot = localRot.Mult(axis);

	// X
	axis = Quaternion::AngleAxis(
		addAxis.x, AsoUtility::AXIS_X);
	localRot = localRot.Mult(axis);

	// Z
	axis = Quaternion::AngleAxis(
		addAxis.z, AsoUtility::AXIS_Z);
	localRot = localRot.Mult(axis);



	//親の回転に沿った砲台・砲身
	transform.quaRot =parentRot.Mult(localRot);

	//親の回転情報に合わせた位置を計算
	VECTOR localPos = Quaternion::PosAxis(parentRot, mLocalPos);

	//今までのやり方
	//相対座標を絶対座標に変換
	//transform.pos = VAdd(mTransformParent->pos, localPos);

	//相対座標を絶対座標に変換
	transform.pos = VAdd(mTransformParent->pos, VScale(localPos, SCALE));

	//モデル制御の基本情報更新
	transform.Update();

}

void Turret::ProcessShot(void)
{
	mStepDelayShot -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepDelayShot < 0.0f)
	{
		mStepDelayShot = 0.0f;
	}
	//キーチェック
	if (mStepDelayShot <= 0.0f)
	{
		//ディレイ時間をセット
		mStepDelayShot = TIME_DELAY_SHOT;

		//弾を生成
		CreateShot();

	}
}

std::vector<TurretShot2*>& Turret::GetShots(void)
{
	return mShots;
}

bool Turret::IsAlive(void) const
{
	return mState == STATE::ATTACK;
}

VECTOR Turret::GetPos(void) const
{
	return mTransformStand.pos;
}

void Turret::Damage(void)
{

	mStepDamaged = TIME_DAMAGED_EFFECT;

	mHp -= 1;

	if (mHp <= 0)
	{
		ChangeState(STATE::DESTROY);
	}

}

void Turret::ChangeState(STATE state)
{
	 
}

void Turret::CreateShot(void)
{
	bool isCreate = false;
	for (auto s : mShots)
	{
		if (!s->IsAlive())
		{
			//生存していなかったら
			//インスタンスを使いまわす
			s->Create(mTransformGun.pos, mTransformGun.GetForward());
			isCreate = true;
		}
	}

	if (!isCreate)
	{
		//インスタンスを新しく作る
		auto newShot = new TurretShot2(&mTransformGun);
		newShot->Create(mTransformGun.pos, mTransformGun.GetForward());

		//可変長配列に追加
		mShots.emplace_back(newShot);
	}
}
