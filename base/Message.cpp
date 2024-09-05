#include "Message.h"

Message* Message::GetInstance()
{
	static Message instance;
	return &instance;
}

bool Message::Update()
{
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // メッセージがある？
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg);  // ウィンドウプロシージャにメッセージを送る
	}

	if (msg.message == WM_QUIT) // 終了メッセージが来たらループを抜ける
	{
		return true;
	}
	return false;

	////Xボタンで終了メッセ時が来たらゲームループを抜ける 
	//if (msg.message == WM_QUIT)
	//{
	//	IDXGIDebug* giDebugInterface = nullptr;

	//	if (giDebugInterface == nullptr)
	//	{
	//		//作成
	//		typedef HRESULT(__stdcall* fPtr)(const IID&, void**);
	//		HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll");
	//		if (hDll != 0)
	//		{
	//			fPtr DXGIGetDebugInterface =
	//				(fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");

	//			DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&giDebugInterface);

	//			//出力
	//			giDebugInterface->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_DETAIL);
	//		}
	//	}
	//	return 1;
	//}
}

bool Message::CheckError()
{
	//IDXGIDebug* giDebugInterface = nullptr;

	//if (giDebugInterface == nullptr)
	//{
	//	//作成
	//	typedef HRESULT(__stdcall* fPtr)(const IID&, void**);
	//	HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll");
	//	if (hDll != 0)
	//	{
	//		fPtr DXGIGetDebugInterface =
	//			(fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");

	//		DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&giDebugInterface);

	//		//出力
	//		giDebugInterface->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_DETAIL);
	//	}
	//}
	return 1;

}
