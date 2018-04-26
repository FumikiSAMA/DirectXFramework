#pragma once
class D3DFramework
{
private:
	// ˽�л����캯��
	D3DFramework() {}
	// ˽�л��������캯��
	D3DFramework(const D3DFramework &) {}
	// ˽�л�'='���ŵ����������
	D3DFramework &operator= (const D3DFramework &) {}
	// ˽�л���ǰ��ľ�̬ʵ��
	static D3DFramework *m_instance;

	// ע�� ��������ʾ����
	HWND D3D9CreateWindow(HINSTANCE, WNDPROC, int, int, bool);

	// У���Կ��Ƿ�֧��Ӳ����������
	int D3D9CheckHardwareVP(IDirect3D9 *, D3DDEVTYPE);

	// ���D3DPRESENT_PARAMETERS�ṹ��
	D3DPRESENT_PARAMETERS D3D9CreatePP(HWND, int, int, bool);

	// �����豸
	bool D3D9CreateDevice(IDirect3D9 *, D3DDEVTYPE, HWND, int, D3DPRESENT_PARAMETERS, IDirect3DDevice9 **);
public:
	~D3DFramework();
	// �ṩ�ⲿ�ӿڻ�ȡ��ǰ���Ψһʵ��������ģʽ��
	static D3DFramework *getInstance();

	// �Direct3D9���
	bool D3D9Initial(HINSTANCE, WNDPROC, int, int, bool, D3DDEVTYPE, IDirect3DDevice9 **);

	// Direct3D9����Ϣѭ��
	int D3D9MessageLoop(bool(*p_Display)(float, IDirect3DDevice9 *), IDirect3DDevice9 *);
};


