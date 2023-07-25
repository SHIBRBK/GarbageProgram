SamplerState testSamp : register(s1);
Texture2D g_SrcTexture : register(t1);

struct VS_INPUT
{
    float3 pos : POSITION0; // ���W(���[�J�����)
    float3 norm : NORMAL; // �@��(���[�J�����)   �@��₱������肪�E�E�E3D�͓\��t���������ɂ���Ė@���������R���R���ς��B
    float3 tan : TANGENT0; // �ڐ�(���[�J�����)   �����ł����� �\�ʂ̍��W�n�B�@���}�b�v�𐳂����\��t���邽�߂̍��W�n�������ō���Ă�B���T�͂������Ȃ��Ă��@���}�b�v������Ƃ������Ƃ��������B
    float3 bin : BINORMAL0; // �]�@��(���[�J�����)
    float4 diffuse : COLOR0; // �f�B�t���[�Y�J���[
    float4 specular : COLOR1; // �X�y�L�����J���[
    float2 uv0 : TEXCOORD0; // �e�N�X�`�����W
    float2 uv1 : TEXCOORD1; // �T�u�e�N�X�`�����W
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 dif : COLOR00;
    float4 spc : COLOR01;
    float3 Normal : NORMAL;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
};

// ��{�p�����[�^
struct DX_D3D11_VS_CONST_BUFFER_BASE    //�Ȃ�Ă������A�����͒��_�V�F�[�_�������ۂ̃e���v���I�Ȋ����ɂȂ����B
{
    float4 AntiViewportMatrix[4]; // �A���`�r���[�|�[�g�s��
    float4 ProjectionMatrix[4]; // �r���[�@���@�v���W�F�N�V�����s��
    float4 ViewMatrix[3]; // ���[���h�@���@�r���[�s��
    float4 LocalWorldMatrix[3]; // ���[�J���@���@���[���h�s��

    float4 ToonOutLineSize; // �g�D�[���̗֊s���̑傫��
    float DiffuseSource; // �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
    float SpecularSource; // �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
    float MulSpecularColor; // �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
    float Padding;
};

// ���̑��̍s��
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX //�������e���v���I��
{

    // �V���h�E�}�b�v�p�̃��C�g�r���[�s��ƃ��C�g�ˉe�s�����Z��������
    float4 ShadowMapLightViewProjectionMatrix[3][4];

    // �e�N�X�`�����W����p�s��
    float4 TextureMatrix[3][2];
};



// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_VS_BASE : register(b1)
{
    DX_D3D11_VS_CONST_BUFFER_BASE base;
};

// ���̑��̍s��
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX : register(b2)
{
    DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX otherMatrix;
};

cbuffer EMERGENCY : register(b5)
{
    float width;
    float height;
    float constants;
};


float4 ComputeLaplacian(float3 position, float3 neighbor1,
    float3 neighbor2, float3 neighbor3, float3 neighbor4)
{
    float3 laplacian = neighbor1 + neighbor2 + neighbor3 + neighbor4 - 4.0 * position;
    return float4(laplacian, 0.0);
}

VS_OUTPUT main(VS_INPUT input)
{
    float3 light = normalize(float3(1, 1, 1));
    VS_OUTPUT ret;
    
    float4 lLocalPosition;
    float4 lWorldPosition;
    float4 lViewPosition;
    float3 lWorldNrm;
    float3 lViewNrm;

       // �e�N�X�`�����W�̌v�Z
    //float2 texCoord = float2(input.uv0.x * width, input.uv0.y * height);

// �e�N�X�`�����獂�������T���v�����O
    float heightValue = g_SrcTexture.SampleLevel(testSamp, input.uv0, 0).r ;
    //float4 TexColor;
    //int3 TexPosition;

    lLocalPosition.xyz = input.pos;
    lLocalPosition.w = 1.0f;
    lLocalPosition.y += heightValue * constants;
    // ���W�v�Z( ���[�J�����r���[���v���W�F�N�V���� )
    lWorldPosition.x = dot(lLocalPosition, base.LocalWorldMatrix[0]);
    lWorldPosition.y = dot(lLocalPosition, base.LocalWorldMatrix[1]);
    lWorldPosition.z = dot(lLocalPosition, base.LocalWorldMatrix[2]);
    lWorldPosition.w = 1.0f;
    //�����܂ł����[�J��

    //�������炪�r���[
    lViewPosition.x = dot(lWorldPosition, base.ViewMatrix[0]);
    lViewPosition.y = dot(lWorldPosition, base.ViewMatrix[1]);
    lViewPosition.z = dot(lWorldPosition, base.ViewMatrix[2]);
    lViewPosition.w = 1.0f;
    //�����܂ł��r���[


    //�������炪�v���W�F�N�V����
    ret.pos.x = dot(lViewPosition, base.ProjectionMatrix[0]);
    ret.pos.y = dot(lViewPosition, base.ProjectionMatrix[1]);
    ret.pos.z = dot(lViewPosition, base.ProjectionMatrix[2]);
    ret.pos.w = dot(lViewPosition, base.ProjectionMatrix[3]);
    //�����܂ł��v���W�F�N�V����
    // ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )

    
    // �o�̓p�����[�^�Z�b�g ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )
    
    ret.dif = input.diffuse * (max(saturate(dot(input.norm, light)), 0.3));
    ret.spc = input.specular;
    
    return ret;
}