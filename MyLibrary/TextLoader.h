#pragma once
#include<vector>
#include<string>

class TextLoader
{
private:
	static std::vector<std::string> stringTextData;


public:


#pragma region �ǂݍ���

	static bool loadText(const char* path);

#pragma endregion



#pragma region �󂯎��
	/// <summary>
	/// int�ɕϊ����ē񎟌�vector�Ɋi�[���܂�(�J���}�͊O����܂�)
	/// </summary>
	/// <returns></returns>
	static void getIntVector(std::vector<std::vector<int>>& intTextData);



#pragma endregion

};

