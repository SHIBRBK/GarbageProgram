#pragma once
#include <string>
#include <DxLib.h>
#include "Transform.h"
class SceneManager;

class SpeechBalloon
{

public:

	// 表示種別
	enum class TYPE
	{
		TEXT,
		SPEECH
	};

	// コンストラクタ
	SpeechBalloon(TYPE type, const Transform* parent);

	// デストラクタ
	~SpeechBalloon(void);

	void Draw(void);
	void DrawTextOrg(void);
	void DrawSpeech(void);
	void Release(void);

	void SetText(const std::string& text);
	void SetTime(double time);

	void SetType(TYPE type);
	void SetVisiblePermanent(void);

	void SetRelativePos(VECTOR relativePos);

private:

	TYPE mType;

	int mImage;

	// trueの場合、永続的に表示する
	bool mIsVisiblePermanent;
	double mVisibleTime;

	// 吹き出しを付ける親オブジェクト
	const Transform* mTransformParent;

	// 表示座標
	VECTOR mPos;

	// 表示位置の調整用
	VECTOR mRelativePos;

	// 表示文字
	std::string mText;

};
