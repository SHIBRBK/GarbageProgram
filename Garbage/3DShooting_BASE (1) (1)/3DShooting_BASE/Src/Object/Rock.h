#pragma once
#include"Common/Transform.h"
class Rock
{
public:

	// コンストラクタ
	Rock(void);

	// デストラクタ
	~Rock(void);

	void Init(int modelId,VECTOR pos,VECTOR angle,VECTOR scale);
	void Update(void);
	void Draw(void);
	void DrawGrid(void);
	void Release(void);
private:
	Transform mTransform;
};

