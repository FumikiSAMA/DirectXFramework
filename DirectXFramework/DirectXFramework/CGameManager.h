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

	// 创建的设备
	IDirect3DDevice9 *device = nullptr;

	// Direct3D框架的单例
	D3DFramework *d3dFramework = D3DFramework::getInstance();

	// 创建模型
	void Construct();
	// 摧毁模型
	void Destruct();
public:
	~CGameManager();
	static CGameManager *getInstance();

	// 获取消息循环体
	void getGameProc(LRESULT(CALLBACK *GameProc)(HWND, UINT, WPARAM, LPARAM));
	// 运行游戏
	bool RunGame(HINSTANCE &, HINSTANCE &, LPWSTR &, int &);
};

// Direct3D9渲染
bool D3D9Display(float, IDirect3DDevice9 *);


