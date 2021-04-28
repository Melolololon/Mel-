#include "PipelineManager.h"

PipelineManager::PipelineManager(){}
PipelineManager::~PipelineManager(){}

PipelineManager* PipelineManager::GetInstance()
{
	static PipelineManager p;
	return &p;
}

void PipelineManager::AddPipeline(std::string keyName, pipeline pl)
{
	pipelines.emplace(keyName, pl);
}

void PipelineManager::DeletePipeline(std::string keyName)
{

}

pipeline PipelineManager::GetPipeline(std::string keyName)
{
	return pipelines[keyName];
}

void PipelineManager::ClearPipeline()
{

}
