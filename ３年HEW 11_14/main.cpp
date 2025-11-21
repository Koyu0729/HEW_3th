#include "main.h"
#include "manager.h"
#include <thread>
#include "kosuCRI.h"

#include <vector> //動的にする用

// Unicode文字のやつ
//const wchar_t* CLASS_NAME = L"AppClass";
//const wchar_t* WINDOW_NAME = L"DX11ゲーム";

// マルチバイト文字のやつ
const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "DX11ゲーム";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


HWND g_Window;

HWND GetWindow()
{
	return g_Window;
}

POINT GetMousePos()
{
	POINT pos;
	GetCursorPos(&pos);
	ScreenToClient(g_Window, &pos); // g_Window は main.cpp のグローバル変数
	return pos;
}

float GetMouseWheel()
{
	return g_MouseWheel;
}


//マウス
static void RegisterRawMouse(HWND hWnd) {
//https://learn.microsoft.com/ja-jp/windows/win32/inputdev/using-raw-input

	RAWINPUTDEVICE rid{};
	rid.usUsagePage = 0x01; //マウス、キーボード、ジョイスティック、ゲームパッドの判定取得用
	rid.usUsage = 0x02; // マウス
	rid.dwFlags = 0;   
	rid.hwndTarget = hWnd;
	RegisterRawInputDevices(&rid, 1, sizeof(rid));
}
LONG g_mouseX = 0;
LONG g_mouseY = 0;

bool g_MouseLeft = false;
bool g_MouseRight = false;
bool g_MouseLeftDown = false;
bool g_MouseLeftUp = false;
bool g_MouseRightDown = false;
bool g_MouseRightUp = false;

float g_MouseWheel = 0.0f;

float g_MouseSensitivity = 0.015f;


static std::vector<BYTE> g_rawBuf;



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);

	printf("デバック用\n");

	WNDCLASSEX wcex;
	{
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = 0;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = CLASS_NAME;
		wcex.hIconSm = nullptr;

		RegisterClassEx(&wcex);


		RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
	}

	//initMouse
	RegisterRawMouse(g_Window);


	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
    
	//CRI初期化関数＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	AudioManager::Init();
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	Manager::Init();



	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);

	// ===== ここでカーソル非表示＋クリップ =====
	ShowCursor(FALSE);

	RECT rect;
	GetClientRect(g_Window, &rect);
	POINT ul = { rect.left, rect.top };
	POINT lr = { rect.right, rect.bottom };
	ClientToScreen(g_Window, &ul);
	ClientToScreen(g_Window, &lr);
	RECT clipRect = { ul.x, ul.y, lr.x, lr.y };
	ClipCursor(&clipRect);


	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;



	MSG msg;
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				//CRI
				AudioManager::Update();

				UpdateMouseButtons();
			
				Manager::Update();
				Manager::Draw();

				//マウスリセット
				g_mouseX = 0;
				g_mouseY = 0;
				g_MouseWheel = 0.0f;

				// ==========================
				// FPS計測＆ウィンドウタイトル表示
				// ==========================
				static int frameCount = 0;
				static DWORD lastTime = timeGetTime();
				frameCount++;

				DWORD now = timeGetTime();
				if (now - lastTime >= 1000) // 1秒ごとに更新
				{
					float fps = frameCount * 1000.0f / (now - lastTime);
					frameCount = 0;
					lastTime = now;

					char title[256];
					sprintf_s(title, "DX11ゲーム - FPS: %.1f", fps);
					SetWindowTextA(g_Window, title);
				}
			}
		}
	}
	ShowCursor(TRUE);
	ClipCursor(NULL);   // 閉じ込め解除


	timeEndPeriod(1);

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	//CRI
	AudioManager::Uninit();

	Manager::Uninit();

	CoUninitialize();

	return (int)msg.wParam;
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch(uMsg)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_INPUT:
	{	
		//https://qiita.com/i_saint/items/957d76a238bd8f046b64

		auto hRawInput = (HRAWINPUT)lParam;
		UINT dwSize = 0;
		std::vector<BYTE>buf;

		GetRawInputData(hRawInput, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
		buf.resize(dwSize);
		GetRawInputData(hRawInput, RID_INPUT, buf.data(), &dwSize, sizeof(RAWINPUTHEADER));

		RAWINPUT* ri = reinterpret_cast<RAWINPUT*>(buf.data());
		if (ri->header.dwType != RIM_TYPEMOUSE) break;

		g_mouseX += ri->data.mouse.lLastX;  // ← ΔX（このWM_INPUT一件ぶん）
		g_mouseY += ri->data.mouse.lLastY;  // ← ΔY


		if (ri->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)  g_MouseLeft = true;
		if (ri->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)    g_MouseLeft = false;

		if (ri->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) g_MouseRight = true;
		if (ri->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)   g_MouseRight = false;

		//判定取れてるか確認
		//printf("%04x\n", ri->data.mouse.usButtonFlags);
	}
		break; 
	case WM_MOUSEWHEEL:
	{
		short delta = GET_WHEEL_DELTA_WPARAM(wParam); // 120 or -120
		g_MouseWheel = (float)delta / 120.0f;          // 1 or -1 に正規化
	}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}