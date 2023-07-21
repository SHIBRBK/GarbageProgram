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
	virtual void Draw(void) = 0;//�p�������
private:

protected:
	BaseScene(); 
	int screenID_;
	const Vector2D screenSize_;//{}���ڐ����������̂͌Œ肳�ꂽ���ł����Ȃ�
	Vector2D drawOffset_;

};

