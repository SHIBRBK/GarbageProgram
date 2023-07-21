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
	virtual void Draw(void) = 0;//Œp³‚·‚é‚Å
private:

protected:
	BaseScene(); 
	int screenID_;
	const Vector2D screenSize_;//{}’¼Ú”š‚ğ‘‚­‚Ì‚ÍŒÅ’è‚³‚ê‚½î•ñ‚Å‚µ‚©‚È‚¢
	Vector2D drawOffset_;

};

