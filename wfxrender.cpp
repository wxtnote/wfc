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

void WfxRender::drawButtton( HDC hdc, const String& strText, const Rect& rc, WORD wState, Dispatcher* pDispatch )
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
	drawSolidRect(hdc, rc, clrBk, pDispatch);
	drawFrame(hdc, rc, clrFrm, pDispatch);
	drawText(hdc, rc, strText, clrText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}


void WfxRender::drawRadioBox( HDC hdc, const Rect& rc, WORD wState, Dispatcher* pDispatch /*= NULL*/ )
{

}

void WfxRender::drawWidget( HDC hdc, const String& strText, const Rect& rc, WORD wState , Dispatcher* pDispatch)
{
	drawSolidRect(hdc, rc, WID_BKGND_STATIC, pDispatch);
	drawText(hdc, rc, strText, RGB(255, 0, 0), DT_VCENTER | DT_SINGLELINE | DT_CENTER, NULL, pDispatch);
	drawFrame(hdc, rc, WID_FRAME_STATIC, pDispatch);
}

void WfxRender::drawSolidRect( HDC hdc, const Rect& rcPaint, COLORREF clr ,Dispatcher* pDispatch)
{
	WFX_CONDITION(::GetObjectType(hdc)==OBJ_DC || ::GetObjectType(hdc)==OBJ_MEMDC);
	::SetBkColor(hdc, clr);
	::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rcPaint, NULL, 0, NULL);
}

void WfxRender::drawFrame( HDC hdc, const Rect& rcPaint, COLORREF clr, Dispatcher* pDispatch )
{
	HBRUSH brsh = ::CreateSolidBrush(clr);
	::FrameRect(hdc, &rcPaint, brsh);
	::DeleteObject(brsh);
}

void WfxRender::drawText( HDC hdc, const Rect& rcPaint, const String& strText, COLORREF clr, DWORD dwFormat, HFONT hFont /*= NULL*/, Dispatcher* pDispatch /*= NULL*/ )
{
	WFX_CONDITION(::GetObjectType(hdc)==OBJ_DC || ::GetObjectType(hdc)==OBJ_MEMDC);
	::SetBkMode(hdc, TRANSPARENT);
	::SetTextColor(hdc, clr);
	HGDIOBJ hOldFont = NULL;
	if (hFont != NULL)
		hOldFont = ::SelectObject(hdc, hFont);
	else
		hOldFont = ::SelectObject(hdc, getFontObject());
	String strTest(L"woqu");
	::DrawTextW(hdc, strText.c_str(), strText.size(), (LPRECT)&rcPaint, dwFormat);
	::SelectObject(hdc, hOldFont);
}

void WfxRender::drawThumb( HDC hdc, const Rect& rc, WORD wState, Dispatcher* pDispatch /*= NULL*/ )
{
	drawSolidRect(hdc, rc, WBTN_BKGND_MOUSE, pDispatch);
}

void WfxRender::drawArror( HDC hdc, const Rect& rc, WORD wState, UINT nDirection, Dispatcher* pDispatch /*= NULL*/ )
{
	drawSolidRect(hdc, rc, WBTN_BKGND_MOUSE, pDispatch);
}

void WfxRender::drawComboDropDown( HDC hdc, const Rect& rc, WORD wState, Dispatcher* pDispatch /*= NULL*/ )
{
	WfxRender::drawFrame(hdc, rc, WBTN_BKGND_MOUSE, pDispatch);
	std::vector<Point> rgPt;
	Point pt(rc.left + (float)rc.getWidth() / 3, rc.top + (float)rc.getHeight() / 3);
	rgPt.push_back(pt);
	pt.x = rc.left + (float)rc.getWidth() / 2;
	pt.y = rc.top + 2 * (float)rc.getHeight() / 3;
	rgPt.push_back(pt);
	pt.x = rc.left + 2 * (float)rc.getWidth() / 3;
	pt.y = rc.top + (float)rc.getHeight() / 3;
	rgPt.push_back(pt);
	rgPt.push_back(rgPt[0]);
	WfxRender::drawLines(hdc, rgPt, WBTN_BKGND_MOUSE);
}

void WfxRender::generateClip( HDC hDC, const Rect& rcItem, RenderClip& clip )
{
	Rect rcClip;
	::GetClipBox(hDC, &rcClip);
	clip.m_hOldRgn = ::CreateRectRgnIndirect(&rcClip);
	clip.m_hRgn = ::CreateRectRgnIndirect(&rcItem);
	::ExtSelectClipRgn(hDC, clip.m_hRgn, RGN_AND);
	clip.m_hDC = hDC;
	clip.m_rcItem = rcItem;
}

void WfxRender::drawHeadCell( HDC hdc, const Rect& rcPaint, DWORD dwState, const String& strText, COLORREF clrText, DWORD dwFormat )
{
	COLORREF clrBk = WCELL_BKGRND;
	if (dwState & WCS_SELECTED)
		clrBk = WCELL_SELECTED;
	String strdraw = strText;
	if (dwState & WCS_ASORT)
	{
		strdraw += L"^";
		clrText = RGB(0, 255, 0);
	}
	else if (dwState & WCS_NASORT)
	{
		strdraw += L"v";
		clrText = RGB(255, 0, 0);
	}
	WfxRender::drawSolidRect(hdc, rcPaint, clrBk, NULL);
	WfxRender::drawText(hdc, rcPaint, strdraw, clrText, dwFormat);
	if (dwState & WCS_MOUSEMOVE)
	{
		String strLine;
		strLine.assign(strdraw.size(), L'_');
		WfxRender::drawText(hdc, rcPaint, strLine, clrText, dwFormat);
	}
	if (dwState & WCS_MOUSEMOVE)
	{
		WfxRender::drawFrame(hdc, rcPaint, WBTN_BKGND_MOUSE, NULL);
	}
	else
	{
		WfxRender::drawFrame(hdc, rcPaint, WBTN_BKGND_MOUSE, NULL);
	}
}

Size WfxRender::estimateWidgetSize( const Rect& rc, const String& strText, WORD wState, Dispatcher* pDispatch /*= NULL*/ )
{
	Size sz;
	sz.cx = rc.right - rc.left;
	sz.cy = rc.bottom - rc.top;
	if (pDispatch != NULL)
	{
		HWND hWnd = pDispatch->getHwnd();
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

void WfxRender::drawLinkCell( HDC hdc, const Rect& rcPaint, DWORD dwState, const String& strText, COLORREF clrText, DWORD dwFormat )
{
	COLORREF clrBk = WCELL_BKGRND;
	if (dwState & WCS_SELECTED)
		clrBk = WCELL_SELECTED;
	String strdraw = strText;
	if (dwState & WCS_ASORT)
	{
		strdraw += L"^";
		clrText = RGB(0, 255, 0);
	}
	else if (dwState & WCS_NASORT)
	{
		strdraw += L"v";
		clrText = RGB(255, 0, 0);
	}
	WfxRender::drawSolidRect(hdc, rcPaint, clrBk, NULL);
	WfxRender::drawText(hdc, rcPaint, strdraw, clrText, dwFormat | DT_NOPREFIX);
	String strLine;
	strLine.assign(strdraw.size(), L'_');
	WfxRender::drawText(hdc, rcPaint, strLine, clrText, dwFormat);
	WfxRender::drawFrame(hdc, rcPaint, WCELL_FRAME, NULL);
}

void WfxRender::drawLayerCell( HDC hdc, const Rect& rcPaint, DWORD dwState, const String& strText, COLORREF clrText, DWORD dwFormat )
{
	String str(L"+");

	if (dwState & WCS_EXPAND)
	{
		str = L"-";
	}
	
	WfxRender::drawText(hdc, rcPaint, str, WBTN_BKGND_MOUSE, dwFormat);
	WfxRender::drawFrame(hdc, rcPaint, WCELL_FRAME, NULL);
}

HFONT WfxRender::getFontObject()
{
	if (s_hFont == NULL)
	{
		LOGFONTW lf = {0};
		wsprintfW(lf.lfFaceName, L"%s", WID_FONT_STATIC);
		lf.lfWeight = FW_NORMAL;
		lf.lfHeight = WID_FSIZE_STATIC;
		lf.lfCharSet = DEFAULT_CHARSET;
		s_hFont = ::CreateFontIndirectW(&lf);
	}
	return s_hFont;
}

void WfxRender::drawTextBox( HDC hdc, const String& strText, const Rect& rc, WORD wState, WORD wMode, Dispatcher* pDispatch /*= NULL*/ )
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
	drawSolidRect(hdc, rc, clrBk, pDispatch);
	drawFrame(hdc, rc, clrFrm, pDispatch);
	Rect rcText = rc;
	::InflateRect(&rcText, -1, -1);
	drawText(hdc, rcText, strText, clrText, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
}

void WfxRender::drawCheckBoxItem( HDC hdc, const Rect& rc, WORD wState, BOOL bChecked, Dispatcher* pDispatch /*= NULL*/ )
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
	drawSolidRect(hdc, rc, clrBk, pDispatch);
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
	drawFrame(hdc, rc, clrFrm, pDispatch);
}

void WfxRender::drawCheckBox( HDC hdc, const String& strText, const Rect& rc, WORD wState, ULONG nTextOffset, Dispatcher* pDispatch /*= NULL*/ )
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
	drawSolidRect(hdc, rc, clrBk, pDispatch);
	drawFrame(hdc, rc, clrFrm, pDispatch);
	Rect rcText = rc;
	rcText.left += nTextOffset;
	drawText(hdc, rcText, strText, clrText, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
}

void WfxRender::drawRadioBoxItem( HDC hdc, const Rect& rc, WORD wState, BOOL bChecked, Dispatcher* pDispatch /*= NULL*/ )
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
	drawSolidRect(hdc, rc, clrBk, pDispatch);
	if (bChecked)
	{
		Rect rcMark = rc;
		rcMark.left += 2;
		rcMark.right -= 2;
		rcMark.top += 2;
		rcMark.bottom -= 2;
		drawSolidRect(hdc, rcMark, clrFrm, pDispatch);
	}
	drawFrame(hdc, rc, clrFrm, pDispatch);
}

void WfxRender::drawProcessBar( HDC hdc, const Rect& rc, WORD wState, ULONG nMax, ULONG nPos, Dispatcher* pDispatch /*= NULL*/ )
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
	strText.format(L"%d/%d", nMax, nMax);
	HGDIOBJ hOldFont = ::SelectObject(hdc, getFontObject());
	Size sz;
	::GetTextExtentPoint32(hdc, strText.c_str(), strText.size(), &sz);
	Rect rcdraw = rc;
	rcdraw.right -= sz.cx + 2;
	rcdraw.top += 2;
	rcdraw.left += 2;
	rcdraw.bottom -= 2;
	Rect rcPercent = rcdraw;
	Rect rcText = rc;
	rcText.left = rcdraw.right;
	rcText.right -= 2;
	drawFrame(hdc, rcdraw, clrFrm);
	drawFrame(hdc, rc, clrFrm);
	if (nMax > 0)
	{
		ULONG nBarLength = (rcdraw.right - rcdraw.left) * (float)nPos / (float)nMax;
		rcdraw.right = rcdraw.left + nBarLength;
		drawSolidRect(hdc, rcdraw, WBTN_BKGND_MOUSE, pDispatch);
		strText.format(L"%.2f%%", (float)(100*(float)nPos)/(float)nMax);
		drawText(hdc, rcPercent, strText, clrText, DT_VCENTER | DT_CENTER | DT_SINGLELINE, getFontObject(), pDispatch);

	}
	strText.format(L"%d/%d", nPos, nMax);
	drawText(hdc, rcText, strText, clrText, DT_VCENTER | DT_RIGHT | DT_SINGLELINE, getFontObject(), pDispatch);
	::SelectObject(hdc, hOldFont);
}

void WfxRender::drawLine( HDC hdc, const Point& ptStart, const Point& ptEnd, COLORREF clr, int nPixel /*= 1*/ )
{
	::MoveToEx(hdc, ptStart.x, ptStart.y, NULL);
	HPEN hPen = ::CreatePen(PS_SOLID, nPixel, clr);
	HGDIOBJ hGdiObj = ::SelectObject(hdc, hPen);
	::LineTo(hdc, ptEnd.x, ptEnd.y);
	::SelectObject(hdc, hGdiObj);
	::DeleteObject(hPen);
}

void WfxRender::drawLines( HDC hdc, const std::vector<Point>& rgPt, COLORREF clr, int nPixel /*= 1*/ )
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

void WfxRender::drawImage(HDC hdc, const SPImage& pImage, const Rect& rc, DWORD dwFomat /*= DT_CENTER*/ )
{
	LONG nImgWidth = pImage->GetWidth();
	LONG nImgHeight = pImage->GetHeight();
	Gdiplus::RectF rcF;
	rcF.Y = rc.top + (rc.getHeight() - nImgHeight) / 2;
	switch(dwFomat)
	{
	case DT_LEFT:
		rcF.X = rc.left;
		break;
	case DT_RIGHT:
		rcF.X = rc.right - nImgWidth;
		break;
	case DT_CENTER:
		rcF.X = rc.left + (rc.getWidth() - nImgWidth) / 2;
	default:
		;
	}
	rcF.Width = rc.getWidth();
	rcF.Height = rc.getHeight();
	Gdiplus::Graphics gs(hdc);
	gs.DrawImage(pImage.get(), rcF);
}

HFONT WfxRender::createFontIndirect( LOGFONTW& logfont )
{
	return ::CreateFontIndirectW(&logfont);
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
