// This is a part of the Widget Foundation Classes.
// Copyright (C) Hirota Studio (www.hirotastudio.com)
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Widget Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Widget Foundation Classes product.
//
#pragma once

#include "wfx.h"

BEGIN_NAMESPACE_WFX

class WFX_API WfxRender
{
public:
	class MemDC
	{
	public:
		MemDC(HWND hWnd, const Rect& rcPaint);
		~MemDC();
	public:
		operator HDC();
	private:
		HDC m_hdc;
		HDC m_hdcMem;
		Rect m_rcPaint;
		HBITMAP m_hBitmap;
		HGDIOBJ m_hOldBitmap;
		HWND m_hWnd;
	};

	class RenderClip
	{
	public:
		RenderClip(HDC hdc, const Rect& rcItem);
		~RenderClip();
		Rect m_rcItem;
		HDC m_hDC;
		HRGN m_hRgn;
		HRGN m_hOldRgn;
	};
	class GdiPlusHelper
	{
	public:
		GdiPlusHelper();
		~GdiPlusHelper();
		ULONG_PTR m_nGdiPlusToken;
	};
public:

	static void drawWidget(HDC hdc, const String& strText, const Rect& rc, WORD wState, Dispatcher* pDispatch = NULL);
	static void drawButtton(HDC hdc, const String& strText, const Rect& rc, WORD wState, Dispatcher* pDispatch = NULL);
	static void drawTextBox(HDC hdc, const String& strText, const Rect& rc, WORD wState, WORD wMode, Dispatcher* pDispatch = NULL);
	static void drawCheckBoxItem(HDC hdc, const Rect& rc, WORD wState, BOOL bChecked, Dispatcher* pDispatch = NULL);
	static void drawCheckBox(HDC hdc, const String& strText, const Rect& rc, WORD wState, ULONG nTextOffset, Dispatcher* pDispatch = NULL);
	static void drawRadioBoxItem(HDC hdc, const Rect& rc, WORD wState, BOOL bChecked, Dispatcher* pDispatch = NULL);
	static void drawRadioBox(HDC hdc, const Rect& rc, WORD wState, Dispatcher* pDispatch = NULL);
	static void drawProcessBar(HDC hdc, const Rect& rc, WORD wState, ULONG nMax, ULONG nPos, Dispatcher* pDispatch = NULL);
	static void drawSolidRect(HDC hdc, const Rect& rcPaint, COLORREF clr, Dispatcher* pDispatch = NULL);
	static void drawFrame(HDC hdc, const Rect& rcPaint, COLORREF clr, Dispatcher* pDispatch = NULL);
	static void drawText(HDC hdc, const Rect& rcPaint, const String& strText, COLORREF clr, DWORD dwFormat, HFONT hFont = NULL, Dispatcher* pDispatch = NULL);
	static void drawThumb(HDC hdc, const Rect& rc, WORD wState, Dispatcher* pDispatch = NULL);
	static void drawArror(HDC hdc, const Rect& rc, WORD wState, UINT nDirection, Dispatcher* pDispatch = NULL);
	static void drawComboDropDown(HDC hdc, const Rect& rc, WORD wState, Dispatcher* pDispatch = NULL);
	static void generateClip(HDC hdc, const Rect& rcItem, RenderClip& clip);
	static void drawHeadCell(HDC hdc, const Rect& rcPaint,  DWORD dwState,
		const String& strText, COLORREF clrText, DWORD dwFormat);
	static void drawLinkCell(HDC hdc, const Rect& rcPaint, DWORD dwState,
		const String& strText, COLORREF clrText, DWORD dwFormat);
	static void drawLayerCell(HDC hdc, const Rect& rcPaint, DWORD dwState,
		const String& strText, COLORREF clrText, DWORD dwFormat);
	static void drawLine(HDC hdc, const Point& ptStart, const Point& ptEnd, COLORREF clr, int nPixel = 1);
	static void drawLines(HDC hdc, const std::vector<Point>& rgPt, COLORREF clr, int nPixel = 1);
	static Size estimateWidgetSize(const Rect& rc, const String& strText, WORD wState, Dispatcher* pDispatch = NULL);
	static HFONT getFontObject();
	static void drawImage(HDC hdc, const SPImage& pImage, const Rect& rc, DWORD dwFomat = DT_CENTER);
	static HFONT createFontIndirect(LOGFONTW& logfont);
protected:
	static HFONT s_hFont;
private:
	// [Frame]
	COLORREF s_clrFrameBkgndStatic;
	// [BUTTON]
	COLORREF s_clrBtnBkgndStatic;
	COLORREF s_clrBtnBkgndMouse;
	COLORREF s_clrBtnBkgndPush;
	COLORREF s_clrBtnBkgndChecked;
	COLORREF s_clrBtnTextStatic;

};

extern WfxRender::GdiPlusHelper gdiplus_init;

END_NAMESPACE_WFX
