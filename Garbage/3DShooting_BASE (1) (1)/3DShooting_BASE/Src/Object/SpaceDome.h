#pragma once
#include<DxLib.h>
#include"../Object/Common/Transform.h"
class SpaceDome
{
public:

	//���
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW,
	};

	// �R���X�g���N�^
	SpaceDome(const Transform* target);

	// �f�X�g���N�^
	~SpaceDome(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:
	//�Ǐ]����
	const Transform* mTarget;

	Transform mTransform;

	STATE mState;

	//���
	void ChangeState(STATE state);

	//3D����ɕK�v�Ȋ�{���
	//VECTOR pos_;//���W���
	//VECTOR rot_;//��]���
	//VECTOR scl_;//�傫���̏��


	//VECTOR mPos;
	//Quaternion mRot;
	//VECTOR mScl;
};

