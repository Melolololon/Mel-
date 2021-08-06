#include"FbxShaderHeader.hlsli"

//テッセレーションコラム
//https://game.watch.impress.co.jp/docs/series/3dcg/590707.html

//分割されたポリゴンをいじるシェーダー(ジオメトリシェーダーみたいなもん)
//ここで折り曲げる処理を実装することによって、ローポリモデルを細かい滑らかなモデルにすることができる


//テッセレータステージから出力された頂点ごとに呼び出される
[domain(DOMEIN)]//ドメイン属性
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,//tryだとfloat3(UVW座標?)
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT Output;

	float4 p = patch[0].pos * domain.x + patch[1].pos * domain.y + patch[2].pos * domain.z;
	float4 mulPrePos = float4(p.xyz, 1);
		
	Output.worldPos = mul(worldMat, mulPrePos);
	Output.svpos = mul(mat, mulPrePos);
	Output.uv = float2(
		patch[0].uv * domain.x + patch[1].uv * domain.y + patch[2].uv * domain.z);
	Output.normal = float3(
		patch[0].normal * domain.x + patch[1].normal * domain.y + patch[2].normal * domain.z);

	return Output;
}
