SamplerState sam : register(s0);
Texture2D tex : register(t0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION; // �s�N�Z���̃X�N���[�����W
    float4 dif : COLOR0;
    float4 spec : COLOR1;
    float4 uv0 : TEXCOORD0; // �s�N�Z���̃e�N�X�`�����W
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
    float c = 10.0f; // �g�̓`�d���x�̌W��


    // �אڃs�N�Z���̍����f�[�^,
   // float2 pixelUV = float2(1.0f / textureWidth, 1.0f / textureHeight);
    
    float2 left = input.uv0.xz - float2(1.0f / textureWidth, 0.0f);
    float2 right = input.uv0.xz + float2(1.0f / textureWidth, 0.0f);
    float2 up = input.uv0.zy + float2(0.0f, 1.0f / textureHeight);
    float2 down = input.uv0.zy - float2(0.0f, 1.0f / textureHeight);
        
    //����
    float4 Height;
    Height = tex.Sample(sam, input.uv0.xy);

    // �O�t���[���̍����f�[�^(����������΍����Ă�t���[��)
    //�ߋ�

    // (2.0f * Height.r) - Height.g + c *
    float nextH = 
    tex.Sample(sam, left).r +
    tex.Sample(sam, right).r +
    tex.Sample(sam, up).r +
    tex.Sample(sam, down).r - 4.0f * Height.r;
    
    nextH = (2 * (Height.r * 2 - Height.g + nextH * c) - 1) * 0.99;

    
    //float2 nextH = (2.0f * Height.r) - Height.g + c *
    //(
    //right +
    //left +
    // up +
    // down - 4.0f * Height.r) * 0.5f;

    //�����`�����v�Z�p�̃V�F�[�_���ԈႦ�Ă邩�炱�����̕���������

    //float2 u = tex.Sample(sam,nextH);
    PSOutput.Out.r = (nextH+1) *0.5 ;
    PSOutput.Out.g = Height.r;
   // PSOutput.Out.g = currentH*2-1;
    //PSOutput.Color =input.dif* 2 - 1;
    return PSOutput;
}