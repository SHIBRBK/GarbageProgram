#include<DxLib.h>
#include"../../Application.h"
#include"../../Manager/SceneManager.h"
#include"../../Manager/ResourceManager.h"
#include"../../Manager/Resource.h"
#include"../../Common/Quaternion.h"
#include"../../Utility/AsoUtility.h"
#include"../Common/Transform.h"
#include"../Common/SpriteAnimator.h"
#include"TurretShot.h"
#include"Turret.h"

Turret::Turret(
	const Transform* transformParent, 
	VECTOR localPos, VECTOR localAddAxis)
	:ShotBase(transformParent)//こ↑こ↓。要マークな。
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
	mTransformStand.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::TURRET_GUN));


	//砲台と砲身のそれぞれの移動回転
	mLocalAddAxisStand = VECTOR();
	mLocalAddAxisGun = VECTOR();

	//初期状態
	ChangeState(STATE::ATTACK);

}

Turret::~Turret(void)
{



}

void Turret::Init(void)
{
	//モデル制御の基本情報
	mTransformStand.scl = { SCALE,SCALE,SCALE };
	mTransformStand.quaRot = Quaternion();

	//ローカル回転
	mTransformStand.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	//親モデルとの同期(主に座標と回転)
	SyncParent(mTransformStand,{0.0f,0.0f,0.0f});





	//モデル制御の基本情報(砲身)
	mTransformGun.scl = { SCALE,SCALE,SCALE };
	mTransformGun.quaRot = Quaternion();

	//ローカル回転
	mTransformGun.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(-5.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)

	);

	//親モデルとの同期(主に座標と回転)
	SyncParent(mTransformGun, { 0.0f,0.0f,0.0f });

	//耐久力
	mHp = 2;


	//砲台と砲身の稼働量(rad)
	mAnglePowStand = ANGLE_POWER_STAND;
	mAnglePowGun = ANGLE_POWER_GUN;

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

	//砲台の稼働回転
	mLocalAddAxisStand.y += mAnglePowStand * delta;//Y軸がどんどん右回り


	//可動範囲の実装
	//砲台が終わったら砲身って感じ。

	float deg;
	deg = AsoUtility::Rad2DegF(mLocalAddAxisStand.y);
	if (deg<ANGLE_Y_MIN_STAND||deg>ANGLE_Y_MAX_STAND)
	{
		mAnglePowStand *= -1.0f;
	}


	//砲台親モデルとの同期させる
	SyncParent(mTransformStand,mLocalAddAxisStand);



	//砲身の稼働回転
	mLocalAddAxisGun.x += mAnglePowGun * delta;

	deg=AsoUtility::Rad2DegF(mLocalAddAxisGun.x);
	if (deg<ANGLE_X_MIN_GUN || deg>ANGLE_X_MAX_GUN)
	{
		mAnglePowGun *= -1.0f;
	}

	//砲台が回転した分、砲身も回転する必要があるので親子回転
	Quaternion stand = Quaternion::Euler(mLocalAddAxisStand);
	Quaternion gun = Quaternion::Euler(mLocalAddAxisGun);
	Quaternion mix = stand.Mult(gun);




	//砲身を親モデルと同期させる
	SyncParent(mTransformGun,mix.ToEuler());



	//①砲身6基から球が発射される
	//②砲身の先当たりから弾が発射される(位置調整)
	// 
	//③弾はShotBaseを継承する(しなくても良い)
	
	mExplosion->Update();
	//④クールタイムを設ける
	

	//⑤自機との当たり判定を設ける
	 
	//⑥自機と弾が衝突したら、バトルシーンの始めから。

	



	//弾の発射間隔
	mDeleyShot = delta;
	if (mDeleyShot < 0.0f)
	{
		ShotBase::Create(mLocalAddAxisGun,AsoUtility::DIR_F);
	}



	//弾の更新
	UpdateShot();

	//被ダメ判定
	if (mStepDamaged > 0.0f)
	{
		mStepDamaged -= delta;
	}
}

void Turret::UpdateDestroy(void)
{

	//弾の更新
	UpdateShot();

	//爆発エフェクトの更新
	mExplosion->Update();



}

void Turret::UpdateShot(void)
{
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
		MV1SetMaterialDifColor(
			mTransformStand.modelId,0,GetColorF(0.8f,0.1f,0.1f,0.8f));
		MV1SetMaterialDifColor(
			mTransformGun.modelId, 0, GetColorF(0.8f, 0.1f, 0.1f, 0.8f));
	}
	else
	{
		MV1SetMaterialDifColor(
			mTransformStand.modelId, 0, GetColorF(0.48f, 0.52f, 0.4f, 1.0f));
		MV1SetMaterialDifColor(
			mTransformGun.modelId, 0, GetColorF(0.2f, 0.4f, 0.3f, 1.0f));
	}


	MV1DrawModel(mTransformStand.modelId);
	MV1DrawModel(mTransformGun.modelId);

	//弾の描画
	DrawShot();

}

void Turret::DrawDestroy(void)
{
	//弾の描画
	DrawShot();

	//爆発エフェクトの描画
	mExplosion->Draw();


}

void Turret::DrawShot(void)
{
	for (const auto s : mShots)
	{
		s->Draw();
	}
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

	//爆発エフェクトの解放
	mExplosion->Release();
	delete mExplosion;
}

void Turret::SyncParent(Transform& transform,VECTOR addAxis)
{
	//ボス戦艦の3D情報
	Quaternion parentRot = mTransformParent->quaRot;

	//砲台、砲身のローカル回転をQuaternionで作る。
	//Unity Z->X->Y=Y*X*Z

	Quaternion localRot;
	Quaternion axis;

	//Y
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.y,AsoUtility::AXIS_Y);
	localRot=localRot.Mult(axis);


	//X
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.x, AsoUtility::AXIS_X);
	localRot = localRot.Mult(axis);

	//Z
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.z, AsoUtility::AXIS_Z);
	localRot = localRot.Mult(axis);




	//自立稼働分の回転をさらに加える。


		//アナザーY
	axis = Quaternion::AngleAxis(
		addAxis.y, AsoUtility::AXIS_Y);
	localRot = localRot.Mult(axis);


	//アナザーX
	axis = Quaternion::AngleAxis(
		addAxis.x, AsoUtility::AXIS_X);
	localRot = localRot.Mult(axis);

	//アナザーZ
	axis = Quaternion::AngleAxis(
		addAxis.z, AsoUtility::AXIS_Z);
	localRot = localRot.Mult(axis);










	//親の回転に沿って砲台・砲身を回転させる。
	transform.quaRot = localRot.Mult(localRot);


	//親の回転情報に合わせた位置を計算
	VECTOR localPos = Quaternion::PosAxis(parentRot,mLocalPos);

	//今まで習ったのやり方
	////相対座標を絶対座標に変換
	//transform.pos = VAdd(
	//	mTransformParent->pos, localPos);


	//親の大きさ分、スケールした相対座標を絶対座標に変換
	transform.pos = VAdd(
		mTransformParent->pos, VScale(localPos,SCALE));//スケールで掛け算してあげるのが本来のやり方



		
		//モデル制御の基本情報更新
	transform.Update();


}

std::vector<TurretShot*>& Turret::GetShot(void)
{
	return mShots;
}

bool Turret::IsAlive(void) const
{
	return mState==STATE::ATTACK;
}

VECTOR Turret::GetPos(void) const
{
	return mTransformStand.pos;
}

void Turret::Damaged(void)
{
	mStepDamaged = TIME_DAMAGED_EFFECT;
	mHp -= 1;
	if (mHp<=0) 
	{
		ChangeState(STATE::DESTROY);
	}
}



void Turret::ChangeState(STATE state)
{
	//状態を更新
	mState = state;
	//状態遷移時初期化
	switch (mState)
	{

	case Turret::STATE::ATTACK:
		break;
	case Turret::STATE::DESTROY:
	{
		mExplosion->Create(mTransformGun.pos, nullptr);
	}
		break;

	}


}


float Turret::GetSpeed(void) const
{
	return 20.0f;
}

float Turret::GetTimeAlive(void) const
{
	return 10.0f;
}

float Turret::GetCollisionRadius(void) const
{
	return mCollisionRadius;
}
