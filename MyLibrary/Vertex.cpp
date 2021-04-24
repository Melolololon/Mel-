#include"DirectXStruct.h"

Vertex::Vertex(){}
Vertex::~Vertex(){}

Vertex::Vertex(const ObjAnimationVertex& objVertex)
	:pos(objVertex.pos),
	 uv(objVertex.uv),
	 normal(objVertex.normal)
{
}
