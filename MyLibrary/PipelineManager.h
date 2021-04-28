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
	static PipelineManager* GetInstance();

	void AddPipeline(std::string keyName,pipeline pl);
	void DeletePipeline(std::string keyName);
	pipeline GetPipeline(std::string keyName);
	void ClearPipeline();
};

