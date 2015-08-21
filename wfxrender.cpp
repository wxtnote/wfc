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

#include "StdAfx.h"
#include "wfxwidget.h"
#include "wfxrender.h"

USING_NAMESPACE_WFX;

WfxRender::MemDC::MemDC( HWND hWnd, const Rect& rcPaint )
: m_hWnd(hWnd)
, m_rcPaint(rcPaint)
, m_hdc(NULL)
, m_hdcMem(NULL)
, m_hBitmap(NULL)
, m_hOldBitmap(NULL)
{
	m_hdc = ::GetDC(m_hWnd);
	if (m_hdc != NULL)
	{
		m_hdcMem = ::CreateCompatibleDC(m_hdc);
		Rect rcClient;
		::GetClientRect(m_hWnd, &rcClient);
		m_hBitmap = ::CreateCompatibleBitmap(m_hdc,
			rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
		if (m_hdcMem != NULL && m_hBitmap != NULL)
			m_hOldBitmap = ::SelectObject(m_hdcMem, m_hBitmap);
	}
}

WfxRender::MemDC::~MemDC()
{
	if (m_hdc != NULL && m_hdcMem != NULL
		&& m_hBitmap != NULL)
	{
		::BitBlt(m_hdc, m_rcPaint.left, m_rcPaint.top, 
			m_rcPaint.right - m_rcPaint.left, 
			m_rcPaint.bottom - m_rcPaint.top,
			m_hdcMem, m_rcPaint.left, m_rcPaint.top, SRCCOPY);
		::SelectObject(m_hdcMem, m_hOldBitmap);
		::DeleteObject(m_hBitmap);
		::DeleteDC(m_hdcMem);
		::ReleaseDC(m_hWnd, m_hdc);
	}
	
}

WfxRender::MemDC::operator HDC()
{
	return m_hdcMem;
}

void WfxRender::DrawButtton( HDC hdc, const String& strText, const Rect& rc, WORD wState, Dispatcher* pDispatch )
{
	COLORREF clrBk = WBTN_BKGND_STATIC;
	COLORREF clrFrm = WBTN_FRAME_STATIC;
	COLORREF clrText = WID_TEXT_STATIC;
	switch(wState)
	{
	case WID_STATE_STATIC:
		clrBk = WID_BKGND_STATIC;
		clrFrm = WBTN_FRAME_STATIC;
		clrText = WID_TEXT_STATIC;
		break;
	case WID_STATE_MOUSE:
		clrBk = WBTN_BKGND_MOUSE;
		clrFrm = WBTN_FRAME_MOUSE;
		clrText = WID_TEXT_MOUSE;
		break;
	case WID_STATE_PUSH:
		clrBk = WBTN_BKGND_PUSH;
		clrFrm = WBTN_FRAME_PUSH;
		clrText = WID_TEXT_PUSH;
		break;
	case WID_STATE_CHECKED:
		clrBk = WBTN_BKGND_CHECKED;
		clrFrm = WBTN_FRAME_CHECKED;
		clrText = WID_STATE_CHECKED;
		break;
	}
	DrawSolidRect(hdc, rc, clrBk, pDispatch);
	DrawFrame(hdc, rc, clrFrm, pDispatch);
	DrawText(hdc, rc, strText, clrText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}


void WfxRender::DrawRadioBox( HDC hdc, const Rect& rc, WORD wState, Dispatcher* pDispatch /*= NULL*/ )
{

}

void WfxRender::DrawWidget( HDC hdc, const String& strText, const Rect& rc, WORD wState , Dispatcher* pDispatch)
{
	DrawSolidRect(hdc, rc, WID_BKGND_STATIC, pDispatch);
	DrawText(hdc, rc, strText, RGB(255, 0, 0), DT_VCENTER | DT_SINGLELINE | DT_CENTER, NULL, pDispatch);
	DrawFrame(hdc, rc, WID_FRAME_STATIC, pDispatch);
}

void WfxRender::DrawSolidRect( HDC hdc, const Rect& rcPaint, COLORREF clr ,Dispatcher* pDispatch)
{
	WFX_CONDITION(::GetObjectType(hdc)==OBJ_DC || ::GetObjectType(hdc)==OBJ_MEMDC);
	::SetBkColor(hdc, clr);
	::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rcPaint, NULL, 0, NULL);
}

void WfxRender::DrawFrame( HDC hdc, const Rect& rcPaint, COLORREF clr, Dispatcher* pDispatch )
{
	HBRUSH brsh = ::CreateSolidBrush(clr);
	::FrameRect(hdc, &rcPaint, brsh);
	::DeleteObject(brsh);
}

void WfxRender::DrawText( HDC hdc, const Rect& rcPaint, const String& strText, COLORREF clr, DWORD dwFormat, HFONT hFont /*= NULL*/, Dispatcher* pDispatch /*= NULL*/ )
{
	WFX_CONDITION(::GetObjectType(hdc)==OBJ_DC || ::GetObjectType(hdc)==OBJ_MEMDC);
	::SetBkMode(hdc, TRANSPARENT);
	::SetTextColor(hdc, clr);
	HGDIOBJ hOldFont = NULL;
	if (hFont != NULL)
		hOldFont = ::SelectObject(hdc, hFont);
	else
		hOldFont = ::SelectObject(hdc, GetFontObject());
	String strTest(L"woqu");
	::DrawTextW(hdc, strText.c_str(), strText.size(), (LPRECT)&rcPaint, dwFormat);
	::SelectObject(hdc, hOldFont);
}

void WfxRender::DrawThumb( HDC hdc, const Rect& rc, WORD wState, Dispatcher* pDispatch /*= NULL*/ )
{
	DrawSolidRect(hdc, rc, WBTN_BKGND_MOUSE, pDispatch);
}

void WfxRender::DrawArror( HDC hdc, const Rect& rc, WORD wState, UINT nDirection, Dispatcher* pDispatch /*= NULL*/ )
{
	DrawSolidRect(hdc, rc, WBTN_BKGND_MOUSE, pDispatch);
}

void WfxRender::DrawComboDropDown( HDC hdc, const Rect& rc, WORD wState, Dispatcher* pDispatch /*= NULL*/ )
{
	WfxRender::DrawFrame(hdc, rc, WBTN_BKGND_MOUSE, pDispatch);
	std::vector<Point> rgPt;
	Point pt(rc.left + (float)rc.GetWidth() / 3, rc.top + (float)rc.GetHeight() / 3);
	rgPt.push_back(pt);
	pt.x = rc.left + (float)rc.GetWidth() / 2;
	pt.y = rc.top + 2 * (float)rc.GetHeight() / 3;
	rgPt.push_back(pt);
	pt.x = rc.left + 2 * (float)rc.GetWidth() / 3;
	pt.y = rc.top + (float)rc.GetHeight() / 3;
	rgPt.push_back(pt);
	rgPt.push_back(rgPt[0]);
	WfxRender::DrawLines(hdc, rgPt, WBTN_BKGND_MOUSE);
}

void WfxRender::GenerateClip( HDC hDC, const Rect& rcItem, RenderClip& clip )
{
	Rect rcClip;
	::GetClipBox(hDC, &rcClip);
	clip.m_hOldRgn = ::CreateRectRgnIndirect(&rcClip);
	clip.m_hRgn = ::CreateRectRgnIndirect(&rcItem);
	::ExtSelectClipRgn(hDC, clip.m_hRgn, RGN_AND);
	clip.m_hDC = hDC;
	clip.m_rcItem = rcItem;
}

void WfxRender::DrawHeadCell( HDC hdc, const Rect& rcPaint, DWORD dwState, const String& strText, COLORREF clrText, DWORD dwFormat )
{
	COLORREF clrBk = WCELL_BKGRND;
	if (dwState & WCS_SELECTED)
		clrBk = WCELL_SELECTED;
	String strDraw = strText;
	if (dwState & WCS_ASORT)
	{
		strDraw += L"^";
		clrText = RGB(0, 255, 0);
	}
	else if (dwState & WCS_NASORT)
	{
		strDraw += L"v";
		clrText = RGB(255, 0, 0);
	}
	WfxRender::DrawSolidRect(hdc, rcPaint, clrBk, NULL);
	WfxRender::DrawText(hdc, rcPaint, strDraw, clrText, dwFormat);
	if (dwState & WCS_MOUSEMOVE)
	{
		String strLine;
		strLine.assign(strDraw.size(), L'_');
		WfxRender::DrawText(hdc, rcPaint, strLine, clrText, dwFormat);
	}
	if (dwState & WCS_MOUSEMOVE)
	{
		WfxRender::DrawFrame(hdc, rcPaint, WBTN_BKGND_MOUSE, NULL);
	}
	else
	{
		WfxRender::DrawFrame(hdc, rcPaint, WBTN_BKGND_MOUSE, NULL);
	}
}

Size WfxRender::EstimateWidgetSize( const Rect& rc, const String& strText, WORD wState, Dispatcher* pDispatch /*= NULL*/ )
{
	Size sz;
	sz.cx = rc.right - rc.left;
	sz.cy = rc.bottom - rc.top;
	if (pDispatch != NULL)
	{
		HWND hWnd = pDispatch->GetHwnd();
		if (hWnd != NULL && ::IsWindow(hWnd))
		{
			HDC hdc = ::GetDC(hWnd);
			if (hdc != NULL)
			{
				::GetTextExtentPoint32(hdc, strText.c_str(), strText.size(), &sz);
			}
			::ReleaseDC(hWnd, hdc);
		}
	}
	return sz;
}

void WfxRender::DrawLinkCell( HDC hdc, const Rect& rcPaint, DWORD dwState, const String& strText, COLORREF clrText, DWORD dwFormat )
{
	COLORREF clrBk = WCELL_BKGRND;
	if (dwState & WCS_SELECTED)
		clrBk = WCELL_SELECTED;
	String strDraw = strText;
	if (dwState & WCS_ASORT)
	{
		strDraw += L"^";
		clrText = RGB(0, 255, 0);
	}
	else if (dwState & WCS_NASORT)
	{
		strDraw += L"v";
		clrText = RGB(255, 0, 0);
	}
	WfxRender::DrawSolidRect(hdc, rcPaint, clrBk, NULL);
	WfxRender::DrawText(hdc, rcPaint, strDraw, clrText, dwFormat | DT_NOPREFIX);
	String strLine;
	strLine.assign(strDraw.size(), L'_');
	WfxRender::DrawText(hdc, rcPaint, strLine, clrText, dwFormat);
	WfxRender::DrawFrame(hdc, rcPaint, WCELL_FRAME, NULL);
}

void WfxRender::DrawLayerCell( HDC hdc, const Rect& rcPaint, DWORD dwState, const String& strText, COLORREF clrText, DWORD dwFormat )
{
	String str(L"+");

	if (dwState & WCS_EXPAND)
	{
		str = L"-";
	}
	
	WfxRender::DrawText(hdc, rcPaint, str, WBTN_BKGND_MOUSE, dwFormat);
	WfxRender::DrawFrame(hdc, rcPaint, WCELL_FRAME, NULL);
}

HFONT WfxRender::GetFontObject()
{
	if (s_hFont == NULL)
	{
		LOGFONTW lf = {0};
		wsprintfW(lf.lfFaceName, L"%s", WID_FONT_STATIC);
		lf.lfWeight = FW_NORMAL;
		lf.lfHeight = 12;
		lf.lfCharSet = DEFAULT_CHARSET;
		s_hFont = ::CreateFontIndirectW(&lf);
	}
	return s_hFont;
}

void WfxRender::DrawTextBox( HDC hdc, const String& strText, const Rect& rc, WORD wState, WORD wMode, Dispatcher* pDispatch /*= NULL*/ )
{
	COLORREF clrBk = WTXB_BKGND_STATIC;
	COLORREF clrFrm = WBTN_FRAME_STATIC;
	COLORREF clrText = WID_TEXT_STATIC;
	switch(wState)
	{
	case WID_STATE_STATIC:
		clrBk = WTXB_BKGND_STATIC;
		clrFrm = WBTN_FRAME_STATIC;
		clrText = WID_TEXT_STATIC;
		break;
	case WID_STATE_MOUSE:
		clrBk = WTXB_BKGND_MOUSE;
		clrFrm = WBTN_FRAME_MOUSE;
		clrText = WID_TEXT_MOUSE;
		break;
	case WID_STATE_PUSH:
		clrBk = WTXB_BKGND_PUSH;
		clrFrm = WBTN_FRAME_PUSH;
		clrText = WID_TEXT_PUSH;
		break;
	case WID_STATE_CHECKED:
		clrBk = WTXB_BKGND_CHECKED;
		clrFrm = WBTN_FRAME_CHECKED;
		clrText = WID_STATE_CHECKED;
		break;
	}
	DrawSolidRect(hdc, rc, clrBk, pDispatch);
	DrawFrame(hdc, rc, clrFrm, pDispatch);
	Rect rcText = rc;
	::InflateRect(&rcText, -1, -1);
	DrawText(hdc, rcText, strText, clrText, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
}

void WfxRender::DrawCheckBoxItem( HDC hdc, const Rect& rc, WORD wState, BOOL bChecked, Dispatcher* pDispatch /*= NULL*/ )
{
	COLORREF clrBk = WBTN_BKGND_STATIC;
	COLORREF clrFrm = WBTN_FRAME_STATIC;
	COLORREF clrText = WID_TEXT_STATIC;
	switch(wState)
	{
	case WID_STATE_STATIC:
		clrFrm = WBTN_FRAME_STATIC;
		clrText = WID_TEXT_STATIC;
		break;
	case WID_STATE_MOUSE:
		clrFrm = WBTN_FRAME_MOUSE;
		clrText = WID_TEXT_MOUSE;
		break;
	case WID_STATE_PUSH:
		clrFrm = WBTN_FRAME_PUSH;
		clrText = WID_TEXT_PUSH;
		break;
	case WID_STATE_CHECKED:
		clrFrm = WBTN_FRAME_CHECKED;
		clrText = WID_STATE_CHECKED;
		break;
	}
	DrawSolidRect(hdc, rc, clrBk, pDispatch);
	if (bChecked)
	{
		Rect rcMark = rc;
		rcMark.left += 2;
		rcMark.right -= 2;
		rcMark.top += 2;
		rcMark.bottom -= 2;
		HPEN hPen = ::CreatePen(PS_SOLID, 2, clrFrm);
		HGDIOBJ hOldPen = ::SelectObject(hdc, hPen);
		int x = rcMark.left;
		int y = rcMark.top + ((float)((rcMark.bottom - rcMark.top) * 2) / (float)3);
		::MoveToEx(hdc, x, y, NULL);
		x = rcMark.left + ((float)(rcMark.right - rcMark.left) / (float)3);
		y = rcMark.bottom;
		::LineTo(hdc, x, y);
		x = rcMark.right;
		y = rcMark.top;
		::LineTo(hdc, x, y);
		::SelectObject(hdc, hOldPen);
		::DeleteObject(hPen);
	}
	DrawFrame(hdc, rc, clrFrm, pDispatch);
}

void WfxRender::DrawCheckBox( HDC hdc, const String& strText, const Rect& rc, WORD wState, ULONG nTextOffset, Dispatcher* pDispatch /*= NULL*/ )
{
	COLORREF clrBk = WBTN_BKGND_STATIC;
	COLORREF clrFrm = WBTN_FRAME_STATIC;
	COLORREF clrText = WID_TEXT_STATIC;
	switch(wState)
	{
	case WID_STATE_STATIC:
		clrBk = WID_BKGND_STATIC;
		clrFrm = WBTN_FRAME_STATIC;
		clrText = WID_TEXT_STATIC;
		break;
	case WID_STATE_MOUSE:
		clrBk = WBTN_BKGND_MOUSE;
		clrFrm = WBTN_FRAME_MOUSE;
		clrText = WID_TEXT_MOUSE;
		break;
	case WID_STATE_PUSH:
		clrBk = WBTN_BKGND_PUSH;
		clrFrm = WBTN_FRAME_PUSH;
		clrText = WID_TEXT_PUSH;
		break;
	case WID_STATE_CHECKED:
		clrBk = WBTN_BKGND_CHECKED;
		clrFrm = WBTN_FRAME_CHECKED;
		clrText = WID_STATE_CHECKED;
		break;
	}
	DrawSolidRect(hdc, rc, clrBk, pDispatch);
	DrawFrame(hdc, rc, clrFrm, pDispatch);
	Rect rcText = rc;
	rcText.left += nTextOffset;
	DrawText(hdc, rcText, strText, clrText, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
}

void WfxRender::DrawRadioBoxItem( HDC hdc, const Rect& rc, WORD wState, BOOL bChecked, Dispatcher* pDispatch /*= NULL*/ )
{
	COLORREF clrBk = WBTN_BKGND_STATIC;
	COLORREF clrFrm = WBTN_FRAME_STATIC;
	COLORREF clrText = WID_TEXT_STATIC;
	switch(wState)
	{
	case WID_STATE_STATIC:
		clrFrm = WBTN_FRAME_STATIC;
		clrText = WID_TEXT_STATIC;
		break;
	case WID_STATE_MOUSE:
		clrFrm = WBTN_FRAME_MOUSE;
		clrText = WID_TEXT_MOUSE;
		break;
	case WID_STATE_PUSH:
		clrFrm = WBTN_FRAME_PUSH;
		clrText = WID_TEXT_PUSH;
		break;
	case WID_STATE_CHECKED:
		clrFrm = WBTN_FRAME_CHECKED;
		clrText = WID_STATE_CHECKED;
		break;
	}
	DrawSolidRect(hdc, rc, clrBk, pDispatch);
	if (bChecked)
	{
		Rect rcMark = rc;
		rcMark.left += 2;
		rcMark.right -= 2;
		rcMark.top += 2;
		rcMark.bottom -= 2;
		DrawSolidRect(hdc, rcMark, clrFrm, pDispatch);
	}
	DrawFrame(hdc, rc, clrFrm, pDispatch);
}

void WfxRender::DrawProcessBar( HDC hdc, const Rect& rc, WORD wState, ULONG nMax, ULONG nPos, Dispatcher* pDispatch /*= NULL*/ )
{
	COLORREF clrBk = WBTN_BKGND_STATIC;
	COLORREF clrFrm = WBTN_FRAME_STATIC;
	COLORREF clrText = WID_TEXT_STATIC;
	switch(wState)
	{
	case WID_STATE_STATIC:
		clrFrm = WBTN_FRAME_STATIC;
		clrText = WID_TEXT_STATIC;
		break;
	case WID_STATE_MOUSE:
		clrFrm = WBTN_FRAME_MOUSE;
		clrText = WID_TEXT_MOUSE;
		break;
	case WID_STATE_PUSH:
		clrFrm = WBTN_FRAME_PUSH;
		clrText = WID_TEXT_PUSH;
		break;
	case WID_STATE_CHECKED:
		clrFrm = WBTN_FRAME_CHECKED;
		clrText = WID_STATE_CHECKED;
		break;
	}
	String strText;
	strText.Format(L"%d/%d", nMax, nMax);
	HGDIOBJ hOldFont = ::SelectObject(hdc, GetFontObject());
	Size sz;
	::GetTextExtentPoint32(hdc, strText.c_str(), strText.size(), &sz);
	Rect rcDraw = rc;
	rcDraw.right -= sz.cx + 2;
	rcDraw.top += 2;
	rcDraw.left += 2;
	rcDraw.bottom -= 2;
	Rect rcPercent = rcDraw;
	Rect rcText = rc;
	rcText.left = rcDraw.right;
	rcText.right -= 2;
	DrawFrame(hdc, rcDraw, clrFrm);
	DrawFrame(hdc, rc, clrFrm);
	if (nMax > 0)
	{
		ULONG nBarLength = (rcDraw.right - rcDraw.left) * (float)nPos / (float)nMax;
		rcDraw.right = rcDraw.left + nBarLength;
		DrawSolidRect(hdc, rcDraw, WBTN_BKGND_MOUSE, pDispatch);
		strText.Format(L"%.2f%%", (float)(100*(float)nPos)/(float)nMax);
		DrawText(hdc, rcPercent, strText, clrText, DT_VCENTER | DT_CENTER | DT_SINGLELINE, GetFontObject(), pDispatch);

	}
	strText.Format(L"%d/%d", nPos, nMax);
	DrawText(hdc, rcText, strText, clrText, DT_VCENTER | DT_RIGHT | DT_SINGLELINE, GetFontObject(), pDispatch);
	::SelectObject(hdc, hOldFont);
}

void WfxRender::DrawLine( HDC hdc, const Point& ptStart, const Point& ptEnd, COLORREF clr, int nPixel /*= 1*/ )
{
	::MoveToEx(hdc, ptStart.x, ptStart.y, NULL);
	HPEN hPen = ::CreatePen(PS_SOLID, nPixel, clr);
	HGDIOBJ hGdiObj = ::SelectObject(hdc, hPen);
	::LineTo(hdc, ptEnd.x, ptEnd.y);
	::SelectObject(hdc, hGdiObj);
	::DeleteObject(hPen);
}

void WfxRender::DrawLines( HDC hdc, const std::vector<Point>& rgPt, COLORREF clr, int nPixel /*= 1*/ )
{
	if (rgPt.size() == 0)
	{
		return;
	}
	HPEN hPen = ::CreatePen(PS_SOLID, nPixel, clr);
	HGDIOBJ hGdiObj = ::SelectObject(hdc, hPen);
	::MoveToEx(hdc, rgPt[0].x, rgPt[0].y, NULL);
	for (ULONG i = 1; i < rgPt.size(); i++)
	{
		::LineTo(hdc, rgPt[i].x, rgPt[i].y);
	}
	::SelectObject(hdc, hGdiObj);
	::DeleteObject(hPen);
}

void WfxRender::DrawImage(HDC hdc, const PImage& pImage, const Rect& rc, DWORD dwFomat /*= DT_CENTER*/ )
{
	LONG nImgWidth = pImage->GetWidth();
	LONG nImgHeight = pImage->GetHeight();
	Gdiplus::RectF rcF;
	rcF.Y = rc.top + (rc.GetHeight() - nImgHeight) / 2;
	switch(dwFomat)
	{
	case DT_LEFT:
		rcF.X = rc.left;
		break;
	case DT_RIGHT:
		rcF.X = rc.right - nImgWidth;
		break;
	case DT_CENTER:
		rcF.X = rc.left + (rc.GetWidth() - nImgWidth) / 2;
	default:
		;
	}
	rcF.Width = rc.GetWidth();
	rcF.Height = rc.GetHeight();
	Gdiplus::Graphics gs(hdc);
	gs.DrawImage(pImage.get(), rcF);
}

HFONT WfxRender::s_hFont = NULL;

WfxRender::RenderClip::~RenderClip()
{
	WFX_CONDITION(::GetObjectType(m_hDC)==OBJ_DC || ::GetObjectType(m_hDC)==OBJ_MEMDC);
	WFX_CONDITION(::GetObjectType(m_hRgn)==OBJ_REGION);
	WFX_CONDITION(::GetObjectType(m_hOldRgn)==OBJ_REGION);
	::SelectClipRgn(m_hDC, m_hOldRgn);
	::DeleteObject(m_hOldRgn);
	::DeleteObject(m_hRgn);
}

WfxRender::RenderClip::RenderClip(HDC hdc, const Rect& rcItem)
: m_hDC(hdc)
{
	Rect rcClip;
	::GetClipBox(m_hDC, &rcClip);
	m_hOldRgn = ::CreateRectRgnIndirect(&rcClip);
	m_hRgn = ::CreateRectRgnIndirect(&rcItem);
	::ExtSelectClipRgn(m_hDC, m_hRgn, RGN_AND);
	m_rcItem = rcItem;
}

WfxRender::GdiPlusHelper::GdiPlusHelper()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_nGdiPlusToken, &gdiplusStartupInput, NULL);
}

WfxRender::GdiPlusHelper::~GdiPlusHelper()
{
	Gdiplus::GdiplusShutdown(m_nGdiPlusToken);
}

WfxRender::GdiPlusHelper gdiplus_init;
