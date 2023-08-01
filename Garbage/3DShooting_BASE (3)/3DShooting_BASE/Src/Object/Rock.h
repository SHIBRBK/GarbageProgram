#pragma once
#include"../Manager/ResourceManager.h"
#include"../Manager/Resource.h"
#include"Common/Transform.h"


class Transform;

class Rock
{
public:
	Rock(void);
	~Rock(void);

	void Init(int modelId,VECTOR pos, VECTOR angle,VECTOR scale);
	void Update();
	void Draw();
	void Release();

private:
	Transform mTransform_;
};

