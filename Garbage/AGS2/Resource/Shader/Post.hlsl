
// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 dif : COLOR0;
    float2 uv : TEXCOORD0;
};

struct PS_OUTPUT
{
    float3 pos : SV_POSITION;
    float4 dif : COLOR0;
    float2 uv : TEXCOORD0;
};


cbuffer Send : register(b6)
{
    float textureWidth; // テクスチャの横大きさ
    float textureHeight; // テクスチャの縦大きさ
    float time ;
}

// C++ 側で設定する定数の定義

// 描画するテクスチャ
SamplerState smp : register(s4);
Texture2D screen : register(t4);


// main関数
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv / (textureWidth * textureHeight);

    float amount = 0.0;
	
    amount = (1.0 + sin(time * 6.0)) * 0.5;
    amount *= 1.0 + sin(time * 16.0) * 0.5;
    amount *= 1.0 + sin(time * 19.0) * 0.5;
    amount *= 1.0 + sin(time * 27.0) * 0.5;
    amount = pow(amount, 3.0);

    amount *= 0.05;
	
    float3 col;
    col.r = screen.Sample(smp, float2(uv.x + amount, uv.y)).r;
    col.g = screen.Sample(smp, uv).g;
    col.b = screen.Sample(smp, float2(uv.x - amount, uv.y)).b;

    col *= (1.0 - amount * 0.5);
	
    return float4(col, 1.0);
}