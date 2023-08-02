#pragma once
#include<vector>
#include<DxLib.h>
#include "../Common/Transform.h"
class ShotBase
{
public:

	enum class STATE
	{
		NONE,
		SHOT,
		BLAST,
		END
	};

	ShotBase(const Transform* parent);
	virtual ~ShotBase(void);
	virtual void CreateShot(VECTOR pos, VECTOR dir);

	virtual void Update(void);


	virtual void Draw(void);

	// �e�̑��x
	virtual float GetSpeed(void) const;
	virtual float GetTimeAlive(void) const;
	virtual bool CheckAlive(void);
	// ���W�̎擾
	VECTOR GetPos(void) const;
	// ���̓����蔻��p�̔��a���擾
	float GetCollisionRadius(void) const;
	// ��������
	bool IsAlive(void) const;
	virtual void Move(void);
	virtual void Release(void) ;
	float Deg2RadF(float deg);

protected:
	// ���f������̊�{���
	Transform transform_;

	Quaternion ShotQua;
	// ���˕���
	VECTOR mDir;
	// ��������
	bool mIsAlive;

	// ��������
	float mStepAlive;
	float speed_;
	// �����蔻��̔��a
	float mCollisionRadius;
	int shotmodel;
private:
	int baseModelId_;
	int* blastImgs_;
	int blastAnimNum_;
	// �e�̕W�����x
	static constexpr float DEFAULT_SPEED = 10.0f;
	static constexpr float DEFAULT_TIME_ALIVE = 10.0f;

};

