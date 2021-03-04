#pragma once
#include"Library.h"
#include<unordered_map>
class PipelineManager
{

private:
	PipelineManager();
	~PipelineManager(); 


	std::unordered_map<std::string, pipeline> pipelines;
public:
	
	PipelineManager(PipelineManager& p) = delete;
	PipelineManager&operator=(PipelineManager& p) = delete;
	static PipelineManager* getInstance();

	void addPipeline(std::string keyName,pipeline pl);
	void deletePipeline(std::string keyName);
	pipeline getPipeline(std::string keyName);
	void clearPipeline();
};

