#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Common/Transform.h"
#include "SceneManager.h"
#include "Camera.h"

Camera::Camera(void)
{

	mPos = VECTOR();
	mQuaRot = Quaternion();
	mTargetPos = VECTOR();
	mCameraUp = VECTOR();

}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{

	ChangeMode(MODE::FOLLOW_SPRING);

	
}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	mPos = DEFAULT_CAMERA_POS;
	mTargetPos = VAdd(mPos, RELATIVE_TARGET_POS);
	mCameraUp = { 0.0f, 1.0f, 0.0f };

	// カメラはX軸に傾いているが、この傾いた状態を傾き無しとする
	// mQuaRotは回転計算用で、
	// あくまで軸となるのは、カメラ座標と注視点とする
	mQuaRot = Quaternion();

}

void Camera::Update()
{
}

void Camera::SetBeforeDraw(void)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mMode)
	{
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::FOLLOW_SPRING:
		SetBeforeDrawFollowSpring();
		break;
	}


	// カメラの設定
	SetCameraPositionAndTargetAndUpVec(
		mPos,
		mTargetPos,
		mCameraUp
	);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
	
}

void Camera::SetBeforeDrawFree(void)
{

	//回転処理
	VECTOR axis = AsoUtility::VECTOR_ZERO;
	float rev = 1.0f;
	if (CheckHitKey(KEY_INPUT_UP)) { axis = AsoUtility::AXIS_X; rev = -1.0f; }
	if (CheckHitKey(KEY_INPUT_DOWN)) { axis = AsoUtility::AXIS_X; }
	if (CheckHitKey(KEY_INPUT_LEFT)) { axis = AsoUtility::AXIS_Y; rev = -1.0f; }
	if (CheckHitKey(KEY_INPUT_RIGHT)) { axis = AsoUtility::AXIS_Y; }
	//カメラ座標を中心として、注視点をかいてんさせる
	if (!AsoUtility::EqualsVZero(axis))
	{
		Quaternion axisQ = Quaternion::AngleAxis(AsoUtility::Deg2RadF(1.0f*rev), axis);
		mQuaRot = mQuaRot.Mult(axisQ);

		//位置情報に回転情報を反映
		VECTOR localPos = mQuaRot.PosAxis(RELATIVE_TARGET_POS);

		//注視点更新
		mTargetPos = VAdd(mPos,localPos);

		//カメラの上方向更新
		mCameraUp = mQuaRot.GetUp();//普通のゲームならY軸で固定してもいいけど、３Dシューティングはこれじゃなきゃだめ
	}


	//カメラ操作ができるように
	//移動処理
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (CheckHitKey(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (CheckHitKey(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (CheckHitKey(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (CheckHitKey(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }


	if (!AsoUtility::EqualsVZero(moveDir))
	{
		float speed = 20.0f;

		//VTransformのクォータニオンバージョン
		//lpos'=q・lpos・q(-1)
		//Quaternion tmp;
		//tmp = tmp.Mult(mQuaRot);
		//tmp = tmp.Mult(Quaternion(0.0f,moveDir.x,moveDir.y,moveDir.z));
		//tmp = tmp.Mult(mQuaRot.Inverse());

		//移動方向をカメラ向きに合わせて回転
		VECTOR rotDir = mQuaRot.PosAxis(moveDir);

		//移動させたい方向に移動量を加える
		VECTOR movePow = VScale(rotDir,speed);

		//カメラ位置と注視点を移動する
		mPos = VAdd(mPos, movePow);
		mTargetPos = VAdd(mTargetPos, movePow);

	}
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//定点カメラなので何もしない

}

void Camera::SetBeforeDrawFollow(void)
{
	VECTOR targetPos = mTarget->pos;
	//MATRIX targetMat= mTarget->matRot;
	//Quaternion targetRot = mTarget->QuaRot;
	VECTOR relativeCPos = VTransform({ 0.0f,25.0f,-80.0f }, mTarget->matRot);
	mPos = VAdd(targetPos, relativeCPos);

	VECTOR relativeTPos = VTransform(RELATIVE_TARGET_POS, mTarget->matRot);
	mTargetPos = VAdd(mPos, relativeTPos);
}

void Camera::SetBeforeDrawFollowSpring(void)
{

	// 自機情報
	VECTOR shipPos = mTarget->pos;
	MATRIX shipMat = mTarget->matRot;
	Quaternion shipRot = mTarget->quaRot;

	// カメラ座標
	VECTOR relativeCPos = shipRot.PosAxis({ 0.0f, 40.0f, 150.0f });

	// ばね定数
	float POW_SPRING = 24.0f;
	float dampening = 2.0f * sqrtf(POW_SPRING);

	// デルタタイムの話から
	//float delta = SceneManager::GetInstance().GetDeltaTime();
	float delta = 1.0f / 60.0f;

	// 理想の位置
	VECTOR idealPos = VAdd(shipPos, relativeCPos);

	// 理想と実際の位置の差
	VECTOR diff = VSub(mPos, idealPos);

	// 力 = -バネの強さ × バネの伸び - 抵抗 × カメラの速度
	VECTOR force = VScale(diff, -POW_SPRING);
	force = VSub(force, VScale(mVelocity, dampening));

	// 速度の更新
	mVelocity = VAdd(mVelocity, VScale(force, delta));

	// カメラ位置の更新
	mPos = VAdd(mPos, VScale(mVelocity, delta));

	// 注視点
	VECTOR localTPos = shipRot.PosAxis(RELATIVE_TARGET_POS);
	mTargetPos = VAdd(mPos, localTPos);

	// カメラの上方向
	mCameraUp = mTarget->GetUp();

}

void Camera::SetBeforeDrawShake(void)
{

	mStepShake -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepShake < 0.0f)
	{
		//終了処理
		mPos = mDefaultPos;
		//固定カメラに戻す
		ChangeMode(MODE::FIXED_POINT);
		return;
	}

	//カメラ揺らし処理（緩やか）
	//波の力 = 波の高さ＊sin（時間＊波の速度）
	//float movePow = WIDTH_SHAKE*sinf(mStepShake*SPEED_SHAKE);//サイン波を使う
	//VECTOR velocity = VScale(mShakeDir, movePow);
	//VECTOR newPos = VAdd(mDefaultPos, velocity);
	//mPos = newPos;
	//sinは-1.0から、1.0までの数字を緩やかに作る
	//↓で作るのは、-1.0か1.0の二種類のみ

	//カメラ揺らし処理（激しい）
	//0 or 1 -> -1 or 1
	
	int cnt = static_cast<int>(mStepShake * SPEED_SHAKE2);
	// 0 or 1
	int shake = cnt % 2;
	//0 or 2
	shake*=2;
	// -1 or 1
	/*if (shake == 0)
	{
		shake -= 1;
	}*/
	shake -= 1;
	float movePow2 = WIDTH_SHAKE * shake;
	VECTOR velocity = VScale(mShakeDir, movePow2);
	VECTOR newPos = VAdd(mDefaultPos, velocity);
	mPos = newPos;
}


void Camera::Draw()
{
}

void Camera::DrawUI(void)
{

	// 操作説明
	int x = 200;
	int y = 10;
	int addY = 40;
	DrawFormatString(
		Application::SCREEN_SIZE_X - x, y, 0xffffff, "　【操作】");
	y += addY;
	DrawFormatString(
		Application::SCREEN_SIZE_X - x, y, 0xffffff, "　　　　旋回：↑↓←→");
	y += addY;
	DrawFormatString(
		Application::SCREEN_SIZE_X - x, y, 0xffffff, "　　　　加速：Ｂ");
	y += addY;
	DrawFormatString(
		Application::SCREEN_SIZE_X - x, y, 0xffffff, "　　　　発弾：Ｎ");
	y += addY;
	DrawFormatString(
		Application::SCREEN_SIZE_X - x, y, 0xffffff, "　シーン遷移：Ｓｐａｃｅ");
	
}

void Camera::Release(void)
{
}

VECTOR Camera::GetPos(void) const
{
	return mPos;
}


VECTOR Camera::GetTargetPos(void) const
{
	return mTargetPos;
}

VECTOR Camera::GetDir(void) const
{
	return VNorm(VSub(mTargetPos, mPos));
}

void Camera::ChangeMode(MODE mode)
{
	SetDefault();

	mMode = mode;

	switch (mMode)
	{
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FOLLOW:
		break;
	case Camera::MODE::FOLLOW_SPRING:
		break;
	case Camera::MODE::SHAKE:
		mStepShake = TIME_SHAKE;
		mShakeDir = VNorm({0.7f,0.7f,0.0f});//斜めのベクトル
		mDefaultPos = mPos;
		break;
	}



}

void Camera::SetFollowTarget(const Transform* target)
{
	mTarget = target;
}
