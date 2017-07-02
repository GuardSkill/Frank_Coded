#include <Windows.h>
LRESULT CALLBACK WindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	static TCHAR szClassName[] = TEXT("myClass");
	HWND handle;
	MSG msg;
	WNDCLASSEX wndclass = { 0 };

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;  //窗口风格
	wndclass.lpfnWndProc = WindowProc;  //窗口过程
	wndclass.cbClsExtra = 0;  
	//The number of extra bytes to allocate following the window-class structure.
	wndclass.cbWndExtra = 0;  //
	//The number of extra bytes to allocate following the window instance.
	// register a dialog box created by using the CLASS directive in the resource file, 
	//it must set this member to DLGWINDOWEXTRA.
	wndclass.hInstance = hInstance;  //当前窗口句柄
	wndclass.hIcon = NULL;
	//wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);  //窗口图标
	wndclass.hCursor = NULL;
	//wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);  //鼠标样式
	wndclass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	//wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //窗口背景画刷
	wndclass.lpszMenuName = NULL;  //窗口菜单
	wndclass.lpszClassName = szClassName;  //窗口类名

										   //注册窗口
	RegisterClassEx(&wndclass);
	handle = CreateWindow(
		szClassName,  //窗口类的名字
		TEXT("Welcome"),  //窗口标题（出现在标题栏）
		WS_OVERLAPPEDWINDOW,  //窗口风格
		CW_USEDEFAULT,  //初始化时x轴的位置
		CW_USEDEFAULT,  //初始化时y轴的位置
		500,  //窗口宽度
		300,  //窗口高度
		NULL,  //父窗口句柄
		NULL,  //窗口菜单句柄
		hInstance,  //当前窗口的句柄
		NULL  //不使用该值
	);

	//显示窗口
	ShowWindow(handle, nCmdShow);
	//更新（绘制）窗口里面的页面 
	//UpdateWindow(handle);
	//optional
	/**********第③步：消息循环**********/
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);  //翻译消息
		DispatchMessage(&msg);  //分派消息
	}
	return msg.wParam;
	return 0;
	/*
	int message_box = 
	MessageBox(NULL,TEXT("hello/can I success?"),TEXT("Hello"), 
	MB_OKCANCEL| MB_ICONEXCLAMATION);
	if (message_box == IDOK)
	{
		MessageBox(NULL, TEXT("你点击了“确定”按钮"), TEXT("提示"), MB_OK);
	}
	else
	{
		MessageBox(NULL, TEXT("你点击了“取消”按钮"), TEXT("提示"), MB_OK);
	}
	*/

}
LRESULT CALLBACK WindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	HDC         hdc;  //设备环境句柄
	PAINTSTRUCT ps;
	RECT        rect;
	switch (uMsg) {
		//窗口绘制消息
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		DrawText(
			hdc,
			TEXT("window应用程序模版"),
			-1,
			&rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER
		);
		EndPaint(hwnd, &ps);
		return 0;
		//窗口销毁消息
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}