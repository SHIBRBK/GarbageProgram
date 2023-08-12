// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
    float4 MirrorProjectionPosition : TEXCOORD0; // ���ɉf��f���̎��_�ł̎ˉe���W
    float4 Diffuse : COLOR0; // �f�B�t���[�Y�J���[
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
    float4 Color0 : SV_TARGET;
};


// C++ ���Őݒ肷��萔�̒�`
SamplerState MirrorTexture : register(s0); // ���ɉf���e�N�X�`��

Texture2D textured : register(t0);
float4 cfMulColor : register(c0); // �e�N�X�`���F�ɏ�Z����F


// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
    float2 TexCoords;
    float4 TexColor;

	// �e�N�X�`�����W�̎Z�o
    TexCoords.x = PSInput.MirrorProjectionPosition.x / PSInput.MirrorProjectionPosition.w;
    TexCoords.y = PSInput.MirrorProjectionPosition.y / PSInput.MirrorProjectionPosition.w;

	// ���ɉf���e�N�X�`������F���擾
    TexColor = textured.Sample(MirrorTexture, TexCoords);

	// �f�B�t���[�Y�J���[�Ə�Z���ďo�͂ɃZ�b�g
    PSOutput.Color0 = TexColor * PSInput.Diffuse;
   
    return PSOutput;
}
