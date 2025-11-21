#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NOMINMAX
#include <windows.h>
#include <assert.h>
#include <functional>

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")


#include <DirectXMath.h>
using namespace DirectX;

#include "DirectXTex.h"
#include "vector3.h"
#include "vector2.h"

#if _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex_Release.lib")
#endif


#pragma comment (lib, "winmm.lib")


#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)

extern LONG g_mouseX;
extern LONG g_mouseY;

extern float g_MouseSensitivity;

extern float g_MouseWheel;

extern bool g_MouseLeft;
extern bool g_MouseRight;
extern bool g_MouseLeftDown;
extern bool g_MouseLeftUp;
extern bool g_MouseRightDown;
extern bool g_MouseRightUp;


static void UpdateMouseButtons()
{
    static bool prevLeft = false;
    static bool prevRight = false;

    g_MouseLeftDown = (!prevLeft && g_MouseLeft);
    g_MouseLeftUp = (prevLeft && !g_MouseLeft);
    g_MouseRightDown = (!prevRight && g_MouseRight);
    g_MouseRightUp = (prevRight && !g_MouseRight);

    prevLeft = g_MouseLeft;
    prevRight = g_MouseRight;
}

HWND GetWindow();

POINT GetMousePos();
float GetMouseWheel();


