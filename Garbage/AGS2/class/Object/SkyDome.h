#pragma once
#include "../Common/Transform.h"

class SkyDome
{
public:

	static constexpr float SCALE = 100.0f;
	static constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };

	// ���
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	// �R���X�g���N�^
	SkyDome(const Transform* syncTransform);

	// �f�X�g���N�^
	~SkyDome(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// ���@�̏��
	const Transform* mSyncTransform;

	// ���
	STATE mState;

	// ���f������̊�{���
	Transform transform_;

	// ��ԑJ��
	void ChangeState(STATE state);
	float Deg2RadF(float deg);
};
