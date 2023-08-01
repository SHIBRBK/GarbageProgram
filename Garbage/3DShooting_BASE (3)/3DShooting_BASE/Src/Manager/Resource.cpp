#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Resource.h"

Resource::Resource(void)
{
	mType = TYPE::NONE;
	mPath = "";

	mNumX = -1;
	mNumY = -1;
	mSizeX = -1;
	mSizeY = -1;

	mHandleId = -1;
	mHandleIds = nullptr;
}

Resource::Resource(TYPE type, const std::string& path)
{
	mType = type;
	mPath = path;

	mNumX = -1;
	mNumY = -1;
	mSizeX = -1;
	mSizeY = -1;

	mHandleId = -1;
	mHandleIds = nullptr;
}

Resource::Resource(TYPE type, const std::string& path, int numX, int numY, int sizeX, int sizeY)
{
	mType = type;
	mPath = path;
	mNumX = numX;
	mNumY = numY;
	mSizeX = sizeX;
	mSizeY = sizeY;

	mHandleId = -1;
	mHandleIds = nullptr;
}

Resource::~Resource(void)
{
}

void Resource::Load(void)
{

	switch (mType)
	{
	case Resource::TYPE::IMG:
		// âÊëú
		mHandleId = LoadGraph(mPath.c_str());
		break;

	case Resource::TYPE::IMGS:
		// ï°êîâÊëú
		mHandleIds = new int[mNumX * mNumY];
		LoadDivGraph(
			mPath.c_str(),
			mNumX * mNumY,
			mNumX, mNumY,
			mSizeX, mSizeY,
			&mHandleIds[0]);
		break;

	case Resource::TYPE::MODEL:
		// ÉÇÉfÉã
		mHandleId = MV1LoadModel(mPath.c_str());
		break;

	case Resource::TYPE::EFFEKSEER:

		mHandleId = LoadEffekseerEffect(mPath.c_str());
		break;

	}

}

void Resource::Release(void)
{

	switch (mType)
	{
	case Resource::TYPE::IMG:
		DeleteGraph(mHandleId);
		break;

	case Resource::TYPE::IMGS:
	{
		int num = mNumX * mNumY;
		for (int i = 0; i < num; i++)
		{
			DeleteGraph(mHandleIds[i]);
		}
		delete[] mHandleIds;
	}
		break;

	case Resource::TYPE::MODEL:
	{
		MV1DeleteModel(mHandleId);
		auto ids = mDuplicateModelIds;
		for (auto id : ids)
		{
			MV1DeleteModel(id);
		}
	}
		break;

	case Resource::TYPE::EFFEKSEER:

		DeleteEffekseerEffect(mHandleId);
		break;

	}

}

void Resource::CopyHandle(int* imgs)
{

	if (mHandleIds == nullptr)
	{
		return;
	}

	int num = mNumX * mNumY;
	for (int i = 0; i < num; i++)
	{
		imgs[i] = mHandleIds[i];
	}

}
