#include "Play.h"

VertexData d;
VertexData model;
heap h;

Play::Play()
{
	Library::createBoard({ 1,1 }, dimention3D, d);
	Library::createHeapData2({ 255,255,255,255 }, 1, &h);
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
	Library::drawGraphic(d.handle, h, 0);
}

void Play::end()
{
}

std::string Play::getNextScene()
{
	return "";
}
