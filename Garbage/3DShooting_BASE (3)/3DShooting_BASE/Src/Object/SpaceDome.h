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

	//GameSceneにstageクラスを宣言して、実行できるようにしてください。
	//DrawGridでXYZを描画してください

private:

	//追従相手
	const Transform* mTarget_;

	//モデルの3制御
	Transform mTransform_;

	STATE mState;

	void ChangeState(STATE state);



};

