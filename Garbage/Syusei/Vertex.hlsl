SamplerState testSamp : register(s1);
Texture2D g_SrcTexture : register(t1);

struct VS_INPUT
{
    float3 pos : POSITION0; // 座標(ローカル空間)
    float3 norm : NORMAL; // 法線(ローカル空間)   　ややこしい問題が・・・3Dは貼り付けた方向によって法線方向がコロコロ変わる。
    float3 tan : TANGENT0; // 接線(ローカル空間)   そこでこいつら 表面の座標系。法線マップを正しく貼り付けるための座標系をここで作ってる。来週はここがなくても法線マップを作れるということを説明する。
    float3 bin : BINORMAL0; // 従法線(ローカル空間)
    float4 diffuse : COLOR0; // ディフューズカラー
    float4 specular : COLOR1; // スペキュラカラー
    float2 uv0 : TEXCOORD0; // テクスチャ座標
    float2 uv1 : TEXCOORD1; // サブテクスチャ座標
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

// 基本パラメータ
struct DX_D3D11_VS_CONST_BUFFER_BASE    //なんていうか、こいつは頂点シェーダを書く際のテンプレ的な感じになりつつある。
{
    float4 AntiViewportMatrix[4]; // アンチビューポート行列
    float4 ProjectionMatrix[4]; // ビュー　→　プロジェクション行列
    float4 ViewMatrix[3]; // ワールド　→　ビュー行列
    float4 LocalWorldMatrix[3]; // ローカル　→　ワールド行列

    float4 ToonOutLineSize; // トゥーンの輪郭線の大きさ
    float DiffuseSource; // ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
    float SpecularSource; // スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
    float MulSpecularColor; // スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
    float Padding;
};

// その他の行列
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX //こいつもテンプレ的な
{

    // シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの
    float4 ShadowMapLightViewProjectionMatrix[3][4];

    // テクスチャ座標操作用行列
    float4 TextureMatrix[3][2];
};



// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_VS_BASE : register(b1)
{
    DX_D3D11_VS_CONST_BUFFER_BASE base;
};

// その他の行列
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

       // テクスチャ座標の計算
    //float2 texCoord = float2(input.uv0.x * width, input.uv0.y * height);

// テクスチャから高さ情報をサンプリング
    float heightValue = g_SrcTexture.SampleLevel(testSamp, input.uv0, 0).r ;
    //float4 TexColor;
    //int3 TexPosition;

    lLocalPosition.xyz = input.pos;
    lLocalPosition.w = 1.0f;
    lLocalPosition.y += heightValue * constants;
    // 座標計算( ローカル→ビュー→プロジェクション )
    lWorldPosition.x = dot(lLocalPosition, base.LocalWorldMatrix[0]);
    lWorldPosition.y = dot(lLocalPosition, base.LocalWorldMatrix[1]);
    lWorldPosition.z = dot(lLocalPosition, base.LocalWorldMatrix[2]);
    lWorldPosition.w = 1.0f;
    //ここまでがローカル

    //ここからがビュー
    lViewPosition.x = dot(lWorldPosition, base.ViewMatrix[0]);
    lViewPosition.y = dot(lWorldPosition, base.ViewMatrix[1]);
    lViewPosition.z = dot(lWorldPosition, base.ViewMatrix[2]);
    lViewPosition.w = 1.0f;
    //ここまでがビュー


    //ここからがプロジェクション
    ret.pos.x = dot(lViewPosition, base.ProjectionMatrix[0]);
    ret.pos.y = dot(lViewPosition, base.ProjectionMatrix[1]);
    ret.pos.z = dot(lViewPosition, base.ProjectionMatrix[2]);
    ret.pos.w = dot(lViewPosition, base.ProjectionMatrix[3]);
    //ここまでがプロジェクション
    // 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )

    
    // 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )
    
    ret.dif = input.diffuse * (max(saturate(dot(input.norm, light)), 0.3));
    ret.spc = input.specular;
    
    return ret;
}