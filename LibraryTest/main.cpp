#include<Windows.h>
#include"Game.h"

//#pragma commentで読み込めば、スタティックライブラリでも.lib読み込める?
//libfbxsdk-md.pdb' が見つかりませんでした。LibraryTest.exe は、デバッグにオブジェクト ファイルを必要とする /DEBUG:FASTLINK を使ってビルドされました。
//は、「ビルド」の、「ソリューションの完全なプログラムデータベースファイルを構築する」を実行すると直る
#include<fbxsdk.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//FbxManager* fbxManager = FbxManager::Create();
	//fbxManager = FbxManager::Create();

	//FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	//fbxManager->SetIOSettings(ios);

	//FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	//fbxImporter->Destroy();

	Game::getInstance()->run();
	return 0;
}