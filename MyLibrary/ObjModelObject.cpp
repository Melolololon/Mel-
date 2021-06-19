#include "ObjModelObject.h"


ObjModelObject::ObjModelObject(ModelData* pModelData, ConstBufferData* userConstBufferData)
	:ModelObject(pModelData, userConstBufferData)
{
}

ObjModelObject::~ObjModelObject()
{
}

void ObjModelObject::Initialize()
{
}

void ObjModelObject::Draw(const std::string& rtName)
{
}
