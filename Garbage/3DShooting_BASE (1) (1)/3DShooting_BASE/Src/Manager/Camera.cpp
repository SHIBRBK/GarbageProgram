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

	// �J�����̏����ݒ�
	mPos = DEFAULT_CAMERA_POS;
	mTargetPos = VAdd(mPos, RELATIVE_TARGET_POS);
	mCameraUp = { 0.0f, 1.0f, 0.0f };

	// �J������X���ɌX���Ă��邪�A���̌X������Ԃ��X�������Ƃ���
	// mQuaRot�͉�]�v�Z�p�ŁA
	// �����܂Ŏ��ƂȂ�̂́A�J�������W�ƒ����_�Ƃ���
	mQuaRot = Quaternion();

}

void Camera::Update()
{
}

void Camera::SetBeforeDraw(void)
{

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
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


	// �J�����̐ݒ�
	SetCameraPositionAndTargetAndUpVec(
		mPos,
		mTargetPos,
		mCameraUp
	);

	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();
	
}

void Camera::SetBeforeDrawFree(void)
{

	//��]����
	VECTOR axis = AsoUtility::VECTOR_ZERO;
	float rev = 1.0f;
	if (CheckHitKey(KEY_INPUT_UP)) { axis = AsoUtility::AXIS_X; rev = -1.0f; }
	if (CheckHitKey(KEY_INPUT_DOWN)) { axis = AsoUtility::AXIS_X; }
	if (CheckHitKey(KEY_INPUT_LEFT)) { axis = AsoUtility::AXIS_Y; rev = -1.0f; }
	if (CheckHitKey(KEY_INPUT_RIGHT)) { axis = AsoUtility::AXIS_Y; }
	//�J�������W�𒆐S�Ƃ��āA�����_�������Ă񂳂���
	if (!AsoUtility::EqualsVZero(axis))
	{
		Quaternion axisQ = Quaternion::AngleAxis(AsoUtility::Deg2RadF(1.0f*rev), axis);
		mQuaRot = mQuaRot.Mult(axisQ);

		//�ʒu���ɉ�]���𔽉f
		VECTOR localPos = mQuaRot.PosAxis(RELATIVE_TARGET_POS);

		//�����_�X�V
		mTargetPos = VAdd(mPos,localPos);

		//�J�����̏�����X�V
		mCameraUp = mQuaRot.GetUp();//���ʂ̃Q�[���Ȃ�Y���ŌŒ肵�Ă��������ǁA�RD�V���[�e�B���O�͂��ꂶ��Ȃ��Ⴞ��
	}


	//�J�������삪�ł���悤��
	//�ړ�����
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (CheckHitKey(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (CheckHitKey(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (CheckHitKey(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (CheckHitKey(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }


	if (!AsoUtility::EqualsVZero(moveDir))
	{
		float speed = 20.0f;

		//VTransform�̃N�H�[�^�j�I���o�[�W����
		//lpos'=q�Elpos�Eq(-1)
		//Quaternion tmp;
		//tmp = tmp.Mult(mQuaRot);
		//tmp = tmp.Mult(Quaternion(0.0f,moveDir.x,moveDir.y,moveDir.z));
		//tmp = tmp.Mult(mQuaRot.Inverse());

		//�ړ��������J���������ɍ��킹�ĉ�]
		VECTOR rotDir = mQuaRot.PosAxis(moveDir);

		//�ړ��������������Ɉړ��ʂ�������
		VECTOR movePow = VScale(rotDir,speed);

		//�J�����ʒu�ƒ����_���ړ�����
		mPos = VAdd(mPos, movePow);
		mTargetPos = VAdd(mTargetPos, movePow);

	}
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//��_�J�����Ȃ̂ŉ������Ȃ�

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

	// ���@���
	VECTOR shipPos = mTarget->pos;
	MATRIX shipMat = mTarget->matRot;
	Quaternion shipRot = mTarget->quaRot;

	// �J�������W
	VECTOR relativeCPos = shipRot.PosAxis({ 0.0f, 40.0f, 150.0f });

	// �΂˒萔
	float POW_SPRING = 24.0f;
	float dampening = 2.0f * sqrtf(POW_SPRING);

	// �f���^�^�C���̘b����
	//float delta = SceneManager::GetInstance().GetDeltaTime();
	float delta = 1.0f / 60.0f;

	// ���z�̈ʒu
	VECTOR idealPos = VAdd(shipPos, relativeCPos);

	// ���z�Ǝ��ۂ̈ʒu�̍�
	VECTOR diff = VSub(mPos, idealPos);

	// �� = -�o�l�̋��� �~ �o�l�̐L�� - ��R �~ �J�����̑��x
	VECTOR force = VScale(diff, -POW_SPRING);
	force = VSub(force, VScale(mVelocity, dampening));

	// ���x�̍X�V
	mVelocity = VAdd(mVelocity, VScale(force, delta));

	// �J�����ʒu�̍X�V
	mPos = VAdd(mPos, VScale(mVelocity, delta));

	// �����_
	VECTOR localTPos = shipRot.PosAxis(RELATIVE_TARGET_POS);
	mTargetPos = VAdd(mPos, localTPos);

	// �J�����̏����
	mCameraUp = mTarget->GetUp();

}

void Camera::SetBeforeDrawShake(void)
{

	mStepShake -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepShake < 0.0f)
	{
		//�I������
		mPos = mDefaultPos;
		//�Œ�J�����ɖ߂�
		ChangeMode(MODE::FIXED_POINT);
		return;
	}

	//�J�����h�炵�����i�ɂ₩�j
	//�g�̗� = �g�̍�����sin�i���ԁ��g�̑��x�j
	//float movePow = WIDTH_SHAKE*sinf(mStepShake*SPEED_SHAKE);//�T�C���g���g��
	//VECTOR velocity = VScale(mShakeDir, movePow);
	//VECTOR newPos = VAdd(mDefaultPos, velocity);
	//mPos = newPos;
	//sin��-1.0����A1.0�܂ł̐������ɂ₩�ɍ��
	//���ō��̂́A-1.0��1.0�̓��ނ̂�

	//�J�����h�炵�����i�������j
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

	// �������
	int x = 200;
	int y = 10;
	int addY = 40;
	DrawFormatString(
		Application::SCREEN_SIZE_X - x, y, 0xffffff, "�@�y����z");
	y += addY;
	DrawFormatString(
		Application::SCREEN_SIZE_X - x, y, 0xffffff, "�@�@�@�@����F��������");
	y += addY;
	DrawFormatString(
		Application::SCREEN_SIZE_X - x, y, 0xffffff, "�@�@�@�@�����F�a");
	y += addY;
	DrawFormatString(
		Application::SCREEN_SIZE_X - x, y, 0xffffff, "�@�@�@�@���e�F�m");
	y += addY;
	DrawFormatString(
		Application::SCREEN_SIZE_X - x, y, 0xffffff, "�@�V�[���J�ځF�r��������");
	
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
		mShakeDir = VNorm({0.7f,0.7f,0.0f});//�΂߂̃x�N�g��
		mDefaultPos = mPos;
		break;
	}



}

void Camera::SetFollowTarget(const Transform* target)
{
	mTarget = target;
}
