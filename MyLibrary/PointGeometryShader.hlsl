#include"PointShaderHeader.hlsli"
static const uint vertNum = 4;

static const float4 ofset_array[vertNum] = 
{
	float4(-0.5f , - 0.5f,0,0),
	float4(-0.5f , + 0.5f,0,0),
	float4(+0.5f , - 0.5f,0,0),
	float4(+0.5f , + 0.5f,0,0)
	
	/*float4(-0.7, -0.7,0,1),
	float4(-0.7 ,+0.7,0,1),
	float4(+0.7 ,-0.7,0,1),
	float4(+0.7, +0.7,0,1)*/
};

static const float2 uvs[vertNum] =
{
	float2(0.0,0.0),
	float2(0.0,1.0),
	float2(1.0,0.0),
	float2(1.0,1.0)

};

[maxvertexcount(vertNum)]
void GSmain(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	for (uint i = 0; i < vertNum; i++)
	{

		//svposに入れてからスケール掛ける
	//資料だとビルボード前にスケール乗算してるが、そうすると座標がおかしくなる
	//どっちみちおかしくなる
		//おかしくなるのは、ワールド座標{0,0,0}に点が来てないから?
		//それで行列掛けてバグってる?
		//普段平行移動した後に掛けてるから関係ない?
		//カメラの位置に応じてめちゃくちゃ移動してる
		//farを20にして試したらプレイヤーと同じ座標なのにプレイヤーよりも早く消えたから確定
	//ピクセルシェーダーでposを返したら白くなったので確定
		//ちゃんと値をリターンできてない?
		//投資投影やめて配列の値変えたりして直で送ってもサイズ変わらん
		//リターンできてた(objでも同じ結果になったため)
		//matの乗算を頂点シェーダーでやったら通常通りに描画できた
		//解決
		//してない!!
		//通常の板ポリのスケールを変えた時にビルボードするとおかしくなる
		//こちらも同じくスケール調整するとおかしくなるかも


		float4 iPos = input[0].svpos;
		float4 offset = ofset_array[i];
		offset.x *= input[0].scale.x;
		offset.y *= input[0].scale.y;
		offset = mul(billboardMat, offset);
		element.svpos = iPos + offset;
		element.svpos = mul(mat, element.svpos);
		element.uv = uvs[i];
		element.color = input[0].color;


	//	float4 iPos = input[0].svpos;
	//	float4 offset = ofset_array[i];
	///*	offset.x *= input[0].scale.x;
	//	offset.y *= input[0].scale.y;
	//	offset = mul(billboardMat, offset);*/
	//	//element.svpos = iPos + offset;
	//	//element.svpos = mul(mat, element.svpos);
	//	element.uv = uvs[i];
	//	element.color = input[0].color;

	//	element.svpos = iPos+ offset ;


		output.Append(element);
	}
}