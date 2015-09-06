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
	class SliderBar;
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
typedef std::tr1::shared_ptr<Widget> SPWidget;
typedef std::tr1::shared_ptr<ScrollBar> SPScrollBar;
typedef std::tr1::shared_ptr<Dispatcher> SPDispatcher;
typedef _my_ptr<Dispatcher> PDispatcher;
typedef _my_ptr<Widget> PWidget;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
// MsgMap Interface of widget
class WFX_API MsgMap
{
public:
	virtual ~MsgMap();
public:
	virtual BOOL processMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID) = 0;
	LRESULT sendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API UnitBase
{
public:
	UnitBase(Widget* pParent = NULL);
	virtual ~UnitBase();
private:
	void doInit();
public:
	virtual void setText(const String& strText);
	virtual void setFormat(DWORD dwFormat);
	virtual void setTextColor(COLORREF clrText);
	virtual void setBkgnd(COLORREF clrBkgnd);
	virtual void setFrame(COLORREF clrFrame);
	virtual void setState(WORD wState);
	virtual void setRect(const Rect& rc);
	virtual void getText(String& str) const;
	const String& getText() const;
	virtual DWORD getFormat() const;
	virtual COLORREF getTextColor() const;
	virtual COLORREF getBkgnd() const;
	virtual COLORREF getFrame() const;
	virtual WORD getState() const;
	virtual void getRect(Rect& rc) const;
	const Rect& getRect() const;
	virtual Widget* getParent() const;
	virtual void setParent(Widget* pParent);
	virtual void getToolTip(String& str) const;
	virtual void setToolTip(const String& strToolTip);
	void getVirtualSize(Size& sz);
	const Size& getVirtualSize();
	BOOL isVirtualSizeCached() const;
	void setVirtualSizeCached(BOOL bCached = TRUE);
protected:
	virtual void calcVirtualSize(Size& sz);
protected:
	String m_strText;
	String m_strToolTip;
	DWORD m_dwFormat;
	COLORREF m_clrText;
	COLORREF m_clrBkgnd;
	COLORREF m_clrFrame;
	WORD m_wState;
	Rect m_rect;
	PWidget m_pParent;
	Size m_szVirtualSize;
	BOOL m_bCachedVirtualSize;
};
typedef std::tr1::shared_ptr<UnitBase> SPBasicAttr;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
// Widget: the root class of ui classes
class WFX_API Widget :
	public MsgMap, public UnitBase
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
private:
	void doInit();

public:
	WFX_BEGIN_MSG_MAP(Widget)
		WFX_ON_MESSAGE(WM_SIZE, onSize)
		WFX_ON_MESSAGE(WM_CREATE, onCreate)
		WFX_ON_MESSAGE(WM_TIMER, onTimer)
		WFX_ON_MESSAGE(WM_KEYDOWN, onKeyDown)
		WFX_ON_MESSAGE(WM_HSCROLL, onHScroll)
	WFX_END_MSG_MAP()
public:
	BOOL create(const Rect& rc, Widget* pParent = NULL, 
		Dispatcher* pDispatch = NULL, BOOL bNC = FALSE);

	// Position
public:
	virtual void setRect(const Rect& rc);
	virtual void setState(WORD wState);
public:
	void getParentRect(Rect& rc) const;
	void showWid(WORD wShow);
	BOOL isShow() const;
	BOOL isCaptured() const;
	void setCapture();
	void releaseCapture();
	BOOL isWidget() const;
protected:
	void doshowWid(WORD wShow);
	virtual Rect getClientRect() const;
	void setClientRect(const Rect& rc);
public:
	virtual void setParent(Widget* pParent);
	// For Dispatcher
protected:
	void setMyParent(Widget* pParent);
	ULONG getChildren(std::vector<Widget*>& rgpChildren) const;
	ULONG getChildren() const;
	BOOL hasChild() const;
protected:
	ULONG addChild(Widget* pWid);
	ULONG removeChild(Widget* pWid);
public:
	// Graphics
	void invalidWid();
protected:
	virtual void onDraw(HDC hdc, const Rect& rcPaint);

protected:
	void setHwid(HWID hWid);
	HWID getHwid() const;
	// Scrollbar
public:
	void enableScrollBar(UINT nBar, BOOL bEnable = TRUE);
	UINT getSBFlag() const;
	void setSBFlag(UINT uSBFlag);
	BOOL scrollWid(int XAmount, int YAmount);
public:
	virtual BOOL inClientAera(const Point& pt);
protected:
	virtual Rect getScrollBarRect(int nBar);
protected:
	void setScrollBar(int nBar, ScrollBar* pScrollBar);
	ScrollBar* getScrollBar(int nBar) const;
public:
	BOOL postMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT sendParentMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	UINT_PTR setTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc = NULL);
	BOOL killTimer(UINT_PTR uIDEvent);

public:
	wfx_msg LRESULT onSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	LONG getVOffset() const;
	LONG getHOffset() const;
public:
	UINT getID() const;
	void setID(UINT nID);
	inline Dispatcher* getDispatcher() const;
protected:
	virtual void calcVirtualSize(Size& sz);
private:
	Dispatcher* searchDispather() const;
	// Identifier
public:
	HWID m_hWid;
	PDispatcher m_pDispatch;
private:
	// Position
	Rect m_rcClient;
	BOOL m_bNC;
	WORD m_wShow;
	// Generation
	std::vector<Widget*> m_rgpChildren;
	// Scrollbar
	SPScrollBar m_pHScrollbar;
	SPScrollBar m_pVScrollbar;
	UINT m_uBarFlag;
	// Event ID
	UINT m_nID;
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API WidUnit : public MsgMap, public UnitBase
{
public:
	WidUnit(Widget* pWid = NULL);
public:
	virtual BOOL processMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID);
	LRESULT sendParentMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
public:
	virtual void onDraw(HDC hdc, const Rect& rc);
};
typedef std::tr1::shared_ptr<WidUnit> SPWidUnit;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API BarBase
{
public:
	BarBase(int nBar = SB_HORZ);
public:
	int getBar() const;
	void setBar(int nBar);
protected:
	int m_nBar;
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API WidCtrl : public Widget
{
public:
	WFX_BEGIN_MSG_MAP(WidCtrl)
		WFX_ON_MESSAGE(WM_MOUSEMOVE, onMouseMove)
		WFX_ON_MESSAGE(WM_MOUSELEAVE, onMouseLeave)
		WFX_ON_MESSAGE(WM_LBUTTONDOWN, onLButtonDown)
		WFX_ON_MESSAGE(WM_LBUTTONUP, onLButtonUp)
		WFX_ON_MESSAGE(WM_UPDATEUISTATE, onStateChanged)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT onMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onStateChanged(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API ProcessBar : public WidCtrl, public BarBase
{
public:
	ProcessBar(int nBar = SB_HORZ);
public:
	virtual void setRange(LONG nMin, LONG nMax);
	virtual LONG getRange() const;
	virtual LONG getMax() const;
	virtual LONG getMin() const;
	virtual void setPos(LONG nPos);
	virtual LONG getPos() const;
public:
	BOOL isCompleted() const;
	virtual void reset();
protected:
	virtual void onDraw(HDC hdc, const Rect& rc);
protected:
	LONG m_nMax;
	LONG m_nMin;
	LONG m_nPos;
};
typedef std::tr1::shared_ptr<ProcessBar> SPProcessBar;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
#define SliderBar_TIMER_MOUSEMOVE 1121
class WFX_API SliderBar : public ProcessBar
{
public:
	SliderBar(int nBar = SB_HORZ);
public:
	WFX_BEGIN_MSG_MAP(SliderBar)
		WFX_ON_MESSAGE(WM_LBUTTONDOWN, onLButtonDown)
		WFX_ON_MESSAGE(WM_LBUTTONUP, onLButtonUp)
		WFX_ON_MESSAGE(WM_MOUSEMOVE, onMouseMove)
		WFX_ON_MESSAGE(WM_TIMER, onTimer)
		WFX_ON_MESSAGE(WM_SIZE, onSize)
		WFX_CHAIN_MSG_MAP(ProcessBar)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT onLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);

public:
	virtual void setThumbSize(LONG nThumbSize);
	virtual LONG getThumbSize() const;
protected:
	virtual LONG getStep() const;
protected:
	virtual void onDraw(HDC hdc, const Rect& rcPaint);
protected:
	void setHorzThumbRect(LONG nThumbSize, LONG x);
	void setVertThumbRect(LONG nThumbSize, LONG y);
	LONG getHorzThumbPosMin() const;
	LONG getHorzThumbPosMax() const;
	LONG getHorzThumbPos() const;
	LONG getVertThumbPosMin() const;
	LONG getVertThumbPosMax() const;
	LONG getVertThumbPos() const;
	LONG calcHorzThumbPos();
	LONG calcVertThumbPos();
	void calcHorzPos();
	void calcVertPos();
protected:
	Point m_ptLButtonDown;
	Point m_ptMouseMove;
	Rect m_rcThumb;
	Rect m_rcThumbHolder;
	BOOL m_bInThumb;
	BOOL m_bDirection;
	LONG m_nThumbSize;
};
typedef std::tr1::shared_ptr<SliderBar> SPSliderBar;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
typedef std::tr1::shared_ptr<SCROLLINFO> SPSCROLLINFO;
// ScrollBar: Common ScrollBar
class WFX_API ScrollBar : public SliderBar
{
public:
	ScrollBar(int nBar = SB_HORZ);
	virtual ~ScrollBar();
public:
	WFX_BEGIN_MSG_MAP(ScrollBar)
		WFX_ON_MESSAGE(WM_SIZE, onSize)
		WFX_CHAIN_MSG_MAP(SliderBar)
	WFX_END_MSG_MAP()
public:
	void getScrollInfo(SCROLLINFO* pScrollInfo) const;
	void setScrollInfo(const SCROLLINFO* pScrollInfo);
protected:
	LONG calcHorzThumbSize(const Rect& rcWid);
	LONG calcVertThumbSize(const Rect& rcWid);
public:
	virtual void setRange(LONG nMin, LONG nMax);
	virtual LONG getRange() const;
	virtual LONG getMax() const;
	virtual LONG getMin() const;
	virtual void setPos(LONG nPos);
	virtual LONG getPos() const;
	virtual void reset();
public:
	wfx_msg LRESULT onSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	virtual void onDraw(HDC hdc, const Rect& rcPaint);
protected:
	LONG m_nArrorSize;
	Rect m_rcArrow1;
	Rect m_rcArrow2;
protected:
	SPSCROLLINFO m_pScrollInfo;
};

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API ImageWid : public WidCtrl
{
public:
	ImageWid();
	ImageWid(const String& strStatic,
		const String& strMouse, 
		const String& strPush,
		const String& strChecked);
public:
	void setImage(WORD wState, const String& strImage);
	void setImage(const String& strStatic,
		const String& strMouse, 
		const String& strPush,
		const String& strChecked);
protected:
	Gdiplus::Image* getImageFromState();
protected:
	SPImage m_pImgStatic;
	SPImage m_pImgMouse;
	SPImage m_pImgPush;
	SPImage m_pImgChecked;
};
typedef std::tr1::shared_ptr<ImageWid> SPImageWid;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Button : public ImageWid
{
public:
	Button(BOOL m_bCheckable = FALSE);
public:
	WFX_BEGIN_MSG_MAP(Button)
		WFX_ON_MESSAGE(WUM_LBUTTONCLICK, onLButtonClik)
		WFX_CHAIN_MSG_MAP(ImageWid)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT onLButtonClik(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onStateChanged(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	void check(BOOL bCheck = TRUE);
	BOOL isChecked() const;
protected:
	virtual void onDraw(HDC hdc, const Rect& rcPaint);
protected:
	BOOL m_bChecked;
	BOOL m_bCheckable;
};
typedef std::tr1::shared_ptr<Button> SPButton;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API CheckBox : public Button
{
public:
	CheckBox();
public:
	WFX_BEGIN_MSG_MAP(CheckBox)
		WFX_ON_MESSAGE(WM_SIZE, onSize)
		WFX_CHAIN_MSG_MAP(Button)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT onSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	virtual void onDraw(HDC hdc, const Rect& rcPaint);
protected:
	Gdiplus::Image* getItemImage() const;
	Rect getItemRect() const;
protected:
	ULONG m_lOffset;
	SPImage m_pImageChecked;
	SPImage m_pImageUnCheck;
	Rect m_rcItem;
};
typedef std::tr1::shared_ptr<CheckBox> SPCheckBox;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API RadioButton : public CheckBox
{
public:
	RadioButton();
protected:
	virtual void onDraw(HDC hdc, const Rect& rc);
};
typedef std::tr1::shared_ptr<RadioButton> SPRadioButton;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Label : public WidCtrl
{
protected:
	virtual void onDraw(HDC hdc, const Rect& rc);
};
typedef std::tr1::shared_ptr<Label> SPLabel;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API InPlaceWid : public WidCtrl
{
	friend class InPlaceWnd;
public:
	InPlaceWid();
public:
	WFX_BEGIN_MSG_MAP(InPlaceWid)
		WFX_ON_MESSAGE(WM_SETFOCUS, onSetFocus)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT onSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	virtual BOOL initial() = 0;
	void reset();
protected:
	InPlaceWnd* m_pWindow;
};

typedef std::tr1::shared_ptr<InPlaceWid> SPInPlaceWid;
typedef _my_ptr<InPlaceWid> PInPlaceWid;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API TextBox : public InPlaceWid
{
public:
	TextBox(WORD wMode = WID_TBM_READWRITE);
public:
	WFX_BEGIN_MSG_MAP(TextBox)
		WFX_ON_MESSAGE(WM_MOUSEMOVE, onMouseMove)
		//WFX_ON_MESSAGE(WM_MOUSELEAVE, onMouseLeave)
		WFX_CHAIN_MSG_MAP(InPlaceWid)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT onMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT onMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
public:
	void setMode(WORD wMode, BOOL bAdd = FALSE);
	WORD getMode() const;
	BOOL isReadonly() const;
	BOOL isPassword() const;
	void setReadonly();
protected:
	String getShowText() const;
public:
	virtual String getToolTip() const;
protected:
	virtual BOOL initial();
	virtual void onDraw(HDC hdc, const Rect& rcPaint);
protected:
	WORD m_wMode;
	BOOL m_bEditting;
};
typedef std::tr1::shared_ptr<TextBox> SPTextBox;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API ComboBox : public InPlaceWid
{
public:
	ComboBox();
public:
	WFX_BEGIN_MSG_MAP(ComboBox)
		WFX_ON_MESSAGE(WM_CREATE, onCreate)
		WFX_ON_MESSAGE(WM_SIZE, onSize)
		WFX_ON_MESSAGE(WUM_COMBO_GET_SIZE, ongetComboSize)
		WFX_ON_MESSAGE(WUM_COMBO_GET_CNT, ongetCount)
		WFX_ON_MESSAGE(WUM_COMBO_GET_ITEM_TEXT, ongetItemText)
		WFX_ON_MESSAGE(WUM_COMBO_GET_ITEM_HEIGHT, ongetItemHeight)
		WFX_ON_MESSAGE(WUM_COMBO_GET_SEL, ongetSelectedItem)
		WFX_ON_MESSAGE(WUM_COMBO_SET_SEL, onsetSelectedItem)
		WFX_CHAIN_MSG_MAP(InPlaceWid)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT onCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT onSize(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT ongetComboSize(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT ongetCount(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT ongetItemText(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT ongetItemHeight(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT ongetSelectedItem(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
	wfx_msg LRESULT onsetSelectedItem(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
protected:
	virtual BOOL initial();
	virtual void onDraw(HDC hdc, const Rect& rcPaint);
protected:
	LONG m_nSelected;
	SPTextBox m_pTextBox;
	Rect m_rcDropDown;
};
typedef std::tr1::shared_ptr<ComboBox> SPComboBox;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
typedef std::tr1::shared_ptr<Dispatcher> SPDispatcher;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
// Window: Window for controls
class WFX_API Window : public MsgMap
{
public:
	// !!! m_pDispatch must be the first member
	SPDispatcher m_pDispatch;
public:
	Window();
	virtual ~Window();
public:
	HWND getHwnd() const;
	operator HWND() const;

	BOOL registerWindowClass();
	BOOL registerSuperClass();

	HWND create(HWND hwndParent, LPCWSTR pszName,
		DWORD dwStyle, DWORD dwExStyle, const Rect& rc,
		HMENU hMenu = NULL);

	HWND subClass(HWND hWnd);
	void unSubClass();
	void showWindow(BOOL bShow = TRUE, BOOL bTakeFocus = TRUE);
	BOOL showModal();
	void close();
	void centerWindow();
	void setIcon(UINT nRes);
	String getText() const;
	void setText(const String& strText);
	void setFont(HFONT hFont) const;
	void getClientRect(Rect& rc);
	UINT_PTR setTimer(UINT_PTR nIDEvent,
		UINT uElapse,
		TIMERPROC lpTimerFunc) const;
protected:
	virtual LPCWSTR getWindowClassName() const;
	virtual LPCWSTR getSuperClassName() const;
	virtual UINT getClassStyle() const;

	LRESULT sendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT postMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);

	virtual BOOL processMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID);
	void resizeClient(int cx = -1, int cy = -1);
	virtual void onInitialMessage(HWND hWnd);
	virtual void onFinalMessage(HWND hWnd);
	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	static void getClientRect(HWND hWnd, Rect& rc);
	static BOOL isWindow(HWND hWnd);

protected:
	HWND m_hWnd;
	WNDPROC m_OldWndProc;
	BOOL m_bSubclassed;
};
typedef std::tr1::shared_ptr<Window> SPWindow;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API WidgetWnd : public Window
{
public:
	// !!! m_pDispatch must be the first member
	SPDispatcher m_pDispatch;
public:
	WidgetWnd();
public:
	virtual BOOL processMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID);
	Dispatcher* getDispatcher() const;
protected:
	virtual void onInitialMessage(HWND hWnd);
};
typedef std::tr1::shared_ptr<WidgetWnd> SPWidgetWnd;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API InPlaceWnd : public WidgetWnd
{
public:
	InPlaceWnd();
	virtual void onFinalMessage(HWND hWnd);
public:
	WFX_BEGIN_MSG_MAP(InPlaceWnd)
		WFX_ON_MESSAGE(WM_KILLFOCUS, onKillFocus)
		WFX_CHAIN_MSG_MAP(WidgetWnd)
	WFX_END_MSG_MAP()
wfx_msg LRESULT onKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam,
	BOOL& bHandled);
public:
	BOOL initial(InPlaceWid* pOwner);
	virtual HWND createInPlaceWindow() = 0;
	virtual void onInPlaceWindowKillFocus();
protected:
	PInPlaceWid m_pOwner;
};
typedef std::tr1::shared_ptr<InPlaceWnd> SPInPlaceWnd;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API TextBoxWnd : public InPlaceWnd
{
public:
	WFX_BEGIN_MSG_MAP(TextBoxWnd)
		WFX_ON_MESSAGE(OCM_COMMAND, onEditChanged)
		WFX_ON_MESSAGE(WM_MOUSEMOVE, onMouseMove)
		WFX_CHAIN_MSG_MAP(InPlaceWnd)
	WFX_END_MSG_MAP()
public:
	virtual void onInPlaceWindowKillFocus();
	virtual HWND createInPlaceWindow();
	virtual LPCWSTR getSuperClassName() const;
	virtual LPCWSTR getWindowClassName() const;
public:
	wfx_msg LRESULT onEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
};
typedef std::tr1::shared_ptr<InPlaceWnd> SPInPlaceWnd;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class ListCtrl;
typedef std::tr1::shared_ptr<ListCtrl> SPListCtrl;
class WFX_API ComboWnd : public InPlaceWnd
{
public:
	ComboWnd();
public:
	WFX_BEGIN_MSG_MAP(ComboWnd)
		WFX_ON_MESSAGE(WM_CREATE, onCreate)
		WFX_ON_MESSAGE(WM_SIZE, onSize)
		WFX_ON_MESSAGE(WM_LBUTTONDOWN, onLButtonDown)
		WFX_ON_MESSAGE(WM_LBUTTONUP, onLButtonUp)
		WFX_CHAIN_MSG_MAP(InPlaceWnd)
	WFX_END_MSG_MAP()
public:
	virtual void onInPlaceWindowKillFocus();
	virtual HWND createInPlaceWindow();
	virtual LPCWSTR getWindowClassName() const;
public:
	wfx_msg LRESULT onCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	Point m_pLButtonDown;
	SPListCtrl m_pListCtrl;
	Size m_szComboSize;
};

typedef std::tr1::shared_ptr<InPlaceWnd> SPInPlaceWnd;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API ToolTipWnd : public Window
{
public:
	ToolTipWnd();
public:
	void doToolTip(HWND hParent, const Rect& rc, const String& strToolTip);
	void endToolTip();
protected:
	SPToolInfo m_pToolInfo;
};

typedef std::tr1::shared_ptr<ToolTipWnd> SPToolTipWnd;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Menu : public InPlaceWnd
{

};
typedef std::tr1::shared_ptr<Menu> SPMenu;
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
	BOOL removeWidTimer(Widget* pWid, UINT_PTR nWidTimer);
	BOOL removeAllWidTimer(Widget* pWid, std::vector<UINT_PTR>& rgWndTimer);
protected:
	UINT_PTR generateWndTimer();
	BOOL recycleWndTimer(UINT_PTR nWndTimer);
protected:
	std::map<TimerKey, Timer2Timer> m_WidToWnd;
	std::map<UINT_PTR, TimerValue> m_WndToWid;
	UINT_PTR m_nTimerBase;
};
typedef std::tr1::shared_ptr<TimerTranslator> SPTimerTranslator;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class Timer
{
	friend class Dispatcher;
public:
	Timer(Dispatcher* pDispatch);
public:
	UINT_PTR setTimer(Widget* pWid, UINT_PTR nWidTimer, UINT uElapse, TIMERPROC lpTimerFunc);
	BOOL killTimer(Widget* pWid, UINT_PTR nWidTimer);
	BOOL getWidgetFromTimer(UINT_PTR nWndTimer, Widget*& pWid, UINT_PTR& nWidTimer);
	void destroy(Widget* pWid);
protected:
	PDispatcher m_pDispatch;
	SPTimerTranslator m_pTimerTranslator;
};
typedef std::tr1::shared_ptr<Timer> SPTimer;
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
	void setHwnd(HWND hWnd);
	HWND getHwnd() const;
protected:
	BOOL createWid(Widget* pThis);
	BOOL setParent(Widget* pThis, Widget* pParent);
	BOOL destroy(HWID& hWid);
	HWID generateHwid();
	void recycleHwid(HWID& hWid); 
	void clearH2O(std::pair<HWID, Widget*>& h2o);
	void setMouseMoveH2O(const std::pair<HWID, Widget*>& h2o);
	void setCapturedH2O(const std::pair<HWID, Widget*>& h2o);
	Widget* getObject(const std::pair<HWID, Widget*>& h2o);
protected:
	void drawWid(Widget* pWid, const Rect& rcPaint);
	void drawGen(Widget* pWid, HDC hdc, const Rect& rcPaint);
	void drawBkgnd(Widget* pWid, HDC hdc, const Rect& rcPaint);
	void onPaint(const Rect& rcPaint);
	void invalidate(const Rect& rc);
protected:
	void setWidRect(Widget* pWid, const Rect& rc);
	void showWid(Widget* pWid, WORD wShow);
public:
	LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	WFX_BEGIN_MSG_MAP(Dispatcher)
		WFX_ON_MESSAGE(WM_ERASEBKGND, onEraseBkgnd)
		WFX_ON_MESSAGE(WM_PAINT, onPaint)
		WFX_ON_MESSAGE(WM_MOUSEMOVE, onMouseMove)
		WFX_ON_MESSAGE(WM_LBUTTONDOWN, onLButtonDown)
		WFX_ON_MESSAGE(WM_LBUTTONUP, onLButtonUp)
		WFX_ON_MESSAGE(WM_KEYDOWN, onKeyDown)
		WFX_ON_MESSAGE(WM_MOUSEWHEEL, onMouseWheel)
		WFX_ON_MESSAGE(WM_COMMAND, onCommand)
		WFX_ON_MESSAGE(WM_TIMER, onTimer)
		WFX_ON_MESSAGE(WM_MOUSELEAVE, onMouseLeave)
		WFX_ON_MESSAGE(WM_MOUSEHOVER, onMouseHOver)
		WFX_ON_MESSAGE(WM_NOTIFY, onNotify)
		WFX_ON_MESSAGE(WM_KILLFOCUS, onKillFocus);
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT onEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onPaint(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onCommand(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onMouseHOver(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onNotify(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	Widget* getWidPt(const Point& pt);
	Widget* getWidPt(Widget* pWidget, const Point& pt);
	Widget* getWidPt(const std::vector<Widget*>& rgpWid);
	Widget* fromHwid(HWID hWid) const;
	void setCapture(Widget* pWid);
	BOOL isCaptured(const Widget* pWid) const;
	void setFocus(Widget* pWid);
	void setLButtonDown(Widget* pWid);
	void releaseCapture();
	BOOL isWidget(const Widget* pWidget) const;
public:
	void enableScrollBar(Widget* pWid, UINT uBarFlag, BOOL bEnable = TRUE);
	void setScrollInfo(Widget* pWid, int nBar, LPCSCROLLINFO lpsi, BOOL redraw);
	void getScrollInfo(Widget* pWid, int nBar, LPSCROLLINFO lpsi);
	void preProcessMsg(Widget* pWid, UINT uMsg, WPARAM wParam, LPARAM lParam);
	UINT_PTR setTimer(Widget* pWid, UINT_PTR nWidTimer, UINT uElapse, TIMERPROC lpTimerFunc);
	BOOL killTimer(Widget* pWid, UINT_PTR nWidTimer);
	BOOL getWidgetFromTimer(UINT_PTR nWndTimer, Widget*& pWid, UINT_PTR& nWidTimer);
protected:
	HWND m_hWnd;
	std::vector<HWID> m_rghWid;
	std::map<HWID, Widget*> m_Handle2Object;
	std::map<HWID, Widget*> m_h2oOrphan;
	std::pair<HWID, Widget*> m_h2oLastMouseMove;
	std::pair<HWID, Widget*> m_h2oCaptured;
	std::pair<HWID, Widget*> m_h2oFocused;
	std::pair<HWID, Widget*> m_h2oLButtonDown;
	SPTimer m_pTimer;
	BOOL m_bMouseTracking;
	SPToolTipWnd m_pToolTipWnd;
public:
	static HWID s_hWidBase;
public:
	static HINSTANCE getInstance();
	static void setInstance(HINSTANCE hInstance);
	static HWND getMainWnd();
	static void setMainWnd(HWND hMainWnd);
	static int messageLoop();
	static void getHSL(short& nH, short& nS, short& nL);
	static void setHSL(short nH, short nS, short nL);
	static int getAlpha();
	static void setAlpha(int nAlpha);
	static void setAlpha(int nAlpha, HWND hWnd);
private:
	static HINSTANCE s_hInstance;
	static HWND s_hMainWnd;
	static std::vector<Dispatcher*> s_rgpDispatch;
	static short s_nH;
	static short s_nS;
	static short s_nL;
	static int   s_nAlpha;
};

typedef Dispatcher WfxDispatcher;

END_NAMESPACE_WFX
