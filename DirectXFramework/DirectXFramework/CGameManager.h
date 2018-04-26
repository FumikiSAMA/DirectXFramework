#pragma once
#include "D3DFramework.h"

class CGameManager
{
private:
	CGameManager() {}
	CGameManager(const CGameManager &) {}
	CGameManager &operator= (const CGameManager &) {}
	static CGameManager *m_instance;

	typedef LRESULT(CALLBACK *p_GameProc)(HWND, UINT, WPARAM, LPARAM);
	p_GameProc GameProc;

	// �������豸
	IDirect3DDevice9 *device = nullptr;

	// Direct3D��ܵĵ���
	D3DFramework *d3dFramework = D3DFramework::getInstance();

	// ����ģ��
	void Construct();
	// �ݻ�ģ��
	void Destruct();
public:
	~CGameManager();
	static CGameManager *getInstance();

	// ��ȡ��Ϣѭ����
	void getGameProc(LRESULT(CALLBACK *GameProc)(HWND, UINT, WPARAM, LPARAM));
	// ������Ϸ
	bool RunGame(HINSTANCE &, HINSTANCE &, LPWSTR &, int &);
};

// Direct3D9��Ⱦ
bool D3D9Display(float, IDirect3DDevice9 *);


