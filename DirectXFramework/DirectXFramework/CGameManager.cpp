#include "stdafx.h"

CGameManager::~CGameManager()
{
}

CGameManager *CGameManager::m_instance = new CGameManager();
CGameManager *CGameManager::getInstance()
{
	return m_instance;
}

// ��ȡ��Ϣѭ����
void CGameManager::getGameProc(LRESULT (CALLBACK *GameProc)(HWND, UINT, WPARAM, LPARAM))
{
	CGameManager::GameProc = GameProc;
}

// ����ģ��
void CGameManager::Construct()
{
	
}

// �ݻ�ģ��
void CGameManager::Destruct()
{

}

// ������Ϸ
bool CGameManager::RunGame(HINSTANCE &hInstance, HINSTANCE &hPrevInstance, LPWSTR &lpCmdLine, int &nCmdShow)
{
	// D3D�豸��������ʼ��
	if (!CGameManager::d3dFramework->D3D9Initial(hInstance, (WNDPROC)GameProc, 1366, 768, false, D3DDEVTYPE_HAL, &device))
	{
		MessageBox(0, L"D3dInitial Failed", 0, 0);
		return true;
	}
	// ģ�ʹ���
	Construct();
	// ��Ϣѭ��
	d3dFramework->D3D9MessageLoop(D3D9Display, device);
	// ģ������
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