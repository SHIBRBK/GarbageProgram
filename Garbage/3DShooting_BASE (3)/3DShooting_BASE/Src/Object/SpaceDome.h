#pragma once
#include<DxLib.h>
#include"../Object/Common/Transform.h"

class SpaceDome
{
public:
	enum class STATE
	{
	NONE,
	STAY,
	FOLLOW,
	};



	SpaceDome(const Transform* target);
	~SpaceDome();
	void Init();
	void Draw();
	void Update();
	void Release();

	//GameScene��stage�N���X��錾���āA���s�ł���悤�ɂ��Ă��������B
	//DrawGrid��XYZ��`�悵�Ă�������

private:

	//�Ǐ]����
	const Transform* mTarget_;

	//���f����3����
	Transform mTransform_;

	STATE mState;

	void ChangeState(STATE state);



};

