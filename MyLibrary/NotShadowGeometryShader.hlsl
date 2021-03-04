#include"NotShadowShaderHeader.hlsli"


[maxvertexcount(3)]//最大頂点数
void GSmain
(
	//triangleはプリミティブのこと
	//[3]は、トライアングルリスト、ストリップということ
	triangle VSOutput input[3],
	inout TriangleStream< GSOutput > output
)
{
	//面を増やす方法
	//svposを2つ作って、座標をずらしたものを2個目に入れる?
	//outputを2回行う?
	//頂点を増やせば面が増える
	//頂点を結ぶ辺の間に頂点をセットする
	//面分割はテッセレータでやった方がいいらしい
	GSOutput element;

	//分割数
	//int level = 2;

	//辺の長さを求めずに、重心を求めたほうがいい?

	//(頂点1X + 頂点2X) / 分割数 = 辺の上の頂点の間隔X
	//(頂点1Y + 頂点2Y) / 分割数 = 辺の上の頂点の間隔Y

	//input[0]の頂点と、input[1]の頂点の間にある頂点の間隔(今回はlevel2なので、辺の中心)
	/*float4 kankaku[3];
	kankaku[0] = float4(float3((input[0].svpos.xyz + input[1].svpos.xyz) / (float)level), 1);
	kankaku[1] = float4(float3((input[1].svpos.xyz + input[1].svpos.xyz) / (float)level), 1);
	kankaku[2] = float4(float3((input[2].svpos.xyz + input[1].svpos.xyz) / (float)level), 1);*/



	//法線を求めている
	//normalを使えるようにする
	/*float3 faceEdgeA = input[1].svpos - input[0].svpos;
	float3 faceEdgeB = input[2].svpos - input[0].svpos;
	float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));*/


	for (uint i = 0; i < 3; i++)
	{
		//element.svpos = input[i].svpos + float4(faceNormal * ex, 1);
		element.svpos = input[i].svpos + float4(input[i].normal  * ex, 1);

		element.normal = input[i].normal;
		element.uv = input[i].uv;
		output.Append(element);
	}


	/*element.svpos = input[0].svpos;
	element.normal = input[0].normal;
	element.uv = input[0].uv;
	output.Append(element);
	element.svpos = kankaku[0]; de32
	output.Append(element);
	element.svpos = kankaku[2];
	output.Append(element);

	element.svpos = input[1].svpos;
	output.Append(element);
	element.svpos = kankaku[0];
	output.Append(element);
	element.svpos = kankaku[1];
	output.Append(element);

	element.svpos = kankaku[0];
	output.Append(element);
	element.svpos = kankaku[1];
	output.Append(element);
	element.svpos = kankaku[2];
	output.Append(element);

	element.svpos = input[2].svpos;
	output.Append(element);
	element.svpos = kankaku[1];
	output.Append(element);
	element.svpos = kankaku[2];
	output.Append(element);*/

	output.RestartStrip();

}