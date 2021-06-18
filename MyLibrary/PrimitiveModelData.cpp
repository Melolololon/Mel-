#include "PrimitiveModelData.h"

std::vector<Vertex>PrimitiveModelData::vertices;

void PrimitiveModelData::CreateBox( std::vector<Vertex>& vertices, std::vector<USHORT>&indices)
{
	float x = 0.5f;
	float y = 0.5f;
	float z = 0.5f;

	vertices.resize(24);

	//正面
	vertices[0].pos = { -x,-y,-z };
	vertices[0].uv = { 0.0f,1.0f };
	vertices[1].pos = { -x,y,-z };
	vertices[1].uv = { 0.0f,0.0f };
	vertices[2].pos = { x,-y,-z };
	vertices[2].uv = { 1.0f,1.0f };
	vertices[3].pos = { x,y,-z };
	vertices[3].uv = { 1.0f,0.0f };

	//正面の上
	vertices[4].pos = { -x, y,-z };
	vertices[4].uv = { 0.0f,1.0f };
	vertices[5].pos = { -x, y,z };
	vertices[5].uv = { 0.0f,0.0f };
	vertices[6].pos = { x,y,-z };
	vertices[6].uv = { 1.0f,1.0f };
	vertices[7].pos = { x,y,z };
	vertices[7].uv = { 1.0f,0.0f };

	//正面の裏
	vertices[8].pos = { -x, y,z };
	vertices[8].uv = { 0.0f,1.0f };
	vertices[9].pos = { -x, -y,z };
	vertices[9].uv = { 0.0f,0.0f };
	vertices[10].pos = { x,y,z };
	vertices[10].uv = { 1.0f,1.0f };
	vertices[11].pos = { x,-y,z };
	vertices[11].uv = { 1.0f,0.0f };

	//正面の下
	vertices[12].pos = { -x, -y,z };
	vertices[12].uv = { 0.0f,1.0f };
	vertices[13].pos = { -x, -y,-z };
	vertices[13].uv = { 0.0f,0.0f };
	vertices[14].pos = { x,-y,z };
	vertices[14].uv = { 1.0f,1.0f };
	vertices[15].pos = { x,-y,-z };
	vertices[15].uv = { 1.0f,0.0f };

	//正面の右
	vertices[16].pos = { x,y,-z };
	vertices[16].uv = { 0.0f,0.0f };
	vertices[17].pos = { x,y,z };
	vertices[17].uv = { 1.0f,0.0f };
	vertices[18].pos = { x,-y,-z };
	vertices[18].uv = { 0.0f,1.0f };
	vertices[19].pos = { x,-y,z };
	vertices[19].uv = { 1.0f,1.0f };

	//正面の左
	vertices[20].pos = { -x,-y,-z };
	vertices[20].uv = { 1.0f,1.0f };
	vertices[21].pos = { -x,-y,z };
	vertices[21].uv = { 0.0f,1.0f };
	vertices[22].pos = { -x,y,-z };
	vertices[22].uv = { 1.0f,0.0f };
	vertices[23].pos = { -x,y,z };
	vertices[23].uv = { 0.0f,0.0f };

	indices =
	{
		0,1,2,2,1,3,
		4,5,6,6,5,7,
		8,9,10,10,9,11,
		12,13,14,14,13,15,
		16,17,18,18,17,19,
		20,21,22,22,21,23,
	};


}

PrimitiveModelData::PrimitiveModelData()
{
}

PrimitiveModelData::~PrimitiveModelData()
{
}

void PrimitiveModelData::Initialize()
{
#pragma region ボックス生成
	//CreateBox();


#pragma endregion


}
