#pragma once
//#include "dxgidebug.h"
#include <d3d12.h>

class Message
{
public:
	static Message* GetInstance();
	//Xボタンで終了メッセ時が来たらゲームループを抜ける 
	bool Update();
	//呼び出した時のエラーを出す
	bool CheckError();
public:
	MSG msg{};
};

