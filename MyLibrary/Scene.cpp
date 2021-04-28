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

void Scene::Initialize() {}
void Scene::Update() {}
void Scene::Draw() {}
void Scene::end() {}

bool Scene::GetIsEnd()
{
	return isEnd;
}

Scene* Scene::GetNextScene()
{
	return this;
}


void* Scene::GetPtr() 
{
	return this;
}