#pragma once
#include "SceneBase.h"

class SpaceDome;



class TitleScene : public SceneBase
{

public:

	// �R���X�g���N�^
	TitleScene(void);

	// �f�X�g���N�^
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawLogo(void);
	void Release(void) override;

private:

	SpaceDome* mSpaceDome;



	// �^�C�g�����S
	int mImgTitleLogo;


};

