#pragma once
#include"ObjBase.h"
class StageBase:public ObjBase
{
public:
	StageBase();
	virtual ~StageBase();
	virtual bool Init()override=0;// ステージをロードする純粋仮想関数
	virtual void Update() override {};  // 仮想関数をオーバーライド
	virtual void Draw() override {};  // 仮想関数をオーバーライド
	virtual void Release() = 0;// ステージをアンロードする純粋仮想関数
};

