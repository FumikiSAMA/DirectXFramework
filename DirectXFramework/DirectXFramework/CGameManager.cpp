#include "stdafx.h"

CGameManager::~CGameManager()
{
}

CGameManager *CGameManager::m_instance = new CGameManager();
CGameManager *CGameManager::getInstance()
{
	return m_instance;
}

// 获取消息循环体
void CGameManager::getGameProc(LRESULT (CALLBACK *GameProc)(HWND, UINT, WPARAM, LPARAM))
{
	CGameManager::GameProc = GameProc;
}

// 创建模型
void CGameManager::Construct()
{
	
}

// 摧毁模型
void CGameManager::Destruct()
{

}

// 运行游戏
bool CGameManager::RunGame(HINSTANCE &hInstance, HINSTANCE &hPrevInstance, LPWSTR &lpCmdLine, int &nCmdShow)
{
	// D3D设备创建及初始化
	if (!CGameManager::d3dFramework->D3D9Initial(hInstance, (WNDPROC)GameProc, 1366, 768, false, D3DDEVTYPE_HAL, &device))
	{
		MessageBox(0, L"D3dInitial Failed", 0, 0);
		return true;
	}
	// 模型创建
	Construct();
	// 消息循环
	d3dFramework->D3D9MessageLoop(D3D9Display, device);
	// 模型销毁
	Destruct();
	device->Release();
	return true;
}

bool D3D9Display(float time, IDirect3DDevice9 * device)
{
	if (device)
	{
		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
		if (SUCCEEDED(device->BeginScene()))
		{   // TODO:
			
		}
		device->EndScene();
		device->Present(0, 0, 0, 0);
	}
	return true;
}