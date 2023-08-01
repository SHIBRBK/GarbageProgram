#pragma once
#include <vector>
#include <DxLib.h>
#include "../Common/Quaternion.h"


class Transform;

class Camera
{

public:

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_FAR = 15000.0f;

	// �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// �J�����ʒu�ƒ����_�Ƃ̑��΍��W
	static constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, -100.0f, 600.0f };

	static constexpr float TIME_SHAKE = 2.0f;

	//static constexpr float WIDTH_SHAKE = 3.0f;
	static constexpr float WIDTH_SHAKE = 5.0f;

	//static constexpr float SPEED_SHAKE = 30.0f;
	static constexpr float SPEED_SHAKE = 15.0f;
	enum class MODE {
		NONE,
		FREE,
		FIXED,
		FOLLOW,
		FOLLOW_SPRING,
		SHAKE
	};


	// �R���X�g���N�^
	Camera(void);

	// �f�X�g���N�^
	~Camera(void);

	// ������
	void Init(void);

	// �J�����������ʒu�ɖ߂�
	void SetDefault(void);

	// �X�V
	void Update(void);

	// �`��O�̃J�����ݒ�
	void SetBeforeDraw(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawFixed(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawFollowSpring(void);
	void SetBeforeDrawShake(void);
	
	// �`��
	void Draw(void);
	void DrawUI(void);

	void Release(void);

	// �J�������W
	VECTOR GetPos(void) const;

	// �����_���W
	VECTOR GetTargetPos(void) const;

	// �J��������
	VECTOR GetDir(void) const;

	//�J�������[�h�̕ύX
	void changeMode(MODE mode);

	//�Ǐ]����̐ݒ�
	void SetFollowTarget(const Transform* target);


private:

	//�Ǐ]�Ώ�
	const Transform* mTarget;


	MODE mMode;

	// �J�����̈ʒu
	VECTOR mPos;


	// �J�����p�x
	Quaternion mQuaRot;

	// �����_
	VECTOR mTargetPos;

	// �J�����̏����
	VECTOR mCameraUp;

	//���x
	VECTOR mVelocity;

	//�J�����V�F�C�N
	float mStepShake;
	//�J�����V�F�C�N�O�̏����ʒu
	VECTOR mDefaultPos;
	//�J������h�炷����
	VECTOR mShakeDir;
};

