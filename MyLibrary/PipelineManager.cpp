#include "PipelineManager.h"

PipelineManager::PipelineManager(){}
PipelineManager::~PipelineManager(){}

PipelineManager* PipelineManager::getInstance()
{
	static PipelineManager p;
	return &p;
}

void PipelineManager::addPipeline(std::string keyName, pipeline pl)
{
	pipelines.emplace(keyName, pl);
}

void PipelineManager::deletePipeline(std::string keyName)
{

}

pipeline PipelineManager::getPipeline(std::string keyName)
{
	return pipelines[keyName];
}

void PipelineManager::clearPipeline()
{

}
