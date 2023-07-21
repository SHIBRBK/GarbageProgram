#pragma once
#include <Windows.h>
#include<string>
class DxCushion
{
public:
	// �`��u�����h���[�h��`
#define DX_BLENDMODE_NOBLEND						(0)				// �m�[�u�����h
#define DX_BLENDMODE_ALPHA							(1)				// ���u�����h
#define DX_BLENDMODE_ADD							(2)				// ���Z�u�����h
#define DX_BLENDMODE_SUB							(3)				// ���Z�u�����h
#define DX_BLENDMODE_MUL							(4)				// ��Z�u�����h


	// CheckHitKeyAll �Œ��ׂ���̓^�C�v
#define DX_CHECKINPUT_KEY							(0x0001)		// �L�[���͂𒲂ׂ�
#define DX_CHECKINPUT_PAD							(0x0002)		// �p�b�h���͂𒲂ׂ�
#define DX_CHECKINPUT_MOUSE							(0x0004)		// �}�E�X�{�^�����͂𒲂ׂ�
#define DX_CHECKINPUT_ALL							(DX_CHECKINPUT_KEY | DX_CHECKINPUT_PAD | DX_CHECKINPUT_MOUSE)	// ���ׂĂ̓��͂𒲂ׂ�

// �p�b�h���͎擾�p�����[�^
#define DX_INPUT_KEY_PAD1							(0x1001)		// �L�[���͂ƃp�b�h�P����
#define DX_INPUT_PAD1								(0x0001)		// �p�b�h�P����
#define DX_INPUT_PAD2								(0x0002)		// �p�b�h�Q����
#define DX_INPUT_PAD3								(0x0003)		// �p�b�h�R����
#define DX_INPUT_PAD4								(0x0004)		// �p�b�h�S����
#define DX_INPUT_PAD5								(0x0005)		// �p�b�h�T����
#define DX_INPUT_PAD6								(0x0006)		// �p�b�h�U����
#define DX_INPUT_PAD7								(0x0007)		// �p�b�h�V����
#define DX_INPUT_PAD8								(0x0008)		// �p�b�h�W����
#define DX_INPUT_PAD9								(0x0009)		// �p�b�h�X����
#define DX_INPUT_PAD10								(0x000a)		// �p�b�h�P�O����
#define DX_INPUT_PAD11								(0x000b)		// �p�b�h�P�P����
#define DX_INPUT_PAD12								(0x000c)		// �p�b�h�P�Q����
#define DX_INPUT_PAD13								(0x000d)		// �p�b�h�P�R����
#define DX_INPUT_PAD14								(0x000e)		// �p�b�h�P�S����
#define DX_INPUT_PAD15								(0x000f)		// �p�b�h�P�T����
#define DX_INPUT_PAD16								(0x0010)		// �p�b�h�P�U����
#define DX_INPUT_KEY								(0x1000)		// �L�[����

// �Q�[���R���g���[���[�^�C�v
#define DX_PADTYPE_OTHER							(0)
#define DX_PADTYPE_XBOX_360							(1)
#define DX_PADTYPE_XBOX_ONE							(2)
#define DX_PADTYPE_DUAL_SHOCK_4						(3)
#define DX_PADTYPE_DUAL_SENSE						(4)
#define DX_PADTYPE_SWITCH_JOY_CON_L					(5)
#define DX_PADTYPE_SWITCH_JOY_CON_R					(6)
#define DX_PADTYPE_SWITCH_PRO_CTRL					(7)
#define DX_PADTYPE_NUM								(8)

// �^�b�`�̓����ڐG���o�Ή��ő吔
#define TOUCHINPUTPOINT_MAX							(16)

// �p�b�h���͒�`
#define PAD_INPUT_DOWN								(0x00000001)	// ���`�F�b�N�}�X�N
#define PAD_INPUT_LEFT								(0x00000002)	// ���`�F�b�N�}�X�N
#define PAD_INPUT_RIGHT								(0x00000004)	// ���`�F�b�N�}�X�N
#define PAD_INPUT_UP								(0x00000008)	// ���`�F�b�N�}�X�N
#define PAD_INPUT_A									(0x00000010)	// �`�{�^���`�F�b�N�}�X�N
#define PAD_INPUT_B									(0x00000020)	// �a�{�^���`�F�b�N�}�X�N
#define PAD_INPUT_C									(0x00000040)	// �b�{�^���`�F�b�N�}�X�N
#define PAD_INPUT_X									(0x00000080)	// �w�{�^���`�F�b�N�}�X�N
#define PAD_INPUT_Y									(0x00000100)	// �x�{�^���`�F�b�N�}�X�N
#define PAD_INPUT_Z									(0x00000200)	// �y�{�^���`�F�b�N�}�X�N
#define PAD_INPUT_L									(0x00000400)	// �k�{�^���`�F�b�N�}�X�N
#define PAD_INPUT_R									(0x00000800)	// �q�{�^���`�F�b�N�}�X�N
#define PAD_INPUT_START								(0x00001000)	// �r�s�`�q�s�{�^���`�F�b�N�}�X�N
#define PAD_INPUT_M									(0x00002000)	// �l�{�^���`�F�b�N�}�X�N
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

// XInput�{�^�����͒�`
#define XINPUT_BUTTON_DPAD_UP						(0)				// �f�W�^�������{�^����
#define XINPUT_BUTTON_DPAD_DOWN						(1)				// �f�W�^�������{�^����
#define XINPUT_BUTTON_DPAD_LEFT						(2)				// �f�W�^�������{�^����
#define XINPUT_BUTTON_DPAD_RIGHT					(3)				// �f�W�^�������{�^���E
#define XINPUT_BUTTON_START							(4)				// START�{�^��
#define XINPUT_BUTTON_BACK							(5)				// BACK�{�^��
#define XINPUT_BUTTON_LEFT_THUMB					(6)				// ���X�e�B�b�N��������
#define XINPUT_BUTTON_RIGHT_THUMB					(7)				// �E�X�e�B�b�N��������
#define XINPUT_BUTTON_LEFT_SHOULDER					(8)				// LB�{�^��
#define XINPUT_BUTTON_RIGHT_SHOULDER				(9)				// RB�{�^��
#define XINPUT_BUTTON_A								(12)			// A�{�^��
#define XINPUT_BUTTON_B								(13)			// B�{�^��
#define XINPUT_BUTTON_X								(14)			// X�{�^��
#define XINPUT_BUTTON_Y								(15)			// Y�{�^��

// �}�E�X���͒�`
#define MOUSE_INPUT_LEFT							(0x0001)		// �}�E�X���{�^��
#define MOUSE_INPUT_RIGHT							(0x0002)		// �}�E�X�E�{�^��
#define MOUSE_INPUT_MIDDLE							(0x0004)		// �}�E�X�����{�^��
#define MOUSE_INPUT_1								(0x0001)		// �}�E�X�P�{�^��
#define MOUSE_INPUT_2								(0x0002)		// �}�E�X�Q�{�^��
#define MOUSE_INPUT_3								(0x0004)		// �}�E�X�R�{�^��
#define MOUSE_INPUT_4								(0x0008)		// �}�E�X�S�{�^��
#define MOUSE_INPUT_5								(0x0010)		// �}�E�X�T�{�^��
#define MOUSE_INPUT_6								(0x0020)		// �}�E�X�U�{�^��
#define MOUSE_INPUT_7								(0x0040)		// �}�E�X�V�{�^��
#define MOUSE_INPUT_8								(0x0080)		// �}�E�X�W�{�^��

// �}�E�X�̃��O���^�C�v
#define MOUSE_INPUT_LOG_DOWN						(0)				// �{�^����������
#define MOUSE_INPUT_LOG_UP							(1)				// �{�^���𗣂���

// �L�[��`
#define KEY_INPUT_BACK								(0x0E)			// BackSpace�L�[	D_DIK_BACK
#define KEY_INPUT_TAB								(0x0F)			// Tab�L�[			D_DIK_TAB
#define KEY_INPUT_RETURN							(0x1C)			// Enter�L�[		D_DIK_RETURN

#define KEY_INPUT_LSHIFT							(0x2A)			// ��Shift�L�[		D_DIK_LSHIFT
#define KEY_INPUT_RSHIFT							(0x36)			// �EShift�L�[		D_DIK_RSHIFT
#define KEY_INPUT_LCONTROL							(0x1D)			// ��Ctrl�L�[		D_DIK_LCONTROL
#define KEY_INPUT_RCONTROL							(0x9D)			// �ECtrl�L�[		D_DIK_RCONTROL
#define KEY_INPUT_ESCAPE							(0x01)			// Esc�L�[			D_DIK_ESCAPE
#define KEY_INPUT_SPACE								(0x39)			// �X�y�[�X�L�[		D_DIK_SPACE
#define KEY_INPUT_PGUP								(0xC9)			// PageUp�L�[		D_DIK_PGUP
#define KEY_INPUT_PGDN								(0xD1)			// PageDown�L�[		D_DIK_PGDN
#define KEY_INPUT_END								(0xCF)			// End�L�[			D_DIK_END
#define KEY_INPUT_HOME								(0xC7)			// Home�L�[			D_DIK_HOME
#define KEY_INPUT_LEFT								(0xCB)			// ���L�[			D_DIK_LEFT
#define KEY_INPUT_UP								(0xC8)			// ��L�[			D_DIK_UP
#define KEY_INPUT_RIGHT								(0xCD)			// �E�L�[			D_DIK_RIGHT
#define KEY_INPUT_DOWN								(0xD0)			// ���L�[			D_DIK_DOWN
#define KEY_INPUT_INSERT							(0xD2)			// Insert�L�[		D_DIK_INSERT
#define KEY_INPUT_DELETE							(0xD3)			// Delete�L�[		D_DIK_DELETE

#define KEY_INPUT_MINUS								(0x0C)			// �|�L�[			D_DIK_MINUS
#define KEY_INPUT_YEN								(0x7D)			// ���L�[			D_DIK_YEN
#define KEY_INPUT_PREVTRACK							(0x90)			// �O�L�[			D_DIK_PREVTRACK
#define KEY_INPUT_PERIOD							(0x34)			// �D�L�[			D_DIK_PERIOD
#define KEY_INPUT_SLASH								(0x35)			// �^�L�[			D_DIK_SLASH
#define KEY_INPUT_LALT								(0x38)			// ��Alt�L�[		D_DIK_LALT
#define KEY_INPUT_RALT								(0xB8)			// �EAlt�L�[		D_DIK_RALT
#define KEY_INPUT_SCROLL							(0x46)			// ScrollLock�L�[	D_DIK_SCROLL
#define KEY_INPUT_SEMICOLON							(0x27)			// �G�L�[			D_DIK_SEMICOLON
#define KEY_INPUT_COLON								(0x92)			// �F�L�[			D_DIK_COLON
#define KEY_INPUT_LBRACKET							(0x1A)			// �m�L�[			D_DIK_LBRACKET
#define KEY_INPUT_RBRACKET							(0x1B)			// �n�L�[			D_DIK_RBRACKET
#define KEY_INPUT_AT								(0x91)			// ���L�[			D_DIK_AT
#define KEY_INPUT_BACKSLASH							(0x2B)			// �_�L�[			D_DIK_BACKSLASH
#define KEY_INPUT_COMMA								(0x33)			// �C�L�[			D_DIK_COMMA
#define KEY_INPUT_KANJI								(0x94)			// �����L�[			D_DIK_KANJI
#define KEY_INPUT_CONVERT							(0x79)			// �ϊ��L�[			D_DIK_CONVERT
#define KEY_INPUT_NOCONVERT							(0x7B)			// ���ϊ��L�[		D_DIK_NOCONVERT
#define KEY_INPUT_KANA								(0x70)			// �J�i�L�[			D_DIK_KANA
#define KEY_INPUT_APPS								(0xDD)			// �A�v���P�[�V�������j���[�L�[		D_DIK_APPS
#define KEY_INPUT_CAPSLOCK							(0x3A)			// CaspLock�L�[		D_DIK_CAPSLOCK
#define KEY_INPUT_SYSRQ								(0xB7)			// PrintScreen�L�[	D_DIK_SYSRQ
#define KEY_INPUT_PAUSE								(0xC5)			// PauseBreak�L�[	D_DIK_PAUSE
#define KEY_INPUT_LWIN								(0xDB)			// ��Win�L�[		D_DIK_LWIN
#define KEY_INPUT_RWIN								(0xDC)			// �EWin�L�[		D_DIK_RWIN

#define KEY_INPUT_NUMLOCK							(0x45)			// �e���L�[NumLock�L�[		D_DIK_NUMLOCK
#define KEY_INPUT_NUMPAD0							(0x52)			// �e���L�[�O				D_DIK_NUMPAD0
#define KEY_INPUT_NUMPAD1							(0x4F)			// �e���L�[�P				D_DIK_NUMPAD1
#define KEY_INPUT_NUMPAD2							(0x50)			// �e���L�[�Q				D_DIK_NUMPAD2
#define KEY_INPUT_NUMPAD3							(0x51)			// �e���L�[�R				D_DIK_NUMPAD3
#define KEY_INPUT_NUMPAD4							(0x4B)			// �e���L�[�S				D_DIK_NUMPAD4
#define KEY_INPUT_NUMPAD5							(0x4C)			// �e���L�[�T				D_DIK_NUMPAD5
#define KEY_INPUT_NUMPAD6							(0x4D)			// �e���L�[�U				D_DIK_NUMPAD6
#define KEY_INPUT_NUMPAD7							(0x47)			// �e���L�[�V				D_DIK_NUMPAD7
#define KEY_INPUT_NUMPAD8							(0x48)			// �e���L�[�W				D_DIK_NUMPAD8
#define KEY_INPUT_NUMPAD9							(0x49)			// �e���L�[�X				D_DIK_NUMPAD9
#define KEY_INPUT_MULTIPLY							(0x37)			// �e���L�[���L�[			D_DIK_MULTIPLY
#define KEY_INPUT_ADD								(0x4E)			// �e���L�[�{�L�[			D_DIK_ADD
#define KEY_INPUT_SUBTRACT							(0x4A)			// �e���L�[�|�L�[			D_DIK_SUBTRACT
#define KEY_INPUT_DECIMAL							(0x53)			// �e���L�[�D�L�[			D_DIK_DECIMAL
#define KEY_INPUT_DIVIDE							(0xB5)			// �e���L�[�^�L�[			D_DIK_DIVIDE
#define KEY_INPUT_NUMPADENTER						(0x9C)			// �e���L�[�̃G���^�[�L�[	D_DIK_NUMPADENTER

#define KEY_INPUT_F1								(0x3B)			// �e�P�L�[			D_DIK_F1
#define KEY_INPUT_F2								(0x3C)			// �e�Q�L�[			D_DIK_F2
#define KEY_INPUT_F3								(0x3D)			// �e�R�L�[			D_DIK_F3
#define KEY_INPUT_F4								(0x3E)			// �e�S�L�[			D_DIK_F4
#define KEY_INPUT_F5								(0x3F)			// �e�T�L�[			D_DIK_F5
#define KEY_INPUT_F6								(0x40)			// �e�U�L�[			D_DIK_F6
#define KEY_INPUT_F7								(0x41)			// �e�V�L�[			D_DIK_F7
#define KEY_INPUT_F8								(0x42)			// �e�W�L�[			D_DIK_F8
#define KEY_INPUT_F9								(0x43)			// �e�X�L�[			D_DIK_F9
#define KEY_INPUT_F10								(0x44)			// �e�P�O�L�[		D_DIK_F10
#define KEY_INPUT_F11								(0x57)			// �e�P�P�L�[		D_DIK_F11
#define KEY_INPUT_F12								(0x58)			// �e�P�Q�L�[		D_DIK_F12

#define KEY_INPUT_A									(0x1E)			// �`�L�[			D_DIK_A
#define KEY_INPUT_B									(0x30)			// �a�L�[			D_DIK_B
#define KEY_INPUT_C									(0x2E)			// �b�L�[			D_DIK_C
#define KEY_INPUT_D									(0x20)			// �c�L�[			D_DIK_D
#define KEY_INPUT_E									(0x12)			// �d�L�[			D_DIK_E
#define KEY_INPUT_F									(0x21)			// �e�L�[			D_DIK_F
#define KEY_INPUT_G									(0x22)			// �f�L�[			D_DIK_G
#define KEY_INPUT_H									(0x23)			// �g�L�[			D_DIK_H
#define KEY_INPUT_I									(0x17)			// �h�L�[			D_DIK_I
#define KEY_INPUT_J									(0x24)			// �i�L�[			D_DIK_J
#define KEY_INPUT_K									(0x25)			// �j�L�[			D_DIK_K
#define KEY_INPUT_L									(0x26)			// �k�L�[			D_DIK_L
#define KEY_INPUT_M									(0x32)			// �l�L�[			D_DIK_M
#define KEY_INPUT_N									(0x31)			// �m�L�[			D_DIK_N
#define KEY_INPUT_O									(0x18)			// �n�L�[			D_DIK_O
#define KEY_INPUT_P									(0x19)			// �o�L�[			D_DIK_P
#define KEY_INPUT_Q									(0x10)			// �p�L�[			D_DIK_Q
#define KEY_INPUT_R									(0x13)			// �q�L�[			D_DIK_R
#define KEY_INPUT_S									(0x1F)			// �r�L�[			D_DIK_S
#define KEY_INPUT_T									(0x14)			// �s�L�[			D_DIK_T
#define KEY_INPUT_U									(0x16)			// �t�L�[			D_DIK_U
#define KEY_INPUT_V									(0x2F)			// �u�L�[			D_DIK_V
#define KEY_INPUT_W									(0x11)			// �v�L�[			D_DIK_W
#define KEY_INPUT_X									(0x2D)			// �w�L�[			D_DIK_X
#define KEY_INPUT_Y									(0x15)			// �x�L�[			D_DIK_Y
#define KEY_INPUT_Z									(0x2C)			// �y�L�[			D_DIK_Z

#define KEY_INPUT_0 								(0x0B)			// �O�L�[			D_DIK_0
#define KEY_INPUT_1									(0x02)			// �P�L�[			D_DIK_1
#define KEY_INPUT_2									(0x03)			// �Q�L�[			D_DIK_2
#define KEY_INPUT_3									(0x04)			// �R�L�[			D_DIK_3
#define KEY_INPUT_4									(0x05)			// �S�L�[			D_DIK_4
#define KEY_INPUT_5									(0x06)			// �T�L�[			D_DIK_5
#define KEY_INPUT_6									(0x07)			// �U�L�[			D_DIK_6
#define KEY_INPUT_7									(0x08)			// �V�L�[			D_DIK_7
#define KEY_INPUT_8									(0x09)			// �W�L�[			D_DIK_8
#define KEY_INPUT_9									(0x0A)			// �X�L�[			D_DIK_9

// �A�X�L�[�R���g���[���L�[�R�[�h
#define CTRL_CODE_BS								(0x08)			// �o�b�N�X�y�[�X
#define CTRL_CODE_TAB								(0x09)			// �^�u
#define CTRL_CODE_CR								(0x0d)			// ���s
#define CTRL_CODE_DEL								(0x10)			// �c�d�k�L�[

#define CTRL_CODE_COPY								(0x03)			// �R�s�[
#define CTRL_CODE_PASTE								(0x16)			// �y�[�X�g
#define CTRL_CODE_CUT								(0x18)			// �J�b�g
#define CTRL_CODE_ALL								(0x01)			// �S�đI��

#define CTRL_CODE_LEFT								(0x1d)			// ���L�[
#define CTRL_CODE_RIGHT								(0x1c)			// ���L�[
#define CTRL_CODE_UP								(0x1e)			// ���L�[
#define CTRL_CODE_DOWN								(0x1f)			// ���L�[

#define CTRL_CODE_HOME								(0x1a)			// �g�n�l�d�{�^��
#define CTRL_CODE_END								(0x19)			// �d�m�c�{�^��
#define CTRL_CODE_PAGE_UP							(0x17)			// �o�`�f�d �t�o
#define CTRL_CODE_PAGE_DOWN							(0x15)			// �o�`�f�d �c�n�v�m

#define CTRL_CODE_ESC								(0x1b)			// �d�r�b�L�[
#define CTRL_CODE_CMP								(0x20)			// ����R�[�h�~���l





// �`����ʎw��p��`
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

	//�J�����֌W�̃N�b�V����
	static int MySetCameraNearFar(float Near,float Far);

	//�C���v�b�g�֘A

	static int MyGetMousePoint(int* XBuf, int* YBuf);

};

