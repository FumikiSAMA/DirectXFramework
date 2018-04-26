#include "stdafx.h"

D3DFramework::~D3DFramework()
{
}

// ����ģʽ
D3DFramework *D3DFramework::m_instance = new D3DFramework();
D3DFramework *D3DFramework::getInstance()
{
	return m_instance;
}

// �Direct3D9���
bool D3DFramework::D3D9Initial(HINSTANCE hInstance, WNDPROC wndProc, int width, int height, bool fullScreen, D3DDEVTYPE deviceType, IDirect3DDevice9 **device)
{
	// ��������
	HWND hwnd = D3D9CreateWindow(hInstance, wndProc, width, height, fullScreen);
	if (!hwnd)
	{
		MessageBox(0, L"D3DCreateWindow Failed", 0, 0);
		return false;
	}

	/* ��һ�� ��ȡָ��ӿ�IDirect3D9ָ��
	ʹ��Direct3DCreate9()����������D3D_SDK_VERSION������ȡ��ָ��
	����D3D_SDK_VERSION��������ȷ��Ӧ�ó���ʹ�õ�ͷ�ļ��ڱ���ʱ�ܹ���DirectX Runtime�Ķ�̬���ӿ���ƥ�� ��֤��������ʱ�ܹ���ȷ���������
	Idrect3D9������Ҫ��������;��
	1.�豸ö�� ָ��ȡϵͳ���õ�ÿ��ͼ�ο������� ��ʾģʽ ��ʽ�Լ�������Ϣ
	2.����IDirectDDevice9���͵Ķ��� */
	IDirect3D9 *d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9)
	{
		MessageBox(0, L"Direct3DCreate9 Failed", 0, 0);
		return false;
	}

	/* �ڶ��� У���Կ��Ƿ�֧��Ӳ����������
	����һ���������Կ���IDirect3DDevice9����ʱ ����ָ��ʹ�øö�����ж������������ ������� ��ѡӲ����������
	����Ӧ�ó���ʱ ����ҪһЩ�µ� �ر�� ���߸߼������ԣ�Ӳ����û�й㷺֧�ֵ����ԣ� ���ȼ��һ���豸��D3DCAPS���Ƿ�֧�ָ������Ǻ��б�Ҫ�� */
	int vp = D3D9CheckHardwareVP(d3d9, deviceType);

	/* ������ ���D3DPRESENT_PARAMETERS�ṹ
	�ýṹ����ָ������Ҫ������IDirect3DDevice9���Ͷ����һЩ���� ��΢��Ϥһ�¸ýṹ �п� */
	D3DPRESENT_PARAMETERS d3dpp = D3D9CreatePP(hwnd, width, height, fullScreen);

	/* ���Ĳ� �����豸
	��������ɹ����ط���D3D_OK ���򷵻ظ���HRESULT���͵Ĵ����� */
	return D3D9CreateDevice(d3d9, deviceType, hwnd, vp, d3dpp, device);
}

// ע�� ��������ʾ����
HWND D3DFramework::D3D9CreateWindow(HINSTANCE hInstance, WNDPROC wndProc, int width, int height, bool fullScreen)
{
	// ��ƴ�����
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	/* style��ָ����һ���ʹ��ڵ���ʽ �ó�Ա������������ʽ��������ϣ���λ����ϣ���
	1.CS_VREDRAW����ֱ�ػ� �����ڴ�ֱ�����ϵĸ߶ȷ����仯ʱ �����»�����������
	2.CS_HREDRAW��ˮƽ�ػ� ������ˮƽ�����ϵĿ�ȷ����仯ʱ �����»�����������
	3.CS_OWNDC����ռ�豸������ Ϊ�����е�ÿ�����ڷ���һ����ֵ���豸������
	4.CS_SAVEBITS����һ�������б����û�ͼ�� �Ա����ڸô��ڱ���ס���ƶ�ʱ����ÿ��ˢ����Ļ ���ǻ�ռ�ø����ڴ�
	5.CS_DBLCLKS�����ڿ��Լ�����˫���¼� ���û��ڴ�����˫�����ʱ �򴰿ڹ��̷������˫����Ϣ
	6.CS_BYTEALLGNCLIENT������û������ֽڶ�����ʾ
	7.CS_BYTEALLGNWINDOW������û����ڰ��ֽڶ�����ʾ
	8.CS_PARENTDC���ڸ��������趨һ���Ӵ��ڵļ����� �Ա����Ӵ����ܹ����ڷ�������
	9.CS_NOCLOSE��ϵͳ�˵���û��CLOSE�˵��� ����û�йرհ�ť */
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	/* lpfnWndProc��ָ�򴰿ڹ��̺����ĺ���ָ�� ���ڹ��̺�����һ���ص����� ���Windows����Ϣ������ƶ������õĹ������£�
	1.����ƴ������ʱ�� �����ڹ��̺����ĵ�ַ��ֵ��lpfnWndProc��Ա����
	2.����RegisterClass(&wndclass)ע�ᴰ���� ϵͳ�������û���д�Ĵ��ڹ��̺����ĵ�ַ
	3.��Ӧ�ó�����յ�ĳһ���ڵ���Ϣʱ ����DispatchMessage(&msg)����Ϣ�ش���ϵͳ ϵͳ��������ǰע�ᴰ����ʱ�õ��ĺ���ָ�� ���ô��ڹ��̺�������Ϣ���д��� */
	wcex.lpfnWndProc = (WNDPROC)wndProc;
	// cbClsExtra��WindowsϵͳΪ������ṹ�����׷�ӵĶ����ֽ��� һ�㽫��ֵ��ʼ��Ϊ0
	wcex.cbClsExtra = 0;
	/* cbWndExtra��WindowsϵͳΪ����ʵ�������׷�ӵĶ�����ֽ��� һ�㽫��ֵ��ʼ��Ϊ0
	�����Ӧ�ó���ʹ����Դ�ļ����CLASSָ����Ի��� ����WNDCLASS�ṹע��öԻ���ʱ cbWndExtra�����ʼ��ΪDLGWINDOWEXTRA */
	wcex.cbWndExtra = 0;
	// hInstance���������ڹ��̵ĳ����ʵ����� һ��ֱ�ӳ�ʼ��ΪwWinMain()������hInstance����ֵ����
	wcex.hInstance = hInstance;
	// hIcon���������ͼ���� �����Ա����������һ��ͼ����Դ�ľ�� ����ʹ��LoadIcon()��������ͼ�� ���hIcon���ΪNULL ��ôϵͳ���ṩһ��Ĭ�ϵ�ͼ��
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	/* hCursor��������Ĺ���� �����Ա����������һ�������Ը�ľ�� ����ʹ��LoadCursor()�������ع�겢����ϵͳ������ù��ľ��
	���hCursorΪNULL Ӧ�ó��������������Ӧ�ó��򴰿�ʱ ��ȷ���ù�����״������� */
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	// hbrBackground��������ı�����ˢ��� �����ڷ����ػ�ʱ ϵͳʹ������ָ���Ļ�ˢ����䴰�ڵı��� �ó�Ա��ָ��Ϊ���ڻ��Ʊ���������ˢ�ľ�� Ҳ����ָ��Ϊһ����׼��ϵͳ��ɫֵ
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	/* lpszMenuName��ָ��һ���Կ���ֹ���ַ��� ���ַ��������˵�����Դ�� ������һ����������ʶ�˵� ��Ҫ��MAKEINTRESOURCE�����滻
	�����lpszMenuName����ΪNULL ��ô������������ഴ���Ĵ��ڽ�û��Ĭ�ϲ˵� */
	wcex.lpszMenuName = NULL;  // MAKEINTRESOURCEW(IDC_MAGICTOWER);
							   // lpszClassName��ָ��һ���Կ���ֹ���ַ��� ���ַ������������������ ���������������RegisterClass����RegisterClassExע������� �����κ��붨��Ŀؼ�����
	wcex.lpszClassName = L"DirectX Framework";
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	// ע�ᴰ��
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(0, L"RegisterClass Failed", 0, 0);
		return 0;
	}

	// ��������
	HWND hWnd =
		/* #define CreateWindowW(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam) \
		CreateWindowExW(0L, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
		lpClassName��ָ������������� �����Windows����ΪWNDCLASS��lpszCalssName��Աָ��������
		����ڵ���CreateWindow����֮ǰû����RegisterClass����ע������� ����ϵͳ���޷���֪��һ���ʹ��ڵ������Ϣ�Ӷ����´�������ʧ��
		lpWindowName��ָ��һ���Կս������ַ���ָ�� ����ָ���������� ���������ʽָ���˱����� ��ô����ָ��Ĵ������ƽ���ʾ�ڴ��ڱ�������
		dwStyle��ָ���������ڵ���ʽ �ò��������ǲ�ͬ������ʽ����� �����Ǽ��ֳ��õĴ�����ʽ
		1.WS_CAPTION������һ���б������Ĵ���
		2.WS_SYSMENU������һ���ڱ������ϴ���ϵͳ�˵��Ĵ��� �����WS_CAPTION����һ��ʹ��
		3.WS_THICKFRAME������һ�����пɵ��߿�Ĵ���
		4.WS_MINIMIZEBOX������һ��������С����ť�Ĵ��� ����ͬʱ�趨WS_SYSMENU����
		5.WS_MAXIMIZEBOS������һ��������󻯰�ť�Ĵ��� ����ͬʱ�趨WS_SYSMENU����
		6.WS_TILED������һ������Ĵ��� һ������Ĵ�����һ����������һ���߿�
		7.WS_TILEDWINDOW������һ������WS_OVERLAPPED WS_CAPTION WS_SYSMENU WS_THICKFRAME��ʽ�Ĵ���
		8.WS_CHILD����������Ϊ�Ӵ��� ����Ӧ���ڵ���ʽ������ʽ
		9.WS_OVERLAPPED������һ������Ĵ��� һ������Ĵ�����һ����������һ���߿� ��WS_TILED��ʽ��ͬ
		10.WS_OVERLAPPEDWINDOW������һ������WS_OVERLAPPED WS_CAPTION WS_SYSMENU WS_THICKFRAME WS_MINIMIZEBOS MS_MAXMIZEBOS��ʽ�Ĳ������ ��WS_TILEDWINDOW��ʽ��ͬ
		11.WS_EX_TOPMOST��ָ���������ʽ�����Ĵ��ڱ��뱻�������зǶ��㴰�ڵ����� ��ʹ��������Ѿ������ڼ���״̬ �����Ǳ�����������
		12.WS_POPUP������һ������ʽ���� ����ʽ������WS_CHILD��ʽͬʱʹ��
		13.WS_VISIBLE������һ����ʼ״̬Ϊ�ɼ��Ĵ���
		x��ָ���������Ͻǵ�X����
		y��ָ���������Ͻǵ�Y����
		nWidth�����豸��Ԫָ�����ڵĿ��
		nHeight�����豸��Ԫָ�����ڵĸ߶�
		hWndParent��ָ���������Ĵ��ڵĸ����ڵľ�� ��Ҫ����һ���Ӵ��� ���ṩһ����Ч�ĸ����ھ��
		hMenu���˵���� ָ�����ڸô��ڵĲ˵�
		hInstance��wWinMain()�����д����Ӧ�ó���ʵ���ľ��
		lpParam����ΪWM_CREATE��Ϣ�ĸ��Ӳ���lpParam���������ָ�� �ڴ������ĵ�����Ŀͻ�����ʱ lpParam����ָ��CLIENTCREATESTRUCT�ṹ�� �������ڽ������������ΪNULL */
		CreateWindowW(L"DirectX Framework", L"DirectX Framework", WS_EX_TOPMOST | WS_OVERLAPPEDWINDOW, 0, 0, width, height, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		MessageBox(0, L"CreateWindow Failed", 0, 0);
		return 0;
	}

	/* WINUSERAPI BOOL WINAPI ShowWindow(_In_ HWND hWnd, _In_ int nCmdShow); �ڴ��ڴ����ɹ�֮�󽫴�����ʾ��������
	hWnd������CreateWindow()�ɹ��������ڷ��صĴ��ھ��
	nCmdShow��ָ��������ʾ��״̬ �����Ǽ������õĴ�����ʾ״̬
	1.SW_HIDE�����ش��ڲ�������������
	2.SW_SHOW���ڴ���ԭ����λ����ԭ���ĳߴ缤�����ʾ����
	3.SW_SHOWMAXIMIZED������ڲ��������ʾ
	4.SW_SHOWMINIMIZED������ڲ�����С����ʾ
	6.SW_SHOWNORMAL�������ʾ���� �������ʱ��С������󻯵�״̬ ϵͳ����ָ���ԭ���ĳߴ�ʹ�С Ӧ�ó����ڵ�һ����ʾ����ʱӦָ����״̬ */
	ShowWindow(hWnd, SW_SHOW);

	/* WINUSERAPI BOOL WINAPI UpdateWindow(_In_ HWND hWnd); ͨ������һ��WM_PAINT��Ϣ��ˢ�´���
	������ڸ��µ�����Ϊ�� UpdateWindow()�����ƹ�Ӧ�ó������Ϣ����ֱ�ӷ���WM_PAINT��ָ�����ڵĴ��ڹ��̺������д���
	������ڸ��µ�����Ϊ�� �򲻷�����Ϣ
	hWnd������CreateWindow()�ɹ��������ڷ��صĴ��ھ�� */
	UpdateWindow(hWnd);

	return hWnd;
}

// У���Կ��Ƿ�֧��Ӳ����������
int D3DFramework::D3D9CheckHardwareVP(IDirect3D9 *d3d9, D3DDEVTYPE deviceType)
{
	int vp = 0;
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	return vp;
}

// ���D3DPRESENT_PARAMETERS�ṹ��
D3DPRESENT_PARAMETERS D3DFramework::D3D9CreatePP(HWND hWnd, int width, int height, bool fullScreen)
{
	D3DPRESENT_PARAMETERS d3dpp;

	// BackBufferWidth����̨����Ŀ��
	d3dpp.BackBufferWidth = width;
	// BackBufferHeight����̨����ĸ߶�
	d3dpp.BackBufferHeight = height;
	/* BackBufferFormat����̨�������������صĸ�ʽ
	�����Ա��һ��D3DFORMATö������ ����ֵ�кܶ���
	DX9ֻ֧��16λ��32λ�ĺ�̨�����ʽ ��֧��24λ �����D3DFORMAT����Ϥ���Խ���ֵ����ΪD3DFMT_UNKNOWN ��ʱ��ʹ������ĸ�ʽ */
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	// BackBufferCount����̨����������Ŀ
	d3dpp.BackBufferCount = 1;
	/* MultiSampleType�����ز���������
	�������� ͨ������ΪD3DMULTISAMPLE_NONE */
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	/* MultiSampleQuality�����ز����������ȼ�
	��MultiSampleType��Աһ�����������Ⱦ�����в����ľ�� ����ȴ���ĺܶ��ڴ���Դ ���Ҳ��������Կ���֧���������趨�Ĺ���
	ͨ������Ϊ0 */
	d3dpp.MultiSampleQuality = 0;
	/* SwapEffect������������������
	ͨ������ΪD3DSWAPEFFECT_DISCARD ��ʾ����̨���渴�Ƶ�ǰ̨ʱ��պ�̨���� */
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// hDeviceWindow�����ھ��
	d3dpp.hDeviceWindow = hWnd;
	// Windowed���Ƿ񴰿���ʾ
	d3dpp.Windowed = !fullScreen;
	/* EnableAutoDepthStencil��������Ȼ���
	�����Ҫʹ��Z�Ỻ�����ģ�建��������Ϊtrue ��������Ϊfalse */
	d3dpp.EnableAutoDepthStencil = true;
	/* AutoDepthStencilFormat����Ȼ����ʽ
	�����ʹ����Ȼ��� �����Ա��û���� �����������Ȼ��� ��ô�����Ա��Ϊ��Ȼ����趨�����ʽ ͨ��ֵ��
	1.D3DFMT_24S8��24λ��Ȼ��� 8λģ�建��
	2.D3DFMT_24X8��24λ��Ȼ���
	3.D3DFMT_16��16λ��Ȼ��� */
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	// Flags��һ������Ϊ0��D3DPRESENTFLAT_LOCKABLE_BACKBUFFER
	d3dpp.Flags = 0;
	/* FullScreen_RefreshRateInHz����Ļ��ˢ���ʣ���λ��Hz)
	����趨��һ����ʾ����֧�ֵ�ˢ���� �����ܴ����豸�򷢳�������Ϣ ����һ������ΪD3DPRESENT_RATE_DEFAULT */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	/* PresentationInterval����Ⱦ�����ˢ�·�ʽ
	1.����ΪD3DPRESENT_INTERVAL_DEFAULT������ʾһ����Ⱦ�����ʱ��һ��Ҫ�ȴ���ʾ��ˢ����һ����Ļ
	2.����ΪD3DPRESENT_INTERVAL_IMMEDIATE����ʵʱ�ķ�ʽ��Ⱦ���� �������֡�ʣ�Fps�� �����п��ܳ��ֻ���˺�ѵ���� */
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	return d3dpp;
}

// �����豸
bool D3DFramework::D3D9CreateDevice(IDirect3D9 *d3d9, D3DDEVTYPE deviceType, HWND hWnd, int vp, D3DPRESENT_PARAMETERS d3dpp, IDirect3DDevice9 **device)
{
	HRESULT hr = 0;
	/* HRESULT CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags,
	D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface); �����豸
	Adapter��ָ��Ҫ���ʵ��Կ� ����ж���Կ���D3DADAPTER_DEFAULT��ʾ���Կ�
	DeviceType������ָ��Direct3D�е���Ⱦ��ʽ �ò����ж�����䷽ʽ
	1.D3DDEVTYPE_HAL������Ӳ����Ⱦ
	2.D3DDEVTYPE_REF�����������Ⱦ
	3.D3DDEVTYPE_NULLREF������ҪӲ�������֧��
	4.D3DDEVTYPE_SW������Ҫ������Ⱦ�����Ŀɲ�����
	hFocusWindow�����豸��صĴ��ھ�� ���ĸ����ڻ��ƾ����ĸ����ڵľ��
	BehaviourFlags��ʹ��Ӳ������������ж��㴦�� Ӧʹ��֮ǰ�Ѿ�У������Ƿ�֧��Ӳ�����㴦�����ķ���ֵ�����
	���֧��Ӳ�����㴦���趨ΪD3DCREATE_HARDWARE_VERTEXPROCESSING
	���֧��������㴦���趨ΪD3DCREATE_SOFTWARE_VERTEXPROCESSING
	PresentationParameters�����֮ǰ�Ѿ��趨�õ�D3DPRESENT_PARAMETERS�ṹ��
	ppReturnedDeviceInterface�����Է��ش�����ɵ��豸 */
	hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hWnd, vp, &d3dpp, device);
	if (FAILED(hr))
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hWnd, vp, &d3dpp, device);
		if (FAILED(hr))
		{
			MessageBox(0, L"D3D9CreateDeivce Faile", 0, 0);
			d3d9->Release();
			return false;
		}
	}
	d3d9->Release();
	return true;
}

// Direct3D9����Ϣѭ��
int D3DFramework::D3D9MessageLoop(bool(*p_Display)(float time, IDirect3DDevice9 *Device), IDirect3DDevice9 *device)
{
	/* typedef struct tagMSG {
	HWND        hwnd;
	UINT        message;
	WPARAM      wParam;
	LPARAM      lParam;
	DWORD       time;
	POINT       pt;
	} MSG, *PMSG, NEAR *NPMSG, FAR *LPMSG; ��Ϣ�ṹ��
	hwnd����ʾ��Ϣ�����Ĵ��ھ��
	message��ָ������Ϣ�ı�ʶ�� ��Windows�� ��Ϣ����һ����ֵ����ʾ�� ��ͬ����Ϣ��Ӧ��ͬ����ֵ
	wParam������ָ����Ϣ�ĸ�����Ϣ
	lParam������ָ����Ϣ�ĸ�����Ϣ
	time����ʾ��ϢͶ�ݵ���Ϣ�����е�ʱ��
	pt����ʾ���ĵ�ǰλ�� */
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	static float time_1 = (float)timeGetTime();

	/* WINUSERAPI BOOL WINAPI GetMessageW(_Out_ LPMSG lpMsg, _In_opt_ HWND hWnd, _In_ UINT wMsgFilterMin, _In_ UINT wMsgFilterMax); ����Ϣ������ȡ����Ϣ
	����������յ�WM_QUIT�����Ϣ ���ط�0ֵ ������յ�WM_QUIT��Ϣ����0 ����������󷵻�-1
	lpMsg��ָ��һ����ϢMSG�ṹ�� GetMessage()���̵߳���Ϣ������ȡ������Ϣ�������ڸýṹ�������
	hWnd��ָ������������һ�����ڵ���Ϣ ָ��ΪNULLʱ �����������ڵ����̵߳����д��ڵĴ�����Ϣ
	wMsgFilterMin��ָ��Ҫ��ȡ����Ϣ����Сֵ ͨ������Ϊ0
	wMsgFilterMax��ָ��Ҫ��ȡ����Ϣ�����ֵ ���wMsgFilterMin��wMsgFilterMax������Ϊ0 �����������Ϣ
	------------------------------------------------------------------------------------------------------------------------------------------------
	���壺
	WINUSERAPI BOOL WINAPI PeekMessageW(_Out_ LPMSG lpMsg, _In_opt_ HWND hWnd, _In_ UINT wMsgFilterMin, _In_ UINT wMsgFilterMax, _In_ UINT wRemoveMsg);
	wRemoveMsg��ָ����Ϣ��ȡ�ķ�ʽ �������ΪPM_NOREMOVE����Ϣ���������Ϣ�����б��Ƴ� �������ΪPM_REMOVE����Ϣ������Ϣ�����б��Ƴ�
	*/
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			/* WINUSERAPI BOOL WINAPI TranslateMessage(_In_ CONST MSG *lpMsg); ���������Ϣת��Ϊ�ַ���Ϣ
			�ַ���Ϣ���͵������̵߳���Ϣ������ ����һ���̵߳��ú���GetMessage()��PeekMessage()ʱ������
			lpMsg��ָ��MSG�ṹ���ָ�� �ýṹ���ڴ�ŵ��ú���GetMessage()����PeekMessage()����Ϣ������ȡ������Ϣ */
			TranslateMessage(&msg);

			/* WINUSERAPI LRESULT WINAPI DispatchMessageW(_In_ CONST MSG *lpMsg); ����һ����Ϣ�����ڹ��� �ɴ��ڹ��̺�������Ϣ���д���
			lpMsg��ָ������Ϣ��MSG�ṹ��ָ��
			����ֵ�����ڹ��̵ķ���ֵ */
			DispatchMessage(&msg);
		}
		else
		{
			float time_2 = (float)timeGetTime();
			float intervalTime = time_2 - time_1;
			p_Display(intervalTime, device);
			time_1 = time_2;
		}
		//return true;
	}
	return true;
}

