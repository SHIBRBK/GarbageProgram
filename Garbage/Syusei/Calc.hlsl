SamplerState sam : register(s0);
Texture2D currentTex : register(t0);
Texture2D Prevtex : register(t1);

struct PS_INPUT
{
    float4 Pos : SV_POSITION; // ピクセルのスクリーン座標
    float4 dif : COLOR0;
    float4 spec : COLOR1;
    float2 uv0 : TEXCOORD0; // ピクセルのテクスチャ座標
};

struct PS_OUTPUT
{
    float4 Out : SV_TARGET0; // ピクセルの出力色
};

cbuffer tensou : register(b5)
{
    float textureWidth; // テクスチャの横大きさ
    float textureHeight; // テクスチャの縦大きさ
    float cont;
}

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT PSOutput;
    // 波動方程式のパラメータ
    float c = 0.5f; // 波の伝播速度の係数


    // 隣接ピクセルの高さデータ,
   // float2 pixelUV = float2(1.0f / textureWidth, 1.0f / textureHeight);
    
    float2 left = input.uv0 - float2(1.0f / textureWidth, 0.0f);
    float2 right = input.uv0 + float2(1.0f / textureWidth, 0.0f);
    float2 up = input.uv0 + float2(0.0f, 1.0f / textureHeight);
    float2 down = input.uv0 - float2(0.0f, 1.0f / textureHeight);
        
    float2 currentH = currentTex.Sample(sam, input.uv0);

    // 前フレームの高さデータ(言い換えれば今見てるフレーム)
    float2 prevH = Prevtex.Sample(sam, input.uv0);
    float2 nextH = (2.0f * currentH) - prevH + c *
    (
    left +
    right +
     up +
     down - 4.0f * currentH) * 0.5f;

    //ワンチャン計算用のシェーダが間違えてるからこっちの方を見張る

    //float2 u = tex.Sample(sam,nextH);
    PSOutput.Out.r = nextH * 2 - 1;
    PSOutput.Out.g = currentH*2-1;
    //PSOutput.Color =input.dif* 2 - 1;
    return PSOutput;
}