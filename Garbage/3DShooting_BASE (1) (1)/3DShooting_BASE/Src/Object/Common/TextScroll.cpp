#include "TextScroll.h"
#include"../../Manager/ResourceManager.h"
#include"../../Common/Quaternion.h"
#include"../../Utility/AsoUtility.h"

TextScroll::TextScroll()
{
}

TextScroll::~TextScroll()
{
}

void TextScroll::Init(void)
{

	mResource = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::ALPHABET);

	//�e�L�X�g���[���̎�ʂ������ݒ�
	mType = TYPE::TITLE;

	//�P�s������̏��
	std::vector<MsgInfo>infos;

	//���͂̐ݒ�
	infos.push_back(MakeMsgInfo("Arima Kinen.", infos.size()));
	infos.push_back(MakeMsgInfo("winner .Tokai Teio.", infos.size()));
	infos.push_back(MakeMsgInfo("runner up .Biwa Hayahide.", infos.size()));
	infos.push_back(MakeMsgInfo("placed.Nice Nature.", infos.size()));
	infos.push_back(MakeMsgInfo("placed.Matikanetannhauser.", infos.size()));
	infos.push_back(MakeMsgInfo("placed.Legacy World.", infos.size()));
	infos.push_back(MakeMsgInfo("Mejiro Palmer.", infos.size()));
	infos.push_back(MakeMsgInfo("Sekitei Ryu O.", infos.size()));
	infos.push_back(MakeMsgInfo("Rice Shower.", infos.size()));
	infos.push_back(MakeMsgInfo("Vega.", infos.size()));
	infos.push_back(MakeMsgInfo("Wish Dream.", infos.size()));
	infos.push_back(MakeMsgInfo("Winning Ticket.", infos.size()));
	infos.push_back(MakeMsgInfo("El Casa River.", infos.size()));
	infos.push_back(MakeMsgInfo("L�[Way Win.", infos.size()));
	infos.push_back(MakeMsgInfo("White Stone.", infos.size()));


	mTextMap.emplace(TYPE::TITLE, infos);

	//�ړ������i���K�|�C���g�@�j�����x�N�g��
	Quaternion rot = Quaternion::Euler(AsoUtility::Deg2RadF(-40.0f), 0.0f, 0.0f);
	//mVelocity =Quaternion::AngleAxis(rot,);
	mVelocity = rot.GetForward();
	//�ړ��ʁi�����~���x�j
	mVelocity=VScale(mVelocity, SPEED_MOVE);

}

void TextScroll::Update(void)
{
	std::vector<MsgInfo>& infos = mTextMap[mType];

	for (auto& info : infos)
	{
		//�����̈ړ�����
		info.pos = VAdd(info.pos, mVelocity);
	}
}

void TextScroll::Draw(void)
{
	const std::vector<MsgInfo>& infos = mTextMap[mType];

	VECTOR pos;

	float plusX;

	for (auto& info : infos)
	{

		plusX = 0.0f;
		for (auto& id:info.messages)
		{

			pos = info.pos;
			pos.x += plusX;

			//���K�A�@���ɍ��W�����炷
			DrawBillboard3D(
				pos, 0.5f, 0.5f, SIZE_3D, 0.0f,
				mResource.mHandleIds[id],true
			);
			plusX += SIZE_3D;
		}
		
	}

}

void TextScroll::Release(void)
{
}

TextScroll::MsgInfo TextScroll::MakeMsgInfo(const std::string& msg, size_t mapCount)
{
	MsgInfo ret;

	//�������摜�z��̓Y�����ɕϊ�
	std::vector<int> messages;

	int ascii;
	int idx ;

	size_t len = msg.size();

	for (int i=0;i<len;i++)
	{
		
		//���K�B�ϊ�����������������

		idx = 0;

		//�ꕶ����ASCII�R�[�h�ɕϊ�
		ascii = msg.at(i);

		if (ascii==32)
		{
			idx = 52;
		}


		if (ascii==46)
		{
			idx = 53;
		}
		//(32��������X�y�[�X�i�摜�Y�����T�Q�j)
		if (ascii == 44)
		{
			idx = 54;
		}
		//�啶��
		if (ascii >=65&& ascii<=90)
		{
			idx = ascii-65;
		}
		//������
		if (ascii >= 97 && ascii <= 122)
		{
			idx = ascii-71;
		}
		messages.emplace_back(idx);
	}

	

	//�C�s���Ƃ̍��W
	//ret.pos = VECTOR();
	ret.pos =
	{
		-400.0f,
		START_Y - (mapCount * SIZE_3D),
		START_Z - (mapCount * SIZE_3D),
	};

	//���W�ƕϊ����ꂽID��������
	ret.messages = messages;

	return ret;
}
