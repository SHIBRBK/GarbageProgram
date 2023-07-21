#pragma once
class ObjBase
{
public:
	virtual ~ObjBase();
	virtual bool Init()=0;
	virtual void Update()=0;
	virtual void Draw()=0;
	virtual void Release()=0;
protected:
	ObjBase();
};

