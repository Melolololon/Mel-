#pragma once
#include<string>

struct Node
{
};

class FbxModel
{
private:
	std::string modelName;

public:
	FbxModel(const std::string& modelName);
	~FbxModel();

};

