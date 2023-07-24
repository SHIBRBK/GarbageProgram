SamplerState sam : register(s0);
Texture2D currentTex : register(t0);
Texture2D Prevtex : register(t1);

struct PS_INPUT
{
    float4 Pos : SV_POSITION; // �s�N�Z���̃X�N���[�����W
    float4 dif : COLOR0;
    float4 spec : COLOR1;
    float2 uv0 : TEXCOORD0; // �s�N�Z���̃e�N�X�`�����W
};

struct PS_OUTPUT
{
    float4 Out : SV_TARGET0; // �s�N�Z���̏o�͐F
};

cbuffer tensou : register(b5)
{
    float textureWidth; // �e�N�X�`���̉��傫��
    float textureHeight; // �e�N�X�`���̏c�傫��
    float cont;
}

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT PSOutput;
    // �g���������̃p�����[�^
    float c = 0.5f; // �g�̓`�d���x�̌W��


    // �אڃs�N�Z���̍����f�[�^,
   // float2 pixelUV = float2(1.0f / textureWidth, 1.0f / textureHeight);
    
    float2 left = input.uv0 - float2(1.0f / textureWidth, 0.0f);
    float2 right = input.uv0 + float2(1.0f / textureWidth, 0.0f);
    float2 up = input.uv0 + float2(0.0f, 1.0f / textureHeight);
    float2 down = input.uv0 - float2(0.0f, 1.0f / textureHeight);
        
    float2 currentH = currentTex.Sample(sam, input.uv0);

    // �O�t���[���̍����f�[�^(����������΍����Ă�t���[��)
    float2 prevH = Prevtex.Sample(sam, input.uv0);
    float2 nextH = (2.0f * currentH) - prevH + c *
    (
    left +
    right +
     up +
     down - 4.0f * currentH) * 0.5f;

    //�����`�����v�Z�p�̃V�F�[�_���ԈႦ�Ă邩�炱�����̕���������

    //float2 u = tex.Sample(sam,nextH);
    PSOutput.Out.r = nextH * 2 - 1;
    PSOutput.Out.g = currentH*2-1;
    //PSOutput.Color =input.dif* 2 - 1;
    return PSOutput;
}