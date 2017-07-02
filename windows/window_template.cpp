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
	wndclass.style = CS_HREDRAW | CS_VREDRAW;  //���ڷ��
	wndclass.lpfnWndProc = WindowProc;  //���ڹ���
	wndclass.cbClsExtra = 0;  
	//The number of extra bytes to allocate following the window-class structure.
	wndclass.cbWndExtra = 0;  //
	//The number of extra bytes to allocate following the window instance.
	// register a dialog box created by using the CLASS directive in the resource file, 
	//it must set this member to DLGWINDOWEXTRA.
	wndclass.hInstance = hInstance;  //��ǰ���ھ��
	wndclass.hIcon = NULL;
	//wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);  //����ͼ��
	wndclass.hCursor = NULL;
	//wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);  //�����ʽ
	wndclass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	//wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //���ڱ�����ˢ
	wndclass.lpszMenuName = NULL;  //���ڲ˵�
	wndclass.lpszClassName = szClassName;  //��������

										   //ע�ᴰ��
	RegisterClassEx(&wndclass);
	handle = CreateWindow(
		szClassName,  //�����������
		TEXT("Welcome"),  //���ڱ��⣨�����ڱ�������
		WS_OVERLAPPEDWINDOW,  //���ڷ��
		CW_USEDEFAULT,  //��ʼ��ʱx���λ��
		CW_USEDEFAULT,  //��ʼ��ʱy���λ��
		500,  //���ڿ��
		300,  //���ڸ߶�
		NULL,  //�����ھ��
		NULL,  //���ڲ˵����
		hInstance,  //��ǰ���ڵľ��
		NULL  //��ʹ�ø�ֵ
	);

	//��ʾ����
	ShowWindow(handle, nCmdShow);
	//���£����ƣ����������ҳ�� 
	//UpdateWindow(handle);
	//optional
	/**********�ڢ۲�����Ϣѭ��**********/
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);  //������Ϣ
		DispatchMessage(&msg);  //������Ϣ
	}
	return msg.wParam;
	return 0;
	/*
	int message_box = 
	MessageBox(NULL,TEXT("hello/can I success?"),TEXT("Hello"), 
	MB_OKCANCEL| MB_ICONEXCLAMATION);
	if (message_box == IDOK)
	{
		MessageBox(NULL, TEXT("�����ˡ�ȷ������ť"), TEXT("��ʾ"), MB_OK);
	}
	else
	{
		MessageBox(NULL, TEXT("�����ˡ�ȡ������ť"), TEXT("��ʾ"), MB_OK);
	}
	*/

}
LRESULT CALLBACK WindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	HDC         hdc;  //�豸�������
	PAINTSTRUCT ps;
	RECT        rect;
	switch (uMsg) {
		//���ڻ�����Ϣ
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		DrawText(
			hdc,
			TEXT("windowӦ�ó���ģ��"),
			-1,
			&rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER
		);
		EndPaint(hwnd, &ps);
		return 0;
		//����������Ϣ
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}