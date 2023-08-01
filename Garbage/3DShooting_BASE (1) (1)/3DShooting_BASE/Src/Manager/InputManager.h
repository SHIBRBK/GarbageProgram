#pragma once
#include <map>
#include "../Common/Vector2.h"

class InputManager
{

public:

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static InputManager& GetInstance(void);

	void Init(void);
	void Update(void);

	// 判定を行うキーを追加
	void Add(int key);

	// 判定を行うキーをクリア
	void Clear(void);

	// キーの押下判定
	bool IsNew(int key) const;

	// キーの押下判定(押しっぱなしはNG)
	bool IsTrgDown(int key) const;

	// キーを離した時の判定
	bool IsTrgUp(int key) const;

	// マウス座標の取得
	Vector2 GetMousePos(void) const;

	// マウスのクリック状態を取得(MOUSE_INPUT_LEFT、RIGHT)
	int GetMouse(void) const;

	// マウスが左クリックされたか
	bool IsClickMouseLeft(void) const;

	// マウスが右クリックされたか
	bool IsClickMouseRight(void) const;

	// マウスが左クリックされたか(押しっぱなしはNG)
	bool IsTrgMouseLeft(void) const;

	// マウスが右クリックされたか(押しっぱなしはNG)
	bool IsTrgMouseRight(void) const;

private:

	// キー情報
	struct Info
	{
		int key;			// キーID
		bool keyOld;		// 1フレーム前の押下状態
		bool keyNew;		// 現フレームの押下状態
		bool keyTrgDown;	// 現フレームでボタンが押されたか
		bool keyTrgUp;		// 現フレームでボタンが離されたか
	};

	// マウス
	struct MouseInfo
	{
		int key;			// キーID
		bool keyOld;		// 1フレーム前の押下状態
		bool keyNew;		// 現フレームの押下状態
		bool keyTrgDown;	// 現フレームでボタンが押されたか
		bool keyTrgUp;		// 現フレームでボタンが離されたか
	};

	// シングルトン用インスタンス
	static InputManager* mInstance;

	// キー情報
	std::map<int, InputManager::Info> mKeyInfos;

	// マウス情報
	std::map<int, InputManager::MouseInfo> mMouseInfos;

	// マウスカーソルの位置
	Vector2 mMousePos;
	
	// マウスボタンの入力状態
	int mMouseInput;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	InputManager(void);
	~InputManager(void);

	// 配列の中からキー情報を取得する
	const InputManager::Info& Find(int key) const;

	// 配列の中からマウス情報を取得する
	const InputManager::MouseInfo& FindMouse(int key) const;

};