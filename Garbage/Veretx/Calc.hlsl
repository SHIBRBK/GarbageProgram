SamplerState sam : register(s0);
Texture2D tex : register(t0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION; // ピクセルのスクリーン座標
    float4 dif : COLOR0;
    float4 spec : COLOR1;
    float4 uv0 : TEXCOORD0; // ピクセルのテクスチャ座標
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
    float c = 10.0f; // 波の伝播速度の係数


    // 隣接ピクセルの高さデータ,
   // float2 pixelUV = float2(1.0f / textureWidth, 1.0f / textureHeight);
    
    float2 left = input.uv0.xz - float2(1.0f / textureWidth, 0.0f);
    float2 right = input.uv0.xz + float2(1.0f / textureWidth, 0.0f);
    float2 up = input.uv0.zy + float2(0.0f, 1.0f / textureHeight);
    float2 down = input.uv0.zy - float2(0.0f, 1.0f / textureHeight);
        
    //現在
    float4 Height;
    Height = tex.Sample(sam, input.uv0.xy);

    // 前フレームの高さデータ(言い換えれば今見てるフレーム)
    //過去

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

    //ワンチャン計算用のシェーダが間違えてるからこっちの方を見張る

    //float2 u = tex.Sample(sam,nextH);
    PSOutput.Out.r = (nextH+1) *0.5 ;
    PSOutput.Out.g = Height.r;
   // PSOutput.Out.g = currentH*2-1;
    //PSOutput.Color =input.dif* 2 - 1;
    return PSOutput;
}