#include "Play.h"

VertexDataKey d;
VertexDataKey model;
HeapDataKey h;

Play::Play()
{
	d.key = "Board";
	Library::createBoard({ 1,1 }, dimention3D, d);
	Library::createHeapData2({ 255,255,255,255 }, 1, h);
	Library::loadOBJVertex("Resources/Obj/test.obj", true, true, nullptr, model);
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
	Library::drawGraphic(d, h.h, 0);
}

void Play::end()
{
}

std::string Play::getNextScene()
{
	return "";
}
