#pragma once

#include <array>
#include <vector>
#include <wrl.h>

#include <XInput.h>
#include "WinApp.h"

#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
using namespace Microsoft::WRL;
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class Input
{
public:
	static Input* GetInstance();
	void Initialize(WinApp* winApp);
	void Update();
	bool PushKey(BYTE keyNumber);
	bool TriggerKey(BYTE keyNumber);

public:
	//ウィンドウ
	WinApp* winApp_;
	ComPtr<IDirectInput8> directInput;
	//キーボード
	ComPtr<IDirectInputDevice8> keyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
};