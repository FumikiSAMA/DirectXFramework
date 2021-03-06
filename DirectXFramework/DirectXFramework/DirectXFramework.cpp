//DirectXFramework.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "DirectXFramework.h"

#define MAX_LOADSTRING 100

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
/* LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 窗口过程函数（并非Windows API中定义而是因程序而异的用户自定义）
hwnd：指向窗口的句柄
message：指定消息类型
wParam：指定其余的 消息特定的消息 该参数内容与message参数值有关
lParam：指定其余的 消息特定的消息 该参数内容与message参数值有关
LRESULT：指定函数返回类型 表示调用结果是否成功
CALLBACK：表明函数由操作系统调用
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
CGameManager *gm = CGameManager::getInstance();

/* int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd);
hInstance：表示应用程序当前运行的实例的句柄
hPrevInstance：表示当前实例的前一个实例的句柄
lpCmdLine：指定传递给应用程序的命令行参数 游戏中通常不会用到这个参数
nCmdShow：一个在程序启动时由操作系统传入（通常为1）的整数 指定程序的窗口应该如何显示
nCmdShow = 0：不显示
nCmdShow = 1：（默认值）正常显示
nCmdShow = 2：最小化显示
nCmdShow = 3：最大化显示 */
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	gm->getGameProc(WndProc);
	if (gm->RunGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow)) return 0;
}