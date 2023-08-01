#pragma once
#include "ShotBase.h"

class PlayerShot : public ShotBase
{
public:

	//コンスト
	PlayerShot(const Transform* parent);

	//デストラクタ
	~PlayerShot(void);

};

