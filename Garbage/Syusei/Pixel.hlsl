SamplerState sam : register(s0);
Texture2D tex : register(t0);

struct PS_INPUT//����A�ʖ�  VS_OUTPUT
{
    float4 Pos : SV_POSITION; // �s�N�Z���̃X�N���[�����W
    float2 Noraml : NORMAL; // �s�N�Z���̖@��
    float4 dif : COLOR0;
    float4 spec : COLOR1;
    float4 uv0 : TEXCOORD0; // �s�N�Z���̃e�N�X�`�����W
};
//


cbuffer tensou : register(b5)
{
    float textureWidth; // �e�N�X�`���̉��傫��
    float textureHeight; // �e�N�X�`���̏c�傫��
    float constant; // �萔
}


float4 main(PS_INPUT input) : SV_TARGET
{

    
    return input.uv0; // = float4(nextH.r, prevH.g, 0.0f, 1.0f);

}