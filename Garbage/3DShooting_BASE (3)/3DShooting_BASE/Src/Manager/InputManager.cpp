#include <DxLib.h>
#include "InputManager.h"

InputManager* InputManager::mInstance = nullptr;

void InputManager::CreateInstance(void)
{
	if (mInstance == nullptr)
	{
		mInstance = new InputManager();
	}
	mInstance->Init();
}

InputManager& InputManager::GetInstance(void)
{
	if (mInstance == nullptr)
	{
		InputManager::CreateInstance();
	}
	return *mInstance;
}

void InputManager::Init(void)
{
	// ゲームで使用したいキーを、
	// 事前にここで登録しておいてください
	InputManager::GetInstance().Add(KEY_INPUT_SPACE);
	InputManager::GetInstance().Add(KEY_INPUT_N);



	InputManager::MouseInfo info;

	// 左クリック
	info = InputManager::MouseInfo();
	info.key = MOUSE_INPUT_LEFT;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	mMouseInfos.emplace(info.key, info);

	// 右クリック
	info = InputManager::MouseInfo();
	info.key = MOUSE_INPUT_RIGHT;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	mMouseInfos.emplace(info.key, info);

}

void InputManager::Update(void)
{

	// キーボード検知
	for (auto& p : mKeyInfos)
	{
		p.second.keyOld = p.second.keyNew;
		p.second.keyNew = CheckHitKey(p.second.key);
		p.second.keyTrgDown = p.second.keyNew && !p.second.keyOld;
		p.second.keyTrgUp = !p.second.keyNew && p.second.keyOld;
	}

	// マウス検知
	mMouseInput = GetMouseInput();
	GetMousePoint(&mMousePos.x, &mMousePos.y);

	for (auto& p : mMouseInfos)
	{
		p.second.keyOld = p.second.keyNew;
		p.second.keyNew = mMouseInput == p.second.key;
		p.second.keyTrgDown = p.second.keyNew && !p.second.keyOld;
		p.second.keyTrgUp = !p.second.keyNew && p.second.keyOld;
	}

}

void InputManager::Add(int key)
{
	InputManager::Info info = InputManager::Info();
	info.key = key;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	mKeyInfos.emplace(key, info);
}

void InputManager::Clear(void)
{
	mKeyInfos.clear();
}

bool InputManager::IsNew(int key) const
{
	return Find(key).keyNew;
}

bool InputManager::IsTrgDown(int key) const
{
	return Find(key).keyTrgDown;
}

bool InputManager::IsTrgUp(int key) const
{
	return Find(key).keyTrgUp;
}

Vector2 InputManager::GetMousePos(void) const
{
	return mMousePos;
}

int InputManager::GetMouse(void) const
{
	return mMouseInput;
}

bool InputManager::IsClickMouseLeft(void) const
{
	return mMouseInput == MOUSE_INPUT_LEFT;
}

bool InputManager::IsClickMouseRight(void) const
{
	return mMouseInput == MOUSE_INPUT_RIGHT;
}

bool InputManager::IsTrgMouseLeft(void) const
{
	return FindMouse(MOUSE_INPUT_LEFT).keyTrgDown;
}

bool InputManager::IsTrgMouseRight(void) const
{
	return FindMouse(MOUSE_INPUT_RIGHT).keyTrgDown;
}

InputManager::InputManager(void)
{
	mMouseInput = -1;
}

InputManager::~InputManager(void)
{
	delete mInstance;
}

const InputManager::Info& InputManager::Find(int key) const
{

	auto it = mKeyInfos.find(key);
	if (it != mKeyInfos.end())
	{
		return it->second;
	}

	return InputManager::Info();

}

const InputManager::MouseInfo& InputManager::FindMouse(int key) const
{
	auto it = mMouseInfos.find(key);
	if (it != mMouseInfos.end())
	{
		return it->second;
	}

	return InputManager::MouseInfo();
}
