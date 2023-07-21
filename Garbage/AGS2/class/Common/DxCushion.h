#pragma once
#include <Windows.h>
#include<string>
class DxCushion
{
public:
	// 描画ブレンドモード定義
#define DX_BLENDMODE_NOBLEND						(0)				// ノーブレンド
#define DX_BLENDMODE_ALPHA							(1)				// αブレンド
#define DX_BLENDMODE_ADD							(2)				// 加算ブレンド
#define DX_BLENDMODE_SUB							(3)				// 減算ブレンド
#define DX_BLENDMODE_MUL							(4)				// 乗算ブレンド


	// CheckHitKeyAll で調べる入力タイプ
#define DX_CHECKINPUT_KEY							(0x0001)		// キー入力を調べる
#define DX_CHECKINPUT_PAD							(0x0002)		// パッド入力を調べる
#define DX_CHECKINPUT_MOUSE							(0x0004)		// マウスボタン入力を調べる
#define DX_CHECKINPUT_ALL							(DX_CHECKINPUT_KEY | DX_CHECKINPUT_PAD | DX_CHECKINPUT_MOUSE)	// すべての入力を調べる

// パッド入力取得パラメータ
#define DX_INPUT_KEY_PAD1							(0x1001)		// キー入力とパッド１入力
#define DX_INPUT_PAD1								(0x0001)		// パッド１入力
#define DX_INPUT_PAD2								(0x0002)		// パッド２入力
#define DX_INPUT_PAD3								(0x0003)		// パッド３入力
#define DX_INPUT_PAD4								(0x0004)		// パッド４入力
#define DX_INPUT_PAD5								(0x0005)		// パッド５入力
#define DX_INPUT_PAD6								(0x0006)		// パッド６入力
#define DX_INPUT_PAD7								(0x0007)		// パッド７入力
#define DX_INPUT_PAD8								(0x0008)		// パッド８入力
#define DX_INPUT_PAD9								(0x0009)		// パッド９入力
#define DX_INPUT_PAD10								(0x000a)		// パッド１０入力
#define DX_INPUT_PAD11								(0x000b)		// パッド１１入力
#define DX_INPUT_PAD12								(0x000c)		// パッド１２入力
#define DX_INPUT_PAD13								(0x000d)		// パッド１３入力
#define DX_INPUT_PAD14								(0x000e)		// パッド１４入力
#define DX_INPUT_PAD15								(0x000f)		// パッド１５入力
#define DX_INPUT_PAD16								(0x0010)		// パッド１６入力
#define DX_INPUT_KEY								(0x1000)		// キー入力

// ゲームコントローラータイプ
#define DX_PADTYPE_OTHER							(0)
#define DX_PADTYPE_XBOX_360							(1)
#define DX_PADTYPE_XBOX_ONE							(2)
#define DX_PADTYPE_DUAL_SHOCK_4						(3)
#define DX_PADTYPE_DUAL_SENSE						(4)
#define DX_PADTYPE_SWITCH_JOY_CON_L					(5)
#define DX_PADTYPE_SWITCH_JOY_CON_R					(6)
#define DX_PADTYPE_SWITCH_PRO_CTRL					(7)
#define DX_PADTYPE_NUM								(8)

// タッチの同時接触検出対応最大数
#define TOUCHINPUTPOINT_MAX							(16)

// パッド入力定義
#define PAD_INPUT_DOWN								(0x00000001)	// ↓チェックマスク
#define PAD_INPUT_LEFT								(0x00000002)	// ←チェックマスク
#define PAD_INPUT_RIGHT								(0x00000004)	// →チェックマスク
#define PAD_INPUT_UP								(0x00000008)	// ↑チェックマスク
#define PAD_INPUT_A									(0x00000010)	// Ａボタンチェックマスク
#define PAD_INPUT_B									(0x00000020)	// Ｂボタンチェックマスク
#define PAD_INPUT_C									(0x00000040)	// Ｃボタンチェックマスク
#define PAD_INPUT_X									(0x00000080)	// Ｘボタンチェックマスク
#define PAD_INPUT_Y									(0x00000100)	// Ｙボタンチェックマスク
#define PAD_INPUT_Z									(0x00000200)	// Ｚボタンチェックマスク
#define PAD_INPUT_L									(0x00000400)	// Ｌボタンチェックマスク
#define PAD_INPUT_R									(0x00000800)	// Ｒボタンチェックマスク
#define PAD_INPUT_START								(0x00001000)	// ＳＴＡＲＴボタンチェックマスク
#define PAD_INPUT_M									(0x00002000)	// Ｍボタンチェックマスク
#define PAD_INPUT_D									(0x00004000)
#define PAD_INPUT_F									(0x00008000)
#define PAD_INPUT_G									(0x00010000)
#define PAD_INPUT_H									(0x00020000)
#define PAD_INPUT_I									(0x00040000)
#define PAD_INPUT_J									(0x00080000)
#define PAD_INPUT_K									(0x00100000)
#define PAD_INPUT_LL								(0x00200000)
#define PAD_INPUT_N									(0x00400000)
#define PAD_INPUT_O									(0x00800000)
#define PAD_INPUT_P									(0x01000000)
#define PAD_INPUT_RR								(0x02000000)
#define PAD_INPUT_S									(0x04000000)
#define PAD_INPUT_T									(0x08000000)
#define PAD_INPUT_U									(0x10000000)
#define PAD_INPUT_V									(0x20000000)
#define PAD_INPUT_W									(0x40000000)
#define PAD_INPUT_XX								(0x80000000)

#define PAD_INPUT_1									(0x00000010)
#define PAD_INPUT_2									(0x00000020)
#define PAD_INPUT_3									(0x00000040)
#define PAD_INPUT_4									(0x00000080)
#define PAD_INPUT_5									(0x00000100)
#define PAD_INPUT_6									(0x00000200)
#define PAD_INPUT_7									(0x00000400)
#define PAD_INPUT_8									(0x00000800)
#define PAD_INPUT_9									(0x00001000)
#define PAD_INPUT_10								(0x00002000)
#define PAD_INPUT_11								(0x00004000)
#define PAD_INPUT_12								(0x00008000)
#define PAD_INPUT_13								(0x00010000)
#define PAD_INPUT_14								(0x00020000)
#define PAD_INPUT_15								(0x00040000)
#define PAD_INPUT_16								(0x00080000)
#define PAD_INPUT_17								(0x00100000)
#define PAD_INPUT_18								(0x00200000)
#define PAD_INPUT_19								(0x00400000)
#define PAD_INPUT_20								(0x00800000)
#define PAD_INPUT_21								(0x01000000)
#define PAD_INPUT_22								(0x02000000)
#define PAD_INPUT_23								(0x04000000)
#define PAD_INPUT_24								(0x08000000)
#define PAD_INPUT_25								(0x10000000)
#define PAD_INPUT_26								(0x20000000)
#define PAD_INPUT_27								(0x40000000)
#define PAD_INPUT_28								(0x80000000)

// XInputボタン入力定義
#define XINPUT_BUTTON_DPAD_UP						(0)				// デジタル方向ボタン上
#define XINPUT_BUTTON_DPAD_DOWN						(1)				// デジタル方向ボタン下
#define XINPUT_BUTTON_DPAD_LEFT						(2)				// デジタル方向ボタン左
#define XINPUT_BUTTON_DPAD_RIGHT					(3)				// デジタル方向ボタン右
#define XINPUT_BUTTON_START							(4)				// STARTボタン
#define XINPUT_BUTTON_BACK							(5)				// BACKボタン
#define XINPUT_BUTTON_LEFT_THUMB					(6)				// 左スティック押し込み
#define XINPUT_BUTTON_RIGHT_THUMB					(7)				// 右スティック押し込み
#define XINPUT_BUTTON_LEFT_SHOULDER					(8)				// LBボタン
#define XINPUT_BUTTON_RIGHT_SHOULDER				(9)				// RBボタン
#define XINPUT_BUTTON_A								(12)			// Aボタン
#define XINPUT_BUTTON_B								(13)			// Bボタン
#define XINPUT_BUTTON_X								(14)			// Xボタン
#define XINPUT_BUTTON_Y								(15)			// Yボタン

// マウス入力定義
#define MOUSE_INPUT_LEFT							(0x0001)		// マウス左ボタン
#define MOUSE_INPUT_RIGHT							(0x0002)		// マウス右ボタン
#define MOUSE_INPUT_MIDDLE							(0x0004)		// マウス中央ボタン
#define MOUSE_INPUT_1								(0x0001)		// マウス１ボタン
#define MOUSE_INPUT_2								(0x0002)		// マウス２ボタン
#define MOUSE_INPUT_3								(0x0004)		// マウス３ボタン
#define MOUSE_INPUT_4								(0x0008)		// マウス４ボタン
#define MOUSE_INPUT_5								(0x0010)		// マウス５ボタン
#define MOUSE_INPUT_6								(0x0020)		// マウス６ボタン
#define MOUSE_INPUT_7								(0x0040)		// マウス７ボタン
#define MOUSE_INPUT_8								(0x0080)		// マウス８ボタン

// マウスのログ情報タイプ
#define MOUSE_INPUT_LOG_DOWN						(0)				// ボタンを押した
#define MOUSE_INPUT_LOG_UP							(1)				// ボタンを離した

// キー定義
#define KEY_INPUT_BACK								(0x0E)			// BackSpaceキー	D_DIK_BACK
#define KEY_INPUT_TAB								(0x0F)			// Tabキー			D_DIK_TAB
#define KEY_INPUT_RETURN							(0x1C)			// Enterキー		D_DIK_RETURN

#define KEY_INPUT_LSHIFT							(0x2A)			// 左Shiftキー		D_DIK_LSHIFT
#define KEY_INPUT_RSHIFT							(0x36)			// 右Shiftキー		D_DIK_RSHIFT
#define KEY_INPUT_LCONTROL							(0x1D)			// 左Ctrlキー		D_DIK_LCONTROL
#define KEY_INPUT_RCONTROL							(0x9D)			// 右Ctrlキー		D_DIK_RCONTROL
#define KEY_INPUT_ESCAPE							(0x01)			// Escキー			D_DIK_ESCAPE
#define KEY_INPUT_SPACE								(0x39)			// スペースキー		D_DIK_SPACE
#define KEY_INPUT_PGUP								(0xC9)			// PageUpキー		D_DIK_PGUP
#define KEY_INPUT_PGDN								(0xD1)			// PageDownキー		D_DIK_PGDN
#define KEY_INPUT_END								(0xCF)			// Endキー			D_DIK_END
#define KEY_INPUT_HOME								(0xC7)			// Homeキー			D_DIK_HOME
#define KEY_INPUT_LEFT								(0xCB)			// 左キー			D_DIK_LEFT
#define KEY_INPUT_UP								(0xC8)			// 上キー			D_DIK_UP
#define KEY_INPUT_RIGHT								(0xCD)			// 右キー			D_DIK_RIGHT
#define KEY_INPUT_DOWN								(0xD0)			// 下キー			D_DIK_DOWN
#define KEY_INPUT_INSERT							(0xD2)			// Insertキー		D_DIK_INSERT
#define KEY_INPUT_DELETE							(0xD3)			// Deleteキー		D_DIK_DELETE

#define KEY_INPUT_MINUS								(0x0C)			// －キー			D_DIK_MINUS
#define KEY_INPUT_YEN								(0x7D)			// ￥キー			D_DIK_YEN
#define KEY_INPUT_PREVTRACK							(0x90)			// ＾キー			D_DIK_PREVTRACK
#define KEY_INPUT_PERIOD							(0x34)			// ．キー			D_DIK_PERIOD
#define KEY_INPUT_SLASH								(0x35)			// ／キー			D_DIK_SLASH
#define KEY_INPUT_LALT								(0x38)			// 左Altキー		D_DIK_LALT
#define KEY_INPUT_RALT								(0xB8)			// 右Altキー		D_DIK_RALT
#define KEY_INPUT_SCROLL							(0x46)			// ScrollLockキー	D_DIK_SCROLL
#define KEY_INPUT_SEMICOLON							(0x27)			// ；キー			D_DIK_SEMICOLON
#define KEY_INPUT_COLON								(0x92)			// ：キー			D_DIK_COLON
#define KEY_INPUT_LBRACKET							(0x1A)			// ［キー			D_DIK_LBRACKET
#define KEY_INPUT_RBRACKET							(0x1B)			// ］キー			D_DIK_RBRACKET
#define KEY_INPUT_AT								(0x91)			// ＠キー			D_DIK_AT
#define KEY_INPUT_BACKSLASH							(0x2B)			// ＼キー			D_DIK_BACKSLASH
#define KEY_INPUT_COMMA								(0x33)			// ，キー			D_DIK_COMMA
#define KEY_INPUT_KANJI								(0x94)			// 漢字キー			D_DIK_KANJI
#define KEY_INPUT_CONVERT							(0x79)			// 変換キー			D_DIK_CONVERT
#define KEY_INPUT_NOCONVERT							(0x7B)			// 無変換キー		D_DIK_NOCONVERT
#define KEY_INPUT_KANA								(0x70)			// カナキー			D_DIK_KANA
#define KEY_INPUT_APPS								(0xDD)			// アプリケーションメニューキー		D_DIK_APPS
#define KEY_INPUT_CAPSLOCK							(0x3A)			// CaspLockキー		D_DIK_CAPSLOCK
#define KEY_INPUT_SYSRQ								(0xB7)			// PrintScreenキー	D_DIK_SYSRQ
#define KEY_INPUT_PAUSE								(0xC5)			// PauseBreakキー	D_DIK_PAUSE
#define KEY_INPUT_LWIN								(0xDB)			// 左Winキー		D_DIK_LWIN
#define KEY_INPUT_RWIN								(0xDC)			// 右Winキー		D_DIK_RWIN

#define KEY_INPUT_NUMLOCK							(0x45)			// テンキーNumLockキー		D_DIK_NUMLOCK
#define KEY_INPUT_NUMPAD0							(0x52)			// テンキー０				D_DIK_NUMPAD0
#define KEY_INPUT_NUMPAD1							(0x4F)			// テンキー１				D_DIK_NUMPAD1
#define KEY_INPUT_NUMPAD2							(0x50)			// テンキー２				D_DIK_NUMPAD2
#define KEY_INPUT_NUMPAD3							(0x51)			// テンキー３				D_DIK_NUMPAD3
#define KEY_INPUT_NUMPAD4							(0x4B)			// テンキー４				D_DIK_NUMPAD4
#define KEY_INPUT_NUMPAD5							(0x4C)			// テンキー５				D_DIK_NUMPAD5
#define KEY_INPUT_NUMPAD6							(0x4D)			// テンキー６				D_DIK_NUMPAD6
#define KEY_INPUT_NUMPAD7							(0x47)			// テンキー７				D_DIK_NUMPAD7
#define KEY_INPUT_NUMPAD8							(0x48)			// テンキー８				D_DIK_NUMPAD8
#define KEY_INPUT_NUMPAD9							(0x49)			// テンキー９				D_DIK_NUMPAD9
#define KEY_INPUT_MULTIPLY							(0x37)			// テンキー＊キー			D_DIK_MULTIPLY
#define KEY_INPUT_ADD								(0x4E)			// テンキー＋キー			D_DIK_ADD
#define KEY_INPUT_SUBTRACT							(0x4A)			// テンキー－キー			D_DIK_SUBTRACT
#define KEY_INPUT_DECIMAL							(0x53)			// テンキー．キー			D_DIK_DECIMAL
#define KEY_INPUT_DIVIDE							(0xB5)			// テンキー／キー			D_DIK_DIVIDE
#define KEY_INPUT_NUMPADENTER						(0x9C)			// テンキーのエンターキー	D_DIK_NUMPADENTER

#define KEY_INPUT_F1								(0x3B)			// Ｆ１キー			D_DIK_F1
#define KEY_INPUT_F2								(0x3C)			// Ｆ２キー			D_DIK_F2
#define KEY_INPUT_F3								(0x3D)			// Ｆ３キー			D_DIK_F3
#define KEY_INPUT_F4								(0x3E)			// Ｆ４キー			D_DIK_F4
#define KEY_INPUT_F5								(0x3F)			// Ｆ５キー			D_DIK_F5
#define KEY_INPUT_F6								(0x40)			// Ｆ６キー			D_DIK_F6
#define KEY_INPUT_F7								(0x41)			// Ｆ７キー			D_DIK_F7
#define KEY_INPUT_F8								(0x42)			// Ｆ８キー			D_DIK_F8
#define KEY_INPUT_F9								(0x43)			// Ｆ９キー			D_DIK_F9
#define KEY_INPUT_F10								(0x44)			// Ｆ１０キー		D_DIK_F10
#define KEY_INPUT_F11								(0x57)			// Ｆ１１キー		D_DIK_F11
#define KEY_INPUT_F12								(0x58)			// Ｆ１２キー		D_DIK_F12

#define KEY_INPUT_A									(0x1E)			// Ａキー			D_DIK_A
#define KEY_INPUT_B									(0x30)			// Ｂキー			D_DIK_B
#define KEY_INPUT_C									(0x2E)			// Ｃキー			D_DIK_C
#define KEY_INPUT_D									(0x20)			// Ｄキー			D_DIK_D
#define KEY_INPUT_E									(0x12)			// Ｅキー			D_DIK_E
#define KEY_INPUT_F									(0x21)			// Ｆキー			D_DIK_F
#define KEY_INPUT_G									(0x22)			// Ｇキー			D_DIK_G
#define KEY_INPUT_H									(0x23)			// Ｈキー			D_DIK_H
#define KEY_INPUT_I									(0x17)			// Ｉキー			D_DIK_I
#define KEY_INPUT_J									(0x24)			// Ｊキー			D_DIK_J
#define KEY_INPUT_K									(0x25)			// Ｋキー			D_DIK_K
#define KEY_INPUT_L									(0x26)			// Ｌキー			D_DIK_L
#define KEY_INPUT_M									(0x32)			// Ｍキー			D_DIK_M
#define KEY_INPUT_N									(0x31)			// Ｎキー			D_DIK_N
#define KEY_INPUT_O									(0x18)			// Ｏキー			D_DIK_O
#define KEY_INPUT_P									(0x19)			// Ｐキー			D_DIK_P
#define KEY_INPUT_Q									(0x10)			// Ｑキー			D_DIK_Q
#define KEY_INPUT_R									(0x13)			// Ｒキー			D_DIK_R
#define KEY_INPUT_S									(0x1F)			// Ｓキー			D_DIK_S
#define KEY_INPUT_T									(0x14)			// Ｔキー			D_DIK_T
#define KEY_INPUT_U									(0x16)			// Ｕキー			D_DIK_U
#define KEY_INPUT_V									(0x2F)			// Ｖキー			D_DIK_V
#define KEY_INPUT_W									(0x11)			// Ｗキー			D_DIK_W
#define KEY_INPUT_X									(0x2D)			// Ｘキー			D_DIK_X
#define KEY_INPUT_Y									(0x15)			// Ｙキー			D_DIK_Y
#define KEY_INPUT_Z									(0x2C)			// Ｚキー			D_DIK_Z

#define KEY_INPUT_0 								(0x0B)			// ０キー			D_DIK_0
#define KEY_INPUT_1									(0x02)			// １キー			D_DIK_1
#define KEY_INPUT_2									(0x03)			// ２キー			D_DIK_2
#define KEY_INPUT_3									(0x04)			// ３キー			D_DIK_3
#define KEY_INPUT_4									(0x05)			// ４キー			D_DIK_4
#define KEY_INPUT_5									(0x06)			// ５キー			D_DIK_5
#define KEY_INPUT_6									(0x07)			// ６キー			D_DIK_6
#define KEY_INPUT_7									(0x08)			// ７キー			D_DIK_7
#define KEY_INPUT_8									(0x09)			// ８キー			D_DIK_8
#define KEY_INPUT_9									(0x0A)			// ９キー			D_DIK_9

// アスキーコントロールキーコード
#define CTRL_CODE_BS								(0x08)			// バックスペース
#define CTRL_CODE_TAB								(0x09)			// タブ
#define CTRL_CODE_CR								(0x0d)			// 改行
#define CTRL_CODE_DEL								(0x10)			// ＤＥＬキー

#define CTRL_CODE_COPY								(0x03)			// コピー
#define CTRL_CODE_PASTE								(0x16)			// ペースト
#define CTRL_CODE_CUT								(0x18)			// カット
#define CTRL_CODE_ALL								(0x01)			// 全て選択

#define CTRL_CODE_LEFT								(0x1d)			// ←キー
#define CTRL_CODE_RIGHT								(0x1c)			// →キー
#define CTRL_CODE_UP								(0x1e)			// ↑キー
#define CTRL_CODE_DOWN								(0x1f)			// ↓キー

#define CTRL_CODE_HOME								(0x1a)			// ＨＯＭＥボタン
#define CTRL_CODE_END								(0x19)			// ＥＮＤボタン
#define CTRL_CODE_PAGE_UP							(0x17)			// ＰＡＧＥ ＵＰ
#define CTRL_CODE_PAGE_DOWN							(0x15)			// ＰＡＧＥ ＤＯＷＮ

#define CTRL_CODE_ESC								(0x1b)			// ＥＳＣキー
#define CTRL_CODE_CMP								(0x20)			// 制御コード敷居値





// 描画先画面指定用定義
#define DX_SCREEN_FRONT								(0xfffffffc)
#define DX_SCREEN_BACK								(0xfffffffe) 
#define DX_SCREEN_WORK 								(0xfffffffd)
#define DX_SCREEN_TEMPFRONT							(0xfffffff0)
#define DX_SCREEN_OTHER								(0xfffffffa)

	static int MyDxLib_Init();
	static int MyDxLib_End();
	static int MyLoadGraph(const std::string path);
	static void MyDrawGraph(int x,int y,int handle, bool trans);
	static int MyMakeScreen(int x,int y,int alpha);
	static void MyScreenFlip();
	static int MySetDrawScreen(int screen);
	static void MyClsDrawScreen();
	static void MyClearDrawScreen();
	static int MySetDrawBlendMode(int BlendMode,int BlendParam);
	static int MyProcessMessage();
	static int MyCheckHitKey(int keycode);
	static int MySetGraphMode(int ScreenSizeX,int ScreenSizeY ,int ColorBitDepth);
	static int MyChangeWindowMode(int Flag);
	static int MyInitGraph();
	static int MyDeleteGraph(int GrHandle,int LogOutFlag = 0);
	static int MyInitSoundMem(int LogOutFlag = 0);
	static int MyLoadSoundMem(const TCHAR* FileName);
	static int MyDeleteSoundMem(int SoundHandle,int LogOutFlag=0);
	static void MyMV1InitModel();
	static int MyMV1LoadModel(const TCHAR* FileName);
	static int MyMV1DeleteModel(int mHandle);

	//カメラ関係のクッション
	static int MySetCameraNearFar(float Near,float Far);

	//インプット関連

	static int MyGetMousePoint(int* XBuf, int* YBuf);

};

