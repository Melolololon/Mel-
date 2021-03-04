#include"TextLoader.h"
#include<fstream>


std::vector<std::string> TextLoader::stringTextData;

#pragma region “Ç‚İ‚İ

bool TextLoader::loadText(const char* path)
{
	std::ifstream text(path);
	if (!text)return false;

	std::string sText;
	while (std::getline(text, sText))
	{
		stringTextData.push_back(sText);
	}
	text.close();
	return true;
}



#pragma endregion


#pragma region ó‚¯æ‚è

void TextLoader::getIntVector(std::vector<std::vector<int>>& intTextData)
{
	
	if (stringTextData.size() == 0)return;

	intTextData.resize(stringTextData.size());

	//string‚Ìvector‚É“ü‚ê‚é‚Æ‚«‚É,‚ğÈ‚­•û–@‚à‚ ‚é
	int kanmaCount = 0;

	for (unsigned int y = 0; y < stringTextData.size(); y++) 
	{
		intTextData[y].resize(stringTextData[y].size() / 2 + 1);
		kanmaCount = 0;
		for (unsigned int x = 0; x < stringTextData[y].size(); x++)
		{
			char c = stringTextData[y][x];
		
			if (c == ',')
			{
				kanmaCount++;
				continue;
			}

			intTextData[y][x - kanmaCount] = atoi(&c);
		}
	}
}
#pragma endregion

