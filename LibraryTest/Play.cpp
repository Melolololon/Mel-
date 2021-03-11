#include "Play.h"

VertexDataKey d;
VertexDataKey modelV;
HeapDataKey h;
HeapDataKey modelH;

Play::Play()
{
	
	Library::createBoard({ 1,1 }, dimention3D,"board", d);
	Library::createHeapData2({ 255,255,255,255 }, 1, "board", h);

	std::string material;
	modelV.key = "biru";
	modelH.key = "biru";
	Library::loadOBJVertex("Resources/Obj/test.obj", true, true, &material,"biru", modelV);
	Library::loadOBJMaterial("Resources/Obj/", material, 1,"biru", modelH);
}


Play::~Play()
{
}

void Play::initialize()
{
}

void Play::update()
{
	

}

void Play::draw()
{
	Library::setPipeline(PIPELINE_NORMAL);
	Library::drawGraphic(modelV, modelH, 0);
}

void Play::end()
{
}

std::string Play::getNextScene()
{
	return "";
}
