#pragma once
#include "ObjBase.h"
class EnemyBase
	:public ObjBase
{
public:
	virtual ~EnemyBase()override;
	virtual bool Init()override=0;
	virtual void Update()override = 0;
	virtual void Draw()override = 0;
	virtual void Release()override = 0;
protected:
	EnemyBase();
};

