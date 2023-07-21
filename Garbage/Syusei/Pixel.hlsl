SamplerState sam : register(s0);
Texture2D tex : register(t0);

struct PS_INPUT//今回、別名  VS_OUTPUT
{
    float4 Pos : SV_POSITION; // ピクセルのスクリーン座標
    float2 Noraml : NORMAL; // ピクセルの法線
    float4 dif : COLOR0;
    float4 spec : COLOR1;
    float4 uv0 : TEXCOORD0; // ピクセルのテクスチャ座標
};
//


cbuffer tensou : register(b5)
{
    float textureWidth; // テクスチャの横大きさ
    float textureHeight; // テクスチャの縦大きさ
    float constant; // 定数
}


float4 main(PS_INPUT input) : SV_TARGET
{

    
    return input.uv0; // = float4(nextH.r, prevH.g, 0.0f, 1.0f);

}