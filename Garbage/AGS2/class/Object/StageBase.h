#pragma once
#include"ObjBase.h"
class StageBase:public ObjBase
{
public:
	StageBase();
	virtual ~StageBase();
	virtual bool Init()override=0;// �X�e�[�W�����[�h���鏃�����z�֐�
	virtual void Update() override {};  // ���z�֐����I�[�o�[���C�h
	virtual void Draw() override {};  // ���z�֐����I�[�o�[���C�h
	virtual void Release() = 0;// �X�e�[�W���A�����[�h���鏃�����z�֐�
};

