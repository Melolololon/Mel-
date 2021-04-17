#include "Scene.h"


Scene::Scene()
{
	isEnd = false;
}


Scene::~Scene()
{
}

void Scene::falseIsEnd() 
{
	isEnd = false;
}

void Scene::initialize() {}
void Scene::update() {}
void Scene::draw() {}
void Scene::end() {}

bool Scene::getIsEnd()
{
	return isEnd;
}

Scene* Scene::getNextScene()
{
	return this;
}


void* Scene::getPtr() 
{
	return this;
}