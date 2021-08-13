#include"FbxShaderHeader.hlsli"

//テッセレーションコラム
//https://game.watch.impress.co.jp/docs/series/3dcg/590707.html

//出力された点(コントロールポイント)をいじるシェーダー
//ここで折り曲げる処理を実装することによって、ローポリモデルを細かい滑らかなモデルにすることができる


//テッセレータステージから出力された頂点ごとに呼び出される
[domain(DOMEIN)]//ドメイン属性
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float2 domain : SV_DomainLocation,//UV座標のこと? tryだとfloat3(UVW) この座標は、ポリゴン上のどの辺にあるかを示す
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT Output;

	//pは、テッセレータに渡した頂点(コントロールポイント)と、ドメイン(ポリゴン上UV(W)座標)から求めた頂点座標

	//三角形の場合は、uvw全部が0になることはない?
	//四角形だとある?あるから patch[0].pos * domain.x + patch[0].pos * domain.yの式で原点に頂点配置される?
	//float4 p = patch[0].pos * domain.x + patch[1].pos * domain.y /*+ patch[2].pos * domain.z*/;

	//正面から(視線が板ポリの法線の逆ベクトルになるように)見て、左下から左上、左下から右下のベクトルを求める
	float3 posVY = patch[1].pos.xyz - patch[0].pos.xyz;
	float3 posVX = patch[2].pos.xyz - patch[0].pos.xyz;

	//ドメインを掛ける(0で左下、1でベクトルの終わりに)
	float4 p = float4(patch[0].pos.xyz + posVY * domain.y + posVX * domain.x, 1);
	
		
	Output.worldPos = mul(worldMat, p);
	Output.svpos = mul(mat, p);
	//Output.uv = float2(patch[0].uv * domain.x + patch[1].uv * domain.y /*+ patch[2].uv * domain.z*/);
	//Output.normal = float3(patch[0].normal * domain.x + patch[1].normal * domain.y /*+ patch[2].normal * domain.z*/);

	float2 uvVY = patch[1].uv.xy - patch[0].uv.xy;
	float2 uvVX = patch[2].uv.xy - patch[0].uv.xy;
	Output.uv = float2(patch[0].uv.xy + uvVY * domain.y + uvVX * domain.x);
	//折り曲げないから法線はそのまま
	Output.normal = patch[0].normal;

	return Output;
}
