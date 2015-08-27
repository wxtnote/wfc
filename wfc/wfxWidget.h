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

///////////////////////////*** a gorgeous partition line ***/////////////////////////////

// forward declare
class Dispatcher;
class Timer;

class Widget;
	class Slider;
	class ScrollBar;
	class ImageWid;
		class Button;
			class RadioButtonItem;
			class CheckBoxItem;
	class CheckBox;
	class RadioButton;
	class Label;
	class ProcessBar;

	class ToolTip;
	class InPlaceWid;

class Window;
	class InPlaceWnd;
		class EditWnd;
		class ComboWnd;
		class Menu;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
// MsgMap Interface of widget
class WFX_API MsgMap
{
public:
	virtual BOOL ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID) = 0;
	LRESULT SendWidMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API AttrBase
{
public:
	AttrBase(Widget* pParent = NULL);
public:
	virtual void SetText(const String& strText);
	virtual void SetFormat(DWORD dwFormat);
	virtual void SetTextColor(COLORREF clrText);
	virtual void SetBkgnd(COLORREF clrBkgnd);
	virtual void SetFrame(COLORREF clrFrame);
	virtual void SetState(WORD wState);
	virtual void SetRect(const Rect& rect);
	virtual String GetText() const;
	virtual DWORD GetFormat() const;
	virtual COLORREF GetTextColor() const;
	virtual COLORREF GetBkgnd() const;
	virtual COLORREF GetFrame() const;
	virtual WORD GetState() const;
	virtual Rect GetRect() const;
	virtual Widget* GetParent() const;
	virtual void SetParent(Widget* pParent);
	virtual String GetToolTip() const;
	virtual void SetToolTip(const String& strToolTip);
	Size GetVirtualSize();
	BOOL IsVirtualSizeCached() const;
	void SetVirtualSizeCached(BOOL bCached = TRUE);
protected:
	virtual Size CalcVirtualSize();
protected:
	String m_strText;
	String m_strToolTip;
	DWORD m_dwFormat;
	COLORREF m_clrText;
	COLORREF m_clrBkgnd;
	COLORREF m_clrFrame;
	WORD m_wState;
	Rect m_rect;
	Widget* m_pParent;
	Size m_szVirtualSize;
	BOOL m_bCachedVirtualSize;
};
typedef SharedPtr<AttrBase> PBasicAttr;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
// Widget: the root class of ui classes
class WFX_API Widget :
	public MsgMap, public AttrBase
{
	friend class Dispatcher;
	friend class Timer;
	friend class TimerTranslator;
public:
	typedef std::vector<Widget*> WidList;
	typedef std::vector<Widget*>::iterator WidIter;
	typedef std::vector<Widget*>::const_iterator CWidIter;
public:
	Widget(void);
	virtual ~Widget(void);
	operator HWID() const;

public:
	WFX_BEGIN_MSG_MAP(Widget)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WM_TIMER, OnTimer)
		WFX_MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		WFX_MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
	WFX_END_MSG_MAP()
public:
	BOOL Create(const Rect& rc, Widget* pParent = NULL, 
		Dispatcher* pDispatch = NULL, BOOL bNC = FALSE);

	// Position
public:
	virtual void SetRect(const Rect& rc);
	virtual void SetState(WORD wState);
public:
	Rect GetParentRect() const;
	void ShowWid(WORD wShow);
	BOOL IsShow() const;
	BOOL IsCaptured() const;
	void SetCapture();
	void ReleaseCapture();
	BOOL IsWidget() const;
protected:
	void MyShowWid(WORD wShow);
	virtual Rect GetClientRect() const;
	void SetClientRect(const Rect& rc);
public:
	void SetParent(Widget* pParent);
	// For Dispatcher
protected:
	void SetMyParent(Widget* pParent);
	ULONG GetChildren(std::vector<Widget*>& rgpChildren) const;
	ULONG GetChildren() const;
	BOOL HasChild() const;
protected:
	ULONG AddChild(Widget* pWid);
	ULONG RemoveChild(Widget* pWid);
public:
	// Graphics
	void InvalidWid();
protected:
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);

protected:
	void SetHwid(HWID hWid);
	HWID GetHwid() const;
	// Scrollbar
public:
	void EnableScrollBar(UINT nBar, BOOL bEnable = TRUE);
	UINT GetSBFlag() const;
	void SetSBFlag(UINT uSBFlag);
	BOOL ScrollWid(int XAmount, int YAmount);
public:
	virtual BOOL InClientAera(const Point& pt);
protected:
	virtual Rect GetScrollBarRect(int nBar);
protected:
	void SetScrollBar(int nBar, ScrollBar* pScrollBar);
	ScrollBar* GetScrollBar(int nBar) const;
public:
	BOOL PostWidMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT SendParentMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	UINT_PTR SetWidTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc = NULL);
	BOOL KillWidTimer(UINT_PTR uIDEvent);

public:
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	LONG GetVOffset() const;
	LONG GetHOffset() const;
public:
	UINT GetID() const;
	void SetID(UINT nID);
	inline Dispatcher* GetDispatcher() const;
protected:
	virtual Size CalcVirtualSize();
private:
	Dispatcher* SearchDispather() const;
	// Identifier
public:
	HWID m_hWid;
	Dispatcher* m_pDispatch;
private:
	// Position
	Rect m_rcClient;
	BOOL m_bNC;
	WORD m_wShow;
	// Generation
	std::vector<Widget*> m_rgpChildren;
	// Scrollbar
	ScrollBar* m_pHScrollbar;
	ScrollBar* m_pVScrollbar;
	UINT m_uBarFlag;
	// Event ID
	UINT m_nID;
};

typedef SharedPtr<Widget> PWidget;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API UnitBase : public MsgMap, public AttrBase
{
public:
	UnitBase(Widget* pWid = NULL);
public:
	virtual BOOL ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID);
	LRESULT SendParentMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
public:
	virtual void OnDraw(HDC hdc, const Rect& rc);
};
typedef SharedPtr<UnitBase> PUnitBase;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API BarBase
{
public:
	BarBase(int nBar = SB_HORZ);
public:
	int GetBar() const;
	void SetBar(int nBar);
protected:
	int m_nBar;
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API WidCtrlBase : public Widget
{
public:
	WFX_BEGIN_MSG_MAP(WidCtrlBase)
		WFX_MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		WFX_MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		WFX_MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		WFX_MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		WFX_MESSAGE_HANDLER(WM_UPDATEUISTATE, OnStateChanged)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnStateChanged(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API ProcessBar : public WidCtrlBase, public BarBase
{
public:
	ProcessBar(int nBar = SB_HORZ);
public:
	virtual void SetRange(LONG nMin, LONG nMax);
	virtual LONG GetRange() const;
	virtual LONG GetMax() const;
	virtual LONG GetMin() const;
	virtual void SetPos(LONG nPos);
	virtual LONG GetPos() const;
public:
	BOOL IsCompleted() const;
	virtual void Reset();
protected:
	virtual void OnDraw(HDC hdc, const Rect& rc);
protected:
	LONG m_nMax;
	LONG m_nMin;
	LONG m_nPos;
};
typedef SharedPtr<ProcessBar> PProcessBar;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
#define SLIDER_TIMER_MOUSEMOVE 1121
class WFX_API Slider : public ProcessBar
{
public:
	Slider(int nBar = SB_HORZ);
public:
	WFX_BEGIN_MSG_MAP(Slider)
		WFX_MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		WFX_MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		WFX_MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		WFX_MESSAGE_HANDLER(WM_TIMER, OnTimer)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_CHAIN_MSG_MAP(ProcessBar)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);

public:
	virtual void SetThumbSize(LONG nThumbSize);
	virtual LONG GetThumbSize() const;
protected:
	virtual LONG GetStep() const;
protected:
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);
protected:
	void SetHorzThumbRect(LONG nThumbSize, LONG x);
	void SetVertThumbRect(LONG nThumbSize, LONG y);
	LONG GetHorzThumbPosMin() const;
	LONG GetHorzThumbPosMax() const;
	LONG GetHorzThumbPos() const;
	LONG GetVertThumbPosMin() const;
	LONG GetVertThumbPosMax() const;
	LONG GetVertThumbPos() const;
	LONG CalcHorzThumbPos();
	LONG CalcVertThumbPos();
	void CalcHorzPos();
	void CalcVertPos();
protected:
	Point m_ptLButtonDown;
	Point m_ptMouseMove;
	Rect m_rcThumb;
	Rect m_rcThumbHolder;
	BOOL m_bInThumb;
	BOOL m_bDirection;
	LONG m_nThumbSize;
};
typedef SharedPtr<Slider> PSlider;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
typedef SharedPtr<SCROLLINFO> PSCROLLINFO;
// ScrollBar: Common ScrollBar
class WFX_API ScrollBar : public Slider
{
public:
	ScrollBar(int nBar = SB_HORZ);
	virtual ~ScrollBar();
public:
	WFX_BEGIN_MSG_MAP(ScrollBar)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_CHAIN_MSG_MAP(Slider)
	WFX_END_MSG_MAP()
public:
	void GetScrollInfo(SCROLLINFO* pScrollInfo) const;
	void SetScrollInfo(const SCROLLINFO* pScrollInfo);
protected:
	LONG CalcHorzThumbSize(const Rect& rcWid);
	LONG CalcVertThumbSize(const Rect& rcWid);
public:
	virtual void SetRange(LONG nMin, LONG nMax);
	virtual LONG GetRange() const;
	virtual LONG GetMax() const;
	virtual LONG GetMin() const;
	virtual void SetPos(LONG nPos);
	virtual LONG GetPos() const;
	virtual void Reset();
public:
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);
protected:
	LONG m_nArrorSize;
	Rect m_rcArrow1;
	Rect m_rcArrow2;
protected:
	PSCROLLINFO m_pScrollInfo;
};
typedef SharedPtr<ScrollBar> PScrollBar;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API ImageWid : public WidCtrlBase
{
public:
	ImageWid();
	ImageWid(const String& strStatic,
		const String& strMouse, 
		const String& strPush,
		const String& strChecked);
public:
	void SetImage(WORD wState, const String& strImage);
	void SetImage(const String& strStatic,
		const String& strMouse, 
		const String& strPush,
		const String& strChecked);
protected:
	Gdiplus::Image* GetImageFromState();
protected:
	PImage m_pImgStatic;
	PImage m_pImgMouse;
	PImage m_pImgPush;
	PImage m_pImgChecked;
};
typedef SharedPtr<ImageWid> PImageWid;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Button : public ImageWid
{
public:
	Button(BOOL m_bCheckable = FALSE);
public:
	WFX_BEGIN_MSG_MAP(Button)
		WFX_MESSAGE_HANDLER(WUM_LBUTTONCLICK, OnLButtonClik)
		WFX_CHAIN_MSG_MAP(ImageWid)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnLButtonClik(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnStateChanged(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	void Check(BOOL bCheck = TRUE);
	BOOL IsChecked() const;
protected:
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);
protected:
	BOOL m_bChecked;
	BOOL m_bCheckable;
};
typedef SharedPtr<Button> PButton;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API CheckBox : public Button
{
public:
	CheckBox();
public:
	WFX_BEGIN_MSG_MAP(CheckBox)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_CHAIN_MSG_MAP(Button)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);
protected:
	Gdiplus::Image* GetItemImage() const;
	Rect GetItemRect() const;
protected:
	ULONG m_lOffset;
	PImage m_pImageChecked;
	PImage m_pImageUnCheck;
	Rect m_rcItem;
};
typedef SharedPtr<CheckBox> PCheckBox;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API RadioButton : public CheckBox
{
public:
	RadioButton();
protected:
	virtual void OnDraw(HDC hdc, const Rect& rc);
};
typedef SharedPtr<RadioButton> PRadioButton;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Label : public WidCtrlBase
{
protected:
	virtual void OnDraw(HDC hdc, const Rect& rc);
};
typedef SharedPtr<Label> PLabel;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API InPlaceWid : public WidCtrlBase
{
	friend class InPlaceWnd;
public:
	InPlaceWid();
public:
	WFX_BEGIN_MSG_MAP(InPlaceWid)
		WFX_MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		WFX_CHAIN_MSG_MAP(WidCtrlBase)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	virtual BOOL Initial() = 0;
	void Reset();
protected:
	InPlaceWnd* m_pWindow;
};

typedef SharedPtr<InPlaceWid> PInPlaceWid;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API TextBox : public InPlaceWid
{
public:
	TextBox(WORD wMode = WID_TBM_READWRITE);
public:
	WFX_BEGIN_MSG_MAP(TextBox)
		WFX_MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		//WFX_MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		WFX_CHAIN_MSG_MAP(InPlaceWid)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
public:
	void SetMode(WORD wMode, BOOL bAdd = FALSE);
	WORD GetMode() const;
	BOOL IsReadonly() const;
	BOOL IsPassword() const;
protected:
	String GetShowText() const;
public:
	virtual String GetToolTip() const;
protected:
	virtual BOOL Initial();
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);
protected:
	WORD m_wMode;
	BOOL m_bEditting;
};
typedef SharedPtr<TextBox> PTextBox;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API ComboBox : public InPlaceWid
{
public:
	ComboBox();
public:
	WFX_BEGIN_MSG_MAP(ComboBox)
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_MESSAGE_HANDLER(WUM_COMBO_GET_SIZE, OnGetComboSize)
		WFX_MESSAGE_HANDLER(WUM_COMBO_GET_CNT, OnGetCount)
		WFX_MESSAGE_HANDLER(WUM_COMBO_GET_ITEM_TEXT, OnGetItemText)
		WFX_MESSAGE_HANDLER(WUM_COMBO_GET_ITEM_HEIGHT, OnGetItemHeight)
		WFX_MESSAGE_HANDLER(WUM_COMBO_GET_SEL, OnGetSelectedItem)
		WFX_MESSAGE_HANDLER(WUM_COMBO_SET_SEL, OnSetSelectedItem)
		WFX_CHAIN_MSG_MAP(InPlaceWid)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT OnGetComboSize(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT OnGetCount(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT OnGetItemText(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT OnGetItemHeight(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT OnGetSelectedItem(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT OnSetSelectedItem(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
protected:
	virtual BOOL Initial();
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);
protected:
	LONG m_nSelected;
	PTextBox m_pTextBox;
	Rect m_rcDropDown;
};
typedef SharedPtr<ComboBox> PComboBox;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
typedef SharedPtr<Dispatcher> PDispatcher;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
// Window: Window for controls
class WFX_API Window : public MsgMap
{
public:
	// !!! m_pDispatch must be the first member
	PDispatcher m_pDispatch;
public:
	Window();
	virtual ~Window();
public:
	HWND GetHwnd() const;
	operator HWND() const;

	BOOL RegisterWindowClass();
	BOOL RegisterSuperClass();

	HWND Create(HWND hwndParent, LPCWSTR pszName,
		DWORD dwStyle, DWORD dwExStyle, const Rect& rc,
		HMENU hMenu = NULL);

	HWND SubClass(HWND hWnd);
	void UnSubClass();
	void ShowWindow(BOOL bShow = TRUE, BOOL bTakeFocus = TRUE);
	BOOL ShowModal();
	void Close();
	void CenterWindow();
	void SetIcon(UINT nRes);
	String GetText() const;
	void SetText(const String& strText);
	void SetFont(HFONT hFont) const;
	void GetClientRect(Rect& rc);
	UINT_PTR SetTimer(UINT_PTR nIDEvent,
		UINT uElapse,
		TIMERPROC lpTimerFunc) const;
protected:
	virtual LPCWSTR GetWindowClassName() const;
	virtual LPCWSTR GetSuperClassName() const;
	virtual UINT GetClassStyle() const;

	LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);

	virtual BOOL ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID);
	void ResizeClient(int cx = -1, int cy = -1);
	virtual void OnInitialMessage(HWND hWnd);
	virtual void OnFinalMessage(HWND hWnd);
	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	HWND m_hWnd;
	WNDPROC m_OldWndProc;
	BOOL m_bSubclassed;
};
typedef SharedPtr<Window> PWindow;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API WidgetWnd : public Window
{
public:
	// !!! m_pDispatch must be the first member
	PDispatcher m_pDispatch;
public:
	WidgetWnd();
public:
	virtual BOOL ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID);
	Dispatcher* GetDispatcher() const;
protected:
	virtual void OnInitialMessage(HWND hWnd);
};
typedef SharedPtr<WidgetWnd> PWidgetWnd;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API InPlaceWnd : public WidgetWnd
{
public:
	InPlaceWnd();
	virtual void OnFinalMessage(HWND hWnd);
public:
	WFX_BEGIN_MSG_MAP(InPlaceWnd)
		WFX_MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
		WFX_CHAIN_MSG_MAP(WidgetWnd)
	WFX_END_MSG_MAP()
wfx_msg LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam,
	BOOL& bHandled);
public:
	BOOL Initial(InPlaceWid* pOwner);
	virtual HWND CreateInPlaceWindow() = 0;
	virtual void OnInPlaceWindowKillFocus();
protected:
	InPlaceWid* m_pOwner;
};
typedef SharedPtr<InPlaceWnd> PInPlaceWnd;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API TextBoxWnd : public InPlaceWnd
{
public:
	WFX_BEGIN_MSG_MAP(TextBoxWnd)
		WFX_MESSAGE_HANDLER(OCM_COMMAND, OnEditChanged)
		WFX_MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		WFX_CHAIN_MSG_MAP(InPlaceWnd)
	WFX_END_MSG_MAP()
public:
	virtual void OnInPlaceWindowKillFocus();
	virtual HWND CreateInPlaceWindow();
	virtual LPCWSTR GetSuperClassName() const;
	virtual LPCWSTR GetWindowClassName() const;
public:
	wfx_msg LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
};
typedef SharedPtr<InPlaceWnd> PInPlaceWnd;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class ListCtrl;
typedef SharedPtr<ListCtrl> PListCtrl;
class WFX_API ComboWnd : public InPlaceWnd
{
public:
	ComboWnd();
public:
	WFX_BEGIN_MSG_MAP(ComboWnd)
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		WFX_MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		WFX_CHAIN_MSG_MAP(InPlaceWnd)
	WFX_END_MSG_MAP()
public:
	virtual void OnInPlaceWindowKillFocus();
	virtual HWND CreateInPlaceWindow();
	virtual LPCWSTR GetWindowClassName() const;
public:
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	Point m_pLButtonDown;
	PListCtrl m_pListCtrl;
	Size m_szComboSize;
};

typedef SharedPtr<InPlaceWnd> PInPlaceWnd;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API ToolTipWnd : public Window
{
public:
	ToolTipWnd();
public:
	void DoToolTip(HWND hParent, const Rect& rc, const String& strToolTip);
	void EndToolTip();
protected:
	PToolInfo m_pToolInfo;
};

typedef SharedPtr<ToolTipWnd> PToolTipWnd;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Menu : public InPlaceWnd
{

};
typedef SharedPtr<Menu> PMenu;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
struct TimerKey
{
	HWID m_hWid;
	Widget* m_pWid;
	TimerKey();
	TimerKey(HWID hWid, Widget* pWid);
	bool operator()(const TimerKey& key1, const TimerKey& key2) const;
	bool operator<(const TimerKey& key) const;
	bool operator==(const TimerKey& key) const;
	bool operator!=(const TimerKey& key) const;
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
struct TimerValue
{
	TimerValue()
		: m_nWidTimer(0)
	{

	}
	TimerValue(const TimerKey& tk, UINT_PTR nWidTimer)
		: m_tk(tk)
		, m_nWidTimer(nWidTimer)
	{

	}
	TimerKey m_tk;
	UINT_PTR m_nWidTimer;
};
typedef std::map<UINT_PTR, UINT_PTR> Timer2Timer;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class TimerTranslator
{
public:
	TimerTranslator();
public:
	BOOL WidTimerToWndTimer(Widget* pWid, UINT_PTR nWidTimer, UINT_PTR& nWndTimer);
	BOOL WndTimerToWidTimer(UINT_PTR nWndTimer, Widget*& pWid, UINT_PTR& nWidTimer);
	BOOL RemoveWidTimer(Widget* pWid, UINT_PTR nWidTimer);
	BOOL RemoveAllWidTimer(Widget* pWid, std::vector<UINT_PTR>& rgWndTimer);
protected:
	UINT_PTR GenerateWndTimer();
	BOOL RecycleWndTimer(UINT_PTR nWndTimer);
protected:
	std::map<TimerKey, Timer2Timer> m_WidToWnd;
	std::map<UINT_PTR, TimerValue> m_WndToWid;
	UINT_PTR m_nTimerBase;
};
typedef SharedPtr<TimerTranslator> PTimerTranslator;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class Timer
{
	friend class Dispatcher;
public:
	Timer(Dispatcher* pDispatch);
public:
	UINT_PTR SetWidTimer(Widget* pWid, UINT_PTR nWidTimer, UINT uElapse, TIMERPROC lpTimerFunc);
	BOOL KillWidTimer(Widget* pWid, UINT_PTR nWidTimer);
	BOOL GetWidgetFromTimer(UINT_PTR nWndTimer, Widget*& pWid, UINT_PTR& nWidTimer);
	void Destroy(Widget* pWid);
protected:
	Dispatcher* m_pDispatch;
	PTimerTranslator m_pTimerTranslator;
};
typedef SharedPtr<Timer> PTimer;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
// Dispatcher: dispatch messages for widget
class WFX_API Dispatcher : public MsgMap
{
	friend class Widget;
	friend class Timer;
public:
	Dispatcher(HWND hWnd = NULL);
	virtual ~Dispatcher();
public:
	void SetHwnd(HWND hWnd);
	HWND GetHwnd() const;
protected:
	BOOL Create(Widget* pThis);
	BOOL SetParent(Widget* pThis, Widget* pParent);
	BOOL Destroy(HWID& hWid);
	HWID GenerateHwid();
	void RecycleHwid(HWID& hWid); 
	void ClearH2O(std::pair<HWID, Widget*>& h2o);
	void SetMouseMoveH2O(const std::pair<HWID, Widget*>& h2o);
	void SetCapturedH2O(const std::pair<HWID, Widget*>& h2o);
	Widget* GetObject(const std::pair<HWID, Widget*>& h2o);
protected:
	void DrawWid(Widget* pWid, const Rect& rcPaint);
	void DrawGen(Widget* pWid, HDC hdc, const Rect& rcPaint);
	void DrawBkgnd(Widget* pWid, HDC hdc, const Rect& rcPaint);
	void OnPaint(const Rect& rcPaint);
	void Invalidate(const Rect& rc);
protected:
	void SetWidRect(Widget* pWid, const Rect& rc);
	void ShowWid(Widget* pWid, WORD wShow);
public:
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	WFX_BEGIN_MSG_MAP(Dispatcher)
		WFX_MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		WFX_MESSAGE_HANDLER(WM_PAINT, OnPaint)
		WFX_MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		WFX_MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		WFX_MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		WFX_MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		WFX_MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
		WFX_MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		WFX_MESSAGE_HANDLER(WM_TIMER, OnTimer)
		WFX_MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		WFX_MESSAGE_HANDLER(WM_MOUSEHOVER, OnMouseHOver)
		WFX_MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
		WFX_MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus);
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseHOver(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	Widget* GetWidPt(const Point& pt);
	Widget* GetWidPt(Widget* pWidget, const Point& pt);
	Widget* GetWidPt(const std::vector<Widget*>& rgpWid);
	Widget* FromHwid(HWID hWid) const;
	void SetCapture(Widget* pWid);
	BOOL IsCaptured(const Widget* pWid) const;
	void SetFocus(Widget* pWid);
	void SetLButtonDown(Widget* pWid);
	void ReleaseCapture();
	BOOL IsWidget(const Widget* pWidget) const;
public:
	void EnableScrollBar(Widget* pWid, UINT uBarFlag, BOOL bEnable = TRUE);
	void SetScrollInfo(Widget* pWid, int nBar, LPCSCROLLINFO lpsi, BOOL redraw);
	void GetScrollInfo(Widget* pWid, int nBar, LPSCROLLINFO lpsi);
	void PreProcessMsg(Widget* pWid, UINT uMsg, WPARAM wParam, LPARAM lParam);
	UINT_PTR SetWidTimer(Widget* pWid, UINT_PTR nWidTimer, UINT uElapse, TIMERPROC lpTimerFunc);
	BOOL KillWidTimer(Widget* pWid, UINT_PTR nWidTimer);
	BOOL GetWidgetFromTimer(UINT_PTR nWndTimer, Widget*& pWid, UINT_PTR& nWidTimer);
protected:
	HWND m_hWnd;
	std::vector<HWID> m_rghWid;
	std::map<HWID, Widget*> m_Handle2Object;
	std::map<HWID, Widget*> m_h2oOrphan;
	std::pair<HWID, Widget*> m_h2oLastMouseMove;
	std::pair<HWID, Widget*> m_h2oCaptured;
	std::pair<HWID, Widget*> m_h2oFocused;
	std::pair<HWID, Widget*> m_h2oLButtonDown;
	PTimer m_pTimer;
	BOOL m_bMouseTracking;
	PToolTipWnd m_pToolTipWnd;
public:
	static HWID s_hWidBase;
public:
	static HINSTANCE GetInstance();
	static void SetInstance(HINSTANCE hInstance);
	static HWND GetMainWnd();
	static void SetMainWnd(HWND hMainWnd);
	static int MessageLoop();
	static void GetHSL(short& nH, short& nS, short& nL);
	static void SetHSL(short nH, short nS, short nL);
	static int GetAlpha();
	static void SetAlpha(int nAlpha);
	static void SetAlpha(int nAlpha, HWND hWnd);
private:
	static HINSTANCE s_hInstance;
	static HWND s_hMainWnd;
	static std::vector<Dispatcher*> s_rgpDispatch;
	static short s_nH;
	static short s_nS;
	static short s_nL;
	static int   s_nAlpha;
};
typedef SharedPtr<Dispatcher> PDispatcher;
typedef Dispatcher WfxDispatcher;

END_NAMESPACE_WFX
