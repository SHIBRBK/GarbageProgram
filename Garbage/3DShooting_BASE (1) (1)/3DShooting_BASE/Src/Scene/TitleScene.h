#pragma once
#include "SceneBase.h"

class SpaceDome;
class ParticleGenerator;
class TextScroll;


class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawLogo(void);
	void Release(void) override;

private:

	SpaceDome* mSpaceDome;
	ParticleGenerator* mParticleGenerator;
	TextScroll* mTextScroll;


	// タイトルロゴ
	int mImgTitleLogo;

};

