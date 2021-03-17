#pragma once

#include "DirectOverlay.h"
#include <d2d1.h>
#include <dwrite.h>
#include <dwmapi.h>
#include <fstream>
#include <comdef.h>
#include <iostream>
#include <ctime>
//#include "examples/D2DOverlay Test External/Vector.h"
//#include "examples/D2DOverlay Test External/Colour.h"
#include "math.h"
#include "Colour.h"
#include "Overlay.h"
#include <vector>
#include "globalVars.h"
#include <mutex>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "dwrite.lib")


ID2D1Factory* factory;
ID2D1HwndRenderTarget* target;
ID2D1SolidColorBrush* solid_brush;
IDWriteFactory* w_factory;
IDWriteTextFormat* w_format;
IDWriteTextLayout* w_layout;
HWND overlayWindow;
HINSTANCE appInstance;
HWND targetWindow;
HWND enumWindow = NULL;
time_t preTime = clock();
time_t showTime = clock();
int fps = 0;

bool o_Foreground = false;
bool o_DrawFPS = false;
bool o_VSync = false;
std::wstring fontname = L"Courier";

DirectOverlayCallback drawLoopCallback = NULL;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


struct drawStructt {
	D2D1_POINT_2F point1, point2;
	ID2D1SolidColorBrush* solid_brush;
	float thickness;


	drawStructt(D2D1_POINT_2F point1_t, D2D1_POINT_2F point2_t, ID2D1SolidColorBrush* solid_brush_t, float thickness_t) {
		point1 = point1_t;
		point2 = point2_t;
		solid_brush = solid_brush_t;
		thickness = thickness_t;
	}
};

std::mutex mtx;
std::vector<drawStructt> drawingListy = {};

void DrawString(std::string str, float fontSize, Vector positon, Colour colour) {
	DrawString(str, fontSize, positon.x, positon.y, colour.r, colour.g, colour.b, colour.a);
}

void DrawString(std::string str, float fontSize, float x, float y, float r, float g, float b, float a)
{
	RECT re;
	GetClientRect(overlayWindow, &re);
	FLOAT dpix, dpiy;
	dpix = static_cast<float>(re.right - re.left);
	dpiy = static_cast<float>(re.bottom - re.top);
	HRESULT res = w_factory->CreateTextLayout(std::wstring(str.begin(), str.end()).c_str(), str.length() + 1, w_format, dpix, dpiy, &w_layout);
	if (SUCCEEDED(res))
	{
		DWRITE_TEXT_RANGE range = { 0, str.length() };
		w_layout->SetFontSize(fontSize, range);
		solid_brush->SetColor(D2D1::ColorF(r, g, b, a));
		target->DrawTextLayout(D2D1::Point2F(x, y), w_layout, solid_brush);
		w_layout->Release();
		w_layout = NULL;
	}
}

void DrawBoxVec(Vector startingPos, float width, float height, float thickness, Colour colour, bool filled) {
	DrawBox(startingPos.x, startingPos.y, width, height, thickness, colour.r, colour.g, colour.b, colour.a, filled);
}

void DrawRoundedBox(
	float x, float y, float x2, float y2, 
	float radius_x, float radius_y, float thickness, float r, float g, float b, float a, bool filled) {
	solid_brush->SetColor(D2D1::ColorF(r, g, b, a));
	if (filled)
		target->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(x, y, x2 + radius_x, y2 + radius_y), radius_x, radius_y), solid_brush);
	else
		target->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(x, y, x2 + radius_x, y2 + radius_y), radius_x, radius_y), solid_brush, thickness);

}

void DrawBox(Vector pos1, Vector pos2, float thickness, float r, float g, float b, float a, bool filled)
{
	solid_brush->SetColor(D2D1::ColorF(r, g, b, a));
	if (filled)
	{
		std::cout << "No fill esp yet, not stream proof yet.";
		//target->FillRectangle(D2D1::RectF(pos1.x, pos1.y, pos2.x, pos2.y), solid_brush);
	}
	else {
		DrawLine(pos1.x, pos1.y, pos1.x, pos2.y, 2.f, 1.f, 0.f, 0.f, 1.f); // Left line
		DrawLine(pos1.x, pos1.y, pos2.x, pos1.y, 2.f, 1.f, 0.f, 0.f, 1.f); // Bottom Line
		DrawLine(pos2.x, pos1.y, pos1.x, pos1.y, 2.f, 1.f, 0.f, 0.f, 1.f); // Right Line
		DrawLine(pos1.x, pos2.y, pos1.x, pos1.y, 2.f, 1.f, 0.f, 0.f, 1.f); // Top line
		//target->DrawRectangle(D2D1::RectF(pos1.x, pos1.y, pos2.x, pos2.y), solid_brush, thickness);

	}
}

void DrawBox(float x, float y, float width, float height, float thickness, float r, float g, float b, float a, bool filled)
{
	solid_brush->SetColor(D2D1::ColorF(r, g, b, a));
	if (filled)  target->FillRectangle(D2D1::RectF(x, y, x + width, y + height), solid_brush);
	else target->DrawRectangle(D2D1::RectF(x, y, x + width, y + height), solid_brush, thickness);
}

void DrawLine(float x1, float y1, float x2, float y2, float thickness, float r, float g, float b, float a) {
	solid_brush->SetColor(D2D1::ColorF(r, g, b, a));
	//target->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), solid_brush, thickness);

	drawingListy.push_back(drawStructt(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), solid_brush, thickness));
}

void clear() {
	drawingListy.clear();
}

void setupData(float frameTime) {
	gVars->frameTime = frameTime;
}

void DrawCircle(float x, float y, float radius, float thickness, float r, float g, float b, float a, bool filled)
{
	solid_brush->SetColor(D2D1::ColorF(r, g, b, a));
	if (filled) target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), solid_brush);
	else target->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), solid_brush, thickness);
}

void DrawEllipse(float x, float y, float width, float height, float thickness, float r, float g, float b, float a, bool filled)
{
	solid_brush->SetColor(D2D1::ColorF(r, g, b, a));
	if (filled) target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), width, height), solid_brush);
	else target->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), width, height), solid_brush, thickness);
}

void d2oSetup(HWND tWindow) {
	//tWindow = CreateWindowEx(NULL, "CLASS PLEaSE OMFG", "WINDOW NAMEEEE", WS_POPUP | WS_VISIBLE, 0, 0, 1920, 1080, NULL, NULL, 0, NULL);

	////
	//SetWindowLong(tWindow, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	////
	//MARGINS mar = { -1 };
	//DwmExtendFrameIntoClientArea(overlayWindow, &mar);
	//ShowWindow(tWindow, SW_SHOW);
	//UpdateWindow(tWindow);


	//targetWindow = tWindow;
	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(0);
	wc.lpszClassName = L"OverActivety_Class";
	RegisterClass(&wc);
	overlayWindow = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
		wc.lpszClassName, L"OverActivety_Overlay", WS_POPUP, 0, 0, 1920, 1080,
		NULL, NULL, wc.hInstance, NULL);

	targetWindow = overlayWindow;

	MARGINS mar = { -1 };
	DwmExtendFrameIntoClientArea(overlayWindow, &mar);
	D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);
	factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)),
		D2D1::HwndRenderTargetProperties(overlayWindow, D2D1::SizeU(200, 200),
			D2D1_PRESENT_OPTIONS_IMMEDIATELY), &target);
	target->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &solid_brush);
	target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&w_factory));
	w_factory->CreateTextFormat(fontname.c_str(), NULL, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 10.0f, L"en-us", &w_format);
}

bool setup = true;
void mainLoop() {
	MSG message;
	message.message = WM_NULL;
	ShowWindow(overlayWindow, 1);
	UpdateWindow(overlayWindow);
	SetLayeredWindowAttributes(overlayWindow, RGB(0, 0, 0), 255, LWA_ALPHA);
	//if (setup) {
	//	setup = !setup;
	//	std::cout << "overlayWindow" << ": " << SetWindowDisplayAffinity(overlayWindow, 0x00000011) << std::endl;
	//	std::cout << "overlayWindow last error" << ": " << GetLastError() << std::endl;
	//}

	if (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, overlayWindow, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		UpdateWindow(overlayWindow);
		WINDOWINFO info;
		ZeroMemory(&info, sizeof(info));
		info.cbSize = sizeof(info);
		GetWindowInfo(overlayWindow, &info);
		D2D1_SIZE_U siz;
		siz.height = ((info.rcClient.bottom) - (info.rcClient.top));
		siz.width = ((info.rcClient.right) - (info.rcClient.left));
		if (!IsIconic(overlayWindow)) {
			SetWindowPos(overlayWindow, NULL, info.rcClient.left, info.rcClient.top, siz.width, siz.height, SWP_SHOWWINDOW);
			target->Resize(&siz);
		}
		target->BeginDraw();
		target->Clear(D2D1::ColorF(0, 0, 0, 0));
		if (drawLoopCallback != NULL) {
			if (o_Foreground) {
				if (GetForegroundWindow() == overlayWindow)
					goto toDraw;
				else goto noDraw;
			}

		toDraw:
			time_t postTime = clock();
			time_t frameTime = postTime - preTime;
			preTime = postTime;

			if (/*o_DrawFPS*/1) {
				if (postTime - showTime > 100) {
					fps = 1000 / (float)frameTime;
					showTime = postTime;
				}
				DrawString(std::to_string(fps), 20, siz.width - 50, 0, 0, 1, 0, 1);
			}

			if (/*o_VSync*/1) {
				int pausetime = 17 - frameTime;
				if (pausetime > 0 && pausetime < 30) {
					Sleep(pausetime);
				}
			}

			mtx.lock();
			for (int i = 0; i < drawingListy.size(); i++) {
				solid_brush->SetColor(D2D1::ColorF(1, 0, 1, 1));
				target->DrawLine(drawingListy.at(i).point1, drawingListy.at(i).point2, solid_brush, drawingListy.at(i).thickness);
			}
			clear();
			mtx.unlock();

			drawLoopCallback(siz.width, siz.height);
		}
	noDraw:
		target->EndDraw();
		Sleep(1);
	}
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uiMessage)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uiMessage, wParam, lParam);
	}
	return 0;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);
	if (lpdwProcessId == GetCurrentProcessId())
	{
		enumWindow = hwnd;
		return FALSE;
	}
	return TRUE;
}

DWORD WINAPI OverlayThread(LPVOID lpParam)
{
	if (lpParam == NULL) {
		EnumWindows(EnumWindowsProc, NULL);
	}
	else {
		enumWindow = (HWND)lpParam;
	}
	d2oSetup(enumWindow);
	for (;;) {
		mainLoop();
	}
}

void DirectOverlaySetup(DirectOverlayCallback callback) {
	drawLoopCallback = callback;
	CreateThread(0, 0, OverlayThread, NULL, 0, NULL);
}

void DirectOverlaySetup(DirectOverlayCallback callback, HWND _targetWindow) {
	drawLoopCallback = callback;
	CreateThread(0, 0, OverlayThread, _targetWindow, 0, NULL);
}

void DirectOverlaySetOption(DWORD option) {
	if (option & D2DOV_REQUIRE_FOREGROUND) o_Foreground = true;
	if (option & D2DOV_DRAW_FPS) o_DrawFPS = true;
	if (option & D2DOV_VSYNC) o_VSync = true;
	if (option & D2DOV_FONT_ARIAL) fontname = L"arial";
	if (option & D2DOV_FONT_COURIER) fontname = L"Courier";
	if (option & D2DOV_FONT_CALIBRI) fontname = L"Calibri";
	if (option & D2DOV_FONT_GABRIOLA) fontname = L"Gabriola";
	if (option & D2DOV_FONT_IMPACT) fontname = L"Impact";
	fontname = L"Calibri";
}