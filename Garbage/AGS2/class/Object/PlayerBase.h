#pragma once
#include <memory>
#include <DxLib.h>
#include "ObjBase.h"



class PlayerBase :
    public ObjBase
{
public:
    virtual ~PlayerBase()override;
    virtual bool Init()override;
    virtual void Update()override;
    virtual void Draw()override;
    virtual void Release()override;

    virtual VECTOR GetPos(void)const =0;
protected:
    PlayerBase();

};

