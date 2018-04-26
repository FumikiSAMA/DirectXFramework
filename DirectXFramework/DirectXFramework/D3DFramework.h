#pragma once
class D3DFramework
{
private:
	// 私有化构造函数
	D3DFramework() {}
	// 私有化拷贝构造函数
	D3DFramework(const D3DFramework &) {}
	// 私有化'='符号的运算符重载
	D3DFramework &operator= (const D3DFramework &) {}
	// 私有化当前类的静态实例
	static D3DFramework *m_instance;

	// 注册 创建并显示窗口
	HWND D3D9CreateWindow(HINSTANCE, WNDPROC, int, int, bool);

	// 校验显卡是否支持硬件顶点运算
	int D3D9CheckHardwareVP(IDirect3D9 *, D3DDEVTYPE);

	// 填充D3DPRESENT_PARAMETERS结构体
	D3DPRESENT_PARAMETERS D3D9CreatePP(HWND, int, int, bool);

	// 创建设备
	bool D3D9CreateDevice(IDirect3D9 *, D3DDEVTYPE, HWND, int, D3DPRESENT_PARAMETERS, IDirect3DDevice9 **);
public:
	~D3DFramework();
	// 提供外部接口获取当前类的唯一实例（单例模式）
	static D3DFramework *getInstance();

	// 搭建Direct3D9框架
	bool D3D9Initial(HINSTANCE, WNDPROC, int, int, bool, D3DDEVTYPE, IDirect3DDevice9 **);

	// Direct3D9的消息循环
	int D3D9MessageLoop(bool(*p_Display)(float, IDirect3DDevice9 *), IDirect3DDevice9 *);
};


