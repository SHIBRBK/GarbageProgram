#pragma once
#include <string>
#include <vector>

class Resource
{

public:
	
	// リソースタイプ
	enum class TYPE
	{
		NONE,
		IMG,
		IMGS,
		MODEL,
		EFFEKSEER
	};

	// コンストラクタ
	Resource(void);
	// コンストラクタ
	Resource(TYPE type, const std::string& path);
	// コンストラクタ(IMGS用)
	Resource(TYPE type, const std::string& path, int numX, int numY, int sizeX, int sizeY);

	// デストラクタ
	~Resource(void);

	// 読み込み
	void Load(void);

	// 解放
	void Release(void);

	// 複数画像ハンドルを別配列にコピー
	void CopyHandle(int* imgs);

	// リソースタイプ
	TYPE mType;

	// リソースの読み込み先
	std::string mPath;

	// 画像とモデルのハンドルID
	int mHandleId;

	// IMGS::LoadDivGraph用
	int* mHandleIds;
	int mNumX;
	int mNumY;
	int mSizeX;
	int mSizeY;

	// モデル複製用
	std::vector<int> mDuplicateModelIds;

};

