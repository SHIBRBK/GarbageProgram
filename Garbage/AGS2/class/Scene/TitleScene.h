#pragma once
#include "BaseScene.h"
class TitleScene :
    public BaseScene
{
public:
    TitleScene();
    ~TitleScene();
    void Init(void)override;
    void Update()override;
    void Draw(void)override;
};

