#pragma once
#include<vector>
#include<string>

class TextLoader
{
private:
	static std::vector<std::string> stringTextData;


public:


#pragma region “Ç‚İ‚İ

	static bool loadText(const char* path);

#pragma endregion



#pragma region ó‚¯æ‚è
	/// <summary>
	/// int‚É•ÏŠ·‚µ‚Ä“ñŸŒ³vector‚ÉŠi”[‚µ‚Ü‚·(ƒJƒ“ƒ}‚ÍŠO‚³‚ê‚Ü‚·)
	/// </summary>
	/// <returns></returns>
	static void getIntVector(std::vector<std::vector<int>>& intTextData);



#pragma endregion

};

