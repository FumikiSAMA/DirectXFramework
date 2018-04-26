#include "stdafx.h"

D3DFramework::~D3DFramework()
{
}

// 单例模式
D3DFramework *D3DFramework::m_instance = new D3DFramework();
D3DFramework *D3DFramework::getInstance()
{
	return m_instance;
}

// 搭建Direct3D9框架
bool D3DFramework::D3D9Initial(HINSTANCE hInstance, WNDPROC wndProc, int width, int height, bool fullScreen, D3DDEVTYPE deviceType, IDirect3DDevice9 **device)
{
	// 创建窗口
	HWND hwnd = D3D9CreateWindow(hInstance, wndProc, width, height, fullScreen);
	if (!hwnd)
	{
		MessageBox(0, L"D3DCreateWindow Failed", 0, 0);
		return false;
	}

	/* 第一步 获取指向接口IDirect3D9指针
	使用Direct3DCreate9()函数并传入D3D_SDK_VERSION参数获取该指针
	传入D3D_SDK_VERSION参数用于确保应用程序使用的头文件在编译时能够与DirectX Runtime的动态链接库相匹配 保证程序运行时能够正确地与库链接
	Idrect3D9对象主要有两个用途：
	1.设备枚举 指获取系统可用的每块图形卡的性能 显示模式 格式以及其他信息
	2.创建IDirectDDevice9类型的对象 */
	IDirect3D9 *d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9)
	{
		MessageBox(0, L"Direct3DCreate9 Failed", 0, 0);
		return false;
	}

	/* 第二步 校验显卡是否支持硬件顶点运算
	创建一个代表主显卡的IDirect3DDevice9对象时 必须指定使用该对象进行顶点运算的类型 如果可以 首选硬件顶点运算
	开发应用程序时 若需要一些新的 特别的 或者高级的特性（硬件还没有广泛支持的特性） 优先检查一下设备（D3DCAPS）是否支持该特性是很有必要的 */
	int vp = D3D9CheckHardwareVP(d3d9, deviceType);

	/* 第三步 填充D3DPRESENT_PARAMETERS结构
	该结构用来指定即将要创建的IDirect3DDevice9类型对象的一些特性 稍微熟悉一下该结构 有坑 */
	D3DPRESENT_PARAMETERS d3dpp = D3D9CreatePP(hwnd, width, height, fullScreen);

	/* 第四步 创建设备
	如果创建成功返回返回D3D_OK 否则返回各种HRESULT类型的错误码 */
	return D3D9CreateDevice(d3d9, deviceType, hwnd, vp, d3dpp, device);
}

// 注册 创建并显示窗口
HWND D3DFramework::D3D9CreateWindow(HINSTANCE hInstance, WNDPROC wndProc, int width, int height, bool fullScreen)
{
	// 设计窗口类
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	/* style：指定这一类型窗口的样式 该成员可以是以下样式的任意组合（按位或组合）：
	1.CS_VREDRAW：垂直重绘 当窗口垂直方向上的高度发生变化时 将重新绘制整个窗口
	2.CS_HREDRAW：水平重绘 当窗口水平方向上的宽度发生变化时 将重新绘制整个窗口
	3.CS_OWNDC：独占设备描述表 为该类中的每个窗口分配一个单值的设备描述表
	4.CS_SAVEBITS：在一个窗口中保存用户图像 以便于在该窗口被遮住或移动时不必每次刷新屏幕 但是会占用更多内存
	5.CS_DBLCLKS：窗口可以检测鼠标双击事件 当用户在窗口中双击鼠标时 向窗口过程发送鼠标双击消息
	6.CS_BYTEALLGNCLIENT：鼠标用户区域按字节对齐显示
	7.CS_BYTEALLGNWINDOW：鼠标用户窗口按字节对齐显示
	8.CS_PARENTDC：在父窗口中设定一个子窗口的剪切区 以便于子窗口能够画在符窗口中
	9.CS_NOCLOSE：系统菜单中没有CLOSE菜单项 窗口没有关闭按钮 */
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	/* lpfnWndProc：指向窗口过程函数的函数指针 窗口过程函数是一个回调函数 针对Windows的消息处理机制而被调用的过程如下：
	1.在设计窗口类的时候 将窗口过程函数的地址赋值给lpfnWndProc成员变量
	2.调用RegisterClass(&wndclass)注册窗口类 系统就有了用户编写的窗口过程函数的地址
	3.当应用程序接收到某一窗口的信息时 调用DispatchMessage(&msg)将消息回传给系统 系统则利用先前注册窗口类时得到的函数指针 调用窗口过程函数对消息进行处理 */
	wcex.lpfnWndProc = (WNDPROC)wndProc;
	// cbClsExtra：Windows系统为窗口类结构分配或追加的额外字节数 一般将该值初始化为0
	wcex.cbClsExtra = 0;
	/* cbWndExtra：Windows系统为窗口实例分配或追加的额外的字节数 一般将该值初始化为0
	但如果应用程序使用资源文件里的CLASS指令创建对话框 并用WNDCLASS结构注册该对话框时 cbWndExtra必须初始化为DLGWINDOWEXTRA */
	wcex.cbWndExtra = 0;
	// hInstance：包含窗口过程的程序的实例句柄 一般直接初始化为wWinMain()函数的hInstance参数值即可
	wcex.hInstance = hInstance;
	// hIcon：窗口类的图标句柄 这个成员变量必须是一个图标资源的句柄 可以使用LoadIcon()函数加载图标 如果hIcon句柄为NULL 那么系统将提供一个默认的图标
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	/* hCursor：窗口类的光标句柄 这个成员变量必须是一个光标自愿的句柄 可以使用LoadCursor()函数加载光标并返回系统分配给该光标的句柄
	如果hCursor为NULL 应用程序必须在鼠标进入应用程序窗口时 明确设置光标的形状（句柄） */
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	// hbrBackground：窗口类的背景画刷句柄 当窗口发生重绘时 系统使用这里指定的画刷来填充窗口的背景 该成员可指定为用于绘制背景的物理画刷的句柄 也可以指定为一个标准的系统颜色值
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	/* lpszMenuName：指向一个以空终止的字符串 该字符串描述菜单的资源名 若是用一个整数来标识菜单 需要用MAKEINTRESOURCE宏来替换
	如果将lpszMenuName设置为NULL 那么基于这个窗口类创建的窗口将没有默认菜单 */
	wcex.lpszMenuName = NULL;  // MAKEINTRESOURCEW(IDC_MAGICTOWER);
							   // lpszClassName：指向一个以空终止的字符串 该字符串描述窗口类的名字 这个类名可以是由RegisterClass或者RegisterClassEx注册的名字 或是任何与定义的控件类名
	wcex.lpszClassName = L"DirectX Framework";
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	// 注册窗口
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(0, L"RegisterClass Failed", 0, 0);
		return 0;
	}

	// 创建窗口
	HWND hWnd =
		/* #define CreateWindowW(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam) \
		CreateWindowExW(0L, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
		lpClassName：指定窗口类的名称 即设计Windows类中为WNDCLASS的lpszCalssName成员指定的名称
		如果在调用CreateWindow函数之前没有用RegisterClass函数注册这个类 操作系统将无法得知这一类型窗口的相关信息从而导致创建窗口失败
		lpWindowName：指向一个以空结束的字符串指针 用来指定窗口名称 如果窗口样式指定了标题栏 那么这里指向的窗口名称将显示在窗口标题栏上
		dwStyle：指定创建窗口的样式 该参数可以是不同窗口样式的组合 以下是几种常用的窗口样式
		1.WS_CAPTION：创建一个有标题栏的窗口
		2.WS_SYSMENU：创建一个在标题栏上带有系统菜单的窗口 必须和WS_CAPTION类型一起使用
		3.WS_THICKFRAME：创建一个具有可调边框的窗口
		4.WS_MINIMIZEBOX：创建一个具有最小化按钮的窗口 必须同时设定WS_SYSMENU类型
		5.WS_MAXIMIZEBOS：创建一个具有最大化按钮的窗口 必须同时设定WS_SYSMENU类型
		6.WS_TILED：创建一个层叠的窗口 一个层叠的窗口有一个标题栏和一个边框
		7.WS_TILEDWINDOW：创建一个具有WS_OVERLAPPED WS_CAPTION WS_SYSMENU WS_THICKFRAME样式的窗口
		8.WS_CHILD：创建窗口为子窗口 不能应用于弹出式窗口样式
		9.WS_OVERLAPPED：产生一个层叠的窗口 一个层叠的窗口有一个标题栏和一个边框 与WS_TILED样式相同
		10.WS_OVERLAPPEDWINDOW：创建一个具有WS_OVERLAPPED WS_CAPTION WS_SYSMENU WS_THICKFRAME WS_MINIMIZEBOS MS_MAXMIZEBOS样式的层叠窗口 与WS_TILEDWINDOW样式相同
		11.WS_EX_TOPMOST：指定用这个样式创建的窗口必须被放在所有非顶层窗口的上面 即使这个窗口已经不处于激活状态 它还是保留在最上面
		12.WS_POPUP：创建一个弹出式窗口 该样式不能与WS_CHILD样式同时使用
		13.WS_VISIBLE：创建一个初始状态为可见的窗口
		x：指定窗口左上角的X坐标
		y：指定窗口左上角的Y坐标
		nWidth：以设备单元指定窗口的宽度
		nHeight：以设备单元指定窗口的高度
		hWndParent：指定被创建的窗口的父窗口的句柄 若要创建一个子窗口 需提供一个有效的父窗口句柄
		hMenu：菜单句柄 指向附属于该窗口的菜单
		hInstance：wWinMain()函数中传入的应用程序实例的句柄
		lpParam：作为WM_CREATE消息的附加参数lpParam传入的数据指针 在创建多文档界面的客户窗口时 lpParam必须指向CLIENTCREATESTRUCT结构体 多数窗口将这个参数设置为NULL */
		CreateWindowW(L"DirectX Framework", L"DirectX Framework", WS_EX_TOPMOST | WS_OVERLAPPEDWINDOW, 0, 0, width, height, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		MessageBox(0, L"CreateWindow Failed", 0, 0);
		return 0;
	}

	/* WINUSERAPI BOOL WINAPI ShowWindow(_In_ HWND hWnd, _In_ int nCmdShow); 在窗口创建成功之后将窗口显示在桌面上
	hWnd：调用CreateWindow()成功创建窗口返回的窗口句柄
	nCmdShow：指定窗口显示的状态 以下是几个常用的窗口显示状态
	1.SW_HIDE：隐藏窗口并激活其他窗口
	2.SW_SHOW：在窗口原来的位置以原来的尺寸激活和显示窗口
	3.SW_SHOWMAXIMIZED：激活窗口并以最大化显示
	4.SW_SHOWMINIMIZED：激活窗口并以最小化显示
	6.SW_SHOWNORMAL：激活并显示窗口 如果窗口时最小化或最大化的状态 系统将其恢复到原来的尺寸和大小 应用程序在第一次显示窗口时应指定此状态 */
	ShowWindow(hWnd, SW_SHOW);

	/* WINUSERAPI BOOL WINAPI UpdateWindow(_In_ HWND hWnd); 通过发送一个WM_PAINT消息来刷新窗口
	如果窗口更新的区域不为空 UpdateWindow()函数绕过应用程序的消息队列直接发送WM_PAINT给指定窗口的窗口过程函数进行处理
	如果窗口更新的区域为空 则不发送消息
	hWnd：调用CreateWindow()成功创建窗口返回的窗口句柄 */
	UpdateWindow(hWnd);

	return hWnd;
}

// 校验显卡是否支持硬件顶点运算
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

// 填充D3DPRESENT_PARAMETERS结构体
D3DPRESENT_PARAMETERS D3DFramework::D3D9CreatePP(HWND hWnd, int width, int height, bool fullScreen)
{
	D3DPRESENT_PARAMETERS d3dpp;

	// BackBufferWidth：后台缓存的宽度
	d3dpp.BackBufferWidth = width;
	// BackBufferHeight：后台缓存的高度
	d3dpp.BackBufferHeight = height;
	/* BackBufferFormat：后台缓冲区保存像素的格式
	这个成员是一个D3DFORMAT枚举类型 它的值有很多种
	DX9只支持16位和32位的后台缓存格式 不支持24位 如果对D3DFORMAT不熟悉可以将该值设置为D3DFMT_UNKNOWN 这时将使用桌面的格式 */
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	// BackBufferCount：后台缓冲区的数目
	d3dpp.BackBufferCount = 1;
	/* MultiSampleType：多重采样的类型
	抗锯齿相关 通常设置为D3DMULTISAMPLE_NONE */
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	/* MultiSampleQuality：多重采样的质量等级
	和MultiSampleType成员一起可以消除渲染过程中产生的锯齿 但是却消耗很多内存资源 并且不是所有显卡都支持这两者设定的功能
	通常设置为0 */
	d3dpp.MultiSampleQuality = 0;
	/* SwapEffect：交换缓冲区的类型
	通常设置为D3DSWAPEFFECT_DISCARD 表示当后台缓存复制到前台时清空后台缓存 */
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// hDeviceWindow：窗口句柄
	d3dpp.hDeviceWindow = hWnd;
	// Windowed：是否窗口显示
	d3dpp.Windowed = !fullScreen;
	/* EnableAutoDepthStencil：激活深度缓存
	如果需要使用Z轴缓存或者模板缓存则设置为true 否则设置为false */
	d3dpp.EnableAutoDepthStencil = true;
	/* AutoDepthStencilFormat：深度缓存格式
	如果不使用深度缓存 这个成员就没有用 如果启用了深度缓存 那么这个成员将为深度缓存设定缓存格式 通常值有
	1.D3DFMT_24S8：24位深度缓存 8位模板缓存
	2.D3DFMT_24X8：24位深度缓存
	3.D3DFMT_16：16位深度缓存 */
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	// Flags：一般设置为0或D3DPRESENTFLAT_LOCKABLE_BACKBUFFER
	d3dpp.Flags = 0;
	/* FullScreen_RefreshRateInHz：屏幕的刷新率（单位：Hz)
	如果设定了一个显示器不支持的刷新率 将不能创建设备或发出警告信息 所以一般设置为D3DPRESENT_RATE_DEFAULT */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	/* PresentationInterval：渲染画面的刷新方式
	1.设置为D3DPRESENT_INTERVAL_DEFAULT：在显示一个渲染画面的时候一定要等待显示器刷新完一次屏幕
	2.设置为D3DPRESENT_INTERVAL_IMMEDIATE：以实时的方式渲染画面 可以提高帧率（Fps） 但是有可能出现画面撕裂的情况 */
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	return d3dpp;
}

// 创建设备
bool D3DFramework::D3D9CreateDevice(IDirect3D9 *d3d9, D3DDEVTYPE deviceType, HWND hWnd, int vp, D3DPRESENT_PARAMETERS d3dpp, IDirect3DDevice9 **device)
{
	HRESULT hr = 0;
	/* HRESULT CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags,
	D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface); 创建设备
	Adapter：指定要访问的显卡 如果有多块显卡则D3DADAPTER_DEFAULT表示主显卡
	DeviceType：用于指定Direct3D中的渲染方式 该参数有多种填充方式
	1.D3DDEVTYPE_HAL：采用硬件渲染
	2.D3DDEVTYPE_REF：采用软件渲染
	3.D3DDEVTYPE_NULLREF：不需要硬件和软件支持
	4.D3DDEVTYPE_SW：采用要进行渲染工作的可插拔软件
	hFocusWindow：与设备相关的窗口句柄 在哪个窗口绘制就填哪个窗口的句柄
	BehaviourFlags：使用硬件或者软件进行顶点处理 应使用之前已经校验过的是否支持硬件顶点处理函数的返回值来填充
	如果支持硬件顶点处理：设定为D3DCREATE_HARDWARE_VERTEXPROCESSING
	如果支持软件顶点处理：设定为D3DCREATE_SOFTWARE_VERTEXPROCESSING
	PresentationParameters：填充之前已经设定好的D3DPRESENT_PARAMETERS结构体
	ppReturnedDeviceInterface：用以返回创建完成的设备 */
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

// Direct3D9的消息循环
int D3DFramework::D3D9MessageLoop(bool(*p_Display)(float time, IDirect3DDevice9 *Device), IDirect3DDevice9 *device)
{
	/* typedef struct tagMSG {
	HWND        hwnd;
	UINT        message;
	WPARAM      wParam;
	LPARAM      lParam;
	DWORD       time;
	POINT       pt;
	} MSG, *PMSG, NEAR *NPMSG, FAR *LPMSG; 消息结构体
	hwnd：表示消息所属的窗口句柄
	message：指定了消息的标识符 在Windows中 消息是由一个数值来表示的 不同的消息对应不同的数值
	wParam：用于指定消息的附加信息
	lParam：用于指定消息的附加信息
	time：表示消息投递到消息队列中的时间
	pt：表示鼠标的当前位置 */
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	static float time_1 = (float)timeGetTime();

	/* WINUSERAPI BOOL WINAPI GetMessageW(_Out_ LPMSG lpMsg, _In_opt_ HWND hWnd, _In_ UINT wMsgFilterMin, _In_ UINT wMsgFilterMax); 从消息队列中取出消息
	如果函数接收到WM_QUIT外的消息 返回非0值 如果接收到WM_QUIT消息返回0 如果发生错误返回-1
	lpMsg：指向一个消息MSG结构体 GetMessage()从线程的消息队列中取出的消息将保存在该结构体对象中
	hWnd：指定接收属于哪一个窗口的消息 指定为NULL时 函数接收属于调用线程的所有窗口的窗口消息
	wMsgFilterMin：指定要获取的消息的最小值 通常设置为0
	wMsgFilterMax：指定要获取的消息的最大值 如果wMsgFilterMin和wMsgFilterMax均设置为0 则接收所有消息
	------------------------------------------------------------------------------------------------------------------------------------------------
	变体：
	WINUSERAPI BOOL WINAPI PeekMessageW(_Out_ LPMSG lpMsg, _In_opt_ HWND hWnd, _In_ UINT wMsgFilterMin, _In_ UINT wMsgFilterMax, _In_ UINT wRemoveMsg);
	wRemoveMsg：指定消息获取的方式 如果设置为PM_NOREMOVE则消息将不会从消息队列中被移除 如果设置为PM_REMOVE则消息将从消息队列中被移除
	*/
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			/* WINUSERAPI BOOL WINAPI TranslateMessage(_In_ CONST MSG *lpMsg); 将虚拟键消息转换为字符消息
			字符消息被送到调用线程的消息队列里 当下一次线程调用函数GetMessage()或PeekMessage()时被读出
			lpMsg：指向MSG结构体的指针 该结构用于存放调用函数GetMessage()或者PeekMessage()从消息队列里取出的消息 */
			TranslateMessage(&msg);

			/* WINUSERAPI LRESULT WINAPI DispatchMessageW(_In_ CONST MSG *lpMsg); 分派一个消息给窗口过程 由窗口过程函数对消息进行处理
			lpMsg：指向含有消息的MSG结构的指针
			返回值：窗口过程的返回值 */
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

