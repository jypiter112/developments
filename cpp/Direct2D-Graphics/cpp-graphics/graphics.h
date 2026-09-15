#pragma once
#include <Windows.h>
#include <d2d1.h>
#include <cmath>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxguid.lib")

#define GRAPHICS_FAILED -1

/*
DRAWING EXAMPLE:

BeginDraw()
ClearScreen()

--Calculate drawing

EndDraw()

*/

namespace uGraphics {
	ID2D1Factory* pfactory = NULL;
	ID2D1HwndRenderTarget* prendertarget;
	HRESULT res = NULL;
	RECT rc;
	ID2D1SolidColorBrush* pblackbrush = NULL;

	void ClearScreen() {
		prendertarget->Clear(D2D1::ColorF(255, 255, 255));
	}
	int Init(HWND hwnd) {
		res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pfactory);
		if (res != S_OK) return -1;

		GetClientRect(hwnd, &rc);

		pfactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
			hwnd, D2D1::SizeU(rc.right-rc.left, rc.bottom-rc.top)),
			&prendertarget
		);
			
		return 0;
	}
	int CreateBrush() {
		prendertarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Red),
			&pblackbrush
		);
		return 0;
	}
	HRESULT DrawRect() {
		prendertarget->BeginDraw();
		ClearScreen();
		prendertarget->DrawRectangle(
			D2D1::RectF(
				rc.left + 20.0f,
				rc.top + 20.0f,
				rc.right - 100.0f,
				rc.bottom - 100.0f),
				pblackbrush);
		return prendertarget->EndDraw();
	}
	HRESULT DrawPath() {
		prendertarget->BeginDraw();
		ClearScreen();
		float y = 0;
		D2D1_POINT_2F prevpoint = D2D1::Point2F(
			(rc.right/2),
			rc.top
		);
		for (int x = 0; y < rc.bottom; x++) {
			y = 0.1 * (x * x);
			D2D1_POINT_2F newpoint = D2D1::Point2F(
				(rc.right / 2) + x + 1,
				rc.top + y + 1);
			prendertarget->DrawLine(
				prevpoint,
				newpoint,
				pblackbrush
			);
			prevpoint = newpoint;
		}
		return prendertarget->EndDraw();
	}
	// release all resources before quitting
	void Exit() {
		if (pblackbrush) pblackbrush->Release();
		if (prendertarget) prendertarget->Release();
		if (pfactory) pfactory->Release();
	}
};