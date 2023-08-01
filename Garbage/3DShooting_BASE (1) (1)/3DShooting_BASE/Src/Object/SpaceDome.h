#pragma once
#include<DxLib.h>
#include"../Object/Common/Transform.h"
class SpaceDome
{
public:

	//状態
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW,
	};

	// コンストラクタ
	SpaceDome(const Transform* target);

	// デストラクタ
	~SpaceDome(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:
	//追従相手
	const Transform* mTarget;

	Transform mTransform;

	STATE mState;

	//状態
	void ChangeState(STATE state);

	//3D制御に必要な基本情報
	//VECTOR pos_;//座標情報
	//VECTOR rot_;//回転情報
	//VECTOR scl_;//大きさの情報


	//VECTOR mPos;
	//Quaternion mRot;
	//VECTOR mScl;
};

