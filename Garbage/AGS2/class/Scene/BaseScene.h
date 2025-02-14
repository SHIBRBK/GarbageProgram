#pragma once
#include<memory>
#include"../Common/Vector2D.h"

class BaseScene;
using uniqueScene = std::unique_ptr<BaseScene>;


class BaseScene
{
public:
	virtual ~BaseScene();
	virtual void Init() = 0;
	virtual void Update()=0;
	virtual void Draw(void) = 0;//継承するで
private:

protected:
	BaseScene(); 
	int screenID_;
	const Vector2D screenSize_;//{}直接数字を書くのは固定された情報でしかない
	Vector2D drawOffset_;

};

