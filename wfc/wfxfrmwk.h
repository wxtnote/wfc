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

class MsgMap;
class Widget;
class Document;
class View;
typedef _my_ptr<Document> PDocument;
class WFX_API View : public Widget
{
	WFX_DECLARE_FACTORY(View);
protected:
	View();
public:
	Document* getDocument() const;
	void setDocument(Document* pDoc);
protected:
	PDocument m_pDoc;
};
typedef Factory<View>							ViewFactory;
typedef std::tr1::shared_ptr<ViewFactory>					SSPViewFactory;
typedef const SSPViewFactory						CSPViewFactroy;
typedef std::tr1::shared_ptr<View>							SPView;
typedef _my_ptr<View> PView;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Document : public MsgMap
{
	WFX_DECLARE_FACTORY(Document);
protected:
	Document();
public:
	virtual BOOL processMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID);
public:
	virtual BOOL open();
	virtual BOOL close();
	View* getView() const;
	void setView(View* pView);
protected:
	PView m_pView;
};
typedef Factory<Document>						DocFactory;
typedef std::tr1::shared_ptr<DocFactory>					SPDocFactory;
typedef const SPDocFactory						CSPDocFactory;
typedef std::tr1::shared_ptr<Document>						SPDocutment;
typedef std::pair<SPDocFactory, SSPViewFactory>	Doc2ViewFactory;
typedef std::vector<Doc2ViewFactory>			Doc2ViewFactoryList;
typedef std::pair<SPDocutment, SPView>			Doc2View;
typedef std::tr1::shared_ptr<Doc2View>						SPDoc2View;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API DocManager : public MsgMap
{
public:
	DocManager();
public:
	virtual BOOL processMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID);
public:
	virtual ULONG addDoc(CSPDocFactory& pDocFactory,
		CSPViewFactroy& pViewFactory);
	virtual Document* next();
	virtual Document* pre();
	virtual Document* getAt(int i);
	virtual BOOL open(int i);
	virtual BOOL close(int i);
protected:
	Doc2ViewFactoryList m_rgpDocToView;
	SPDoc2View			m_pCurDoc2View;
	LONG m_nCurrent;
};
typedef std::tr1::shared_ptr<DocManager> SPDocManager;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Frame : public Widget
{
public:
	Frame();
public:
	virtual Document* getActiveDoc();
public:
	View* getActiveView();
protected:
	SPWidget m_pMenuBar;
	SPWidget m_pStatusBar;
};
typedef std::tr1::shared_ptr<Frame> SPFrame;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Application : public MsgMap
{
public:
	Application(HINSTANCE hInstance, SPFrame pFrame = SPFrame());
protected:
	void addDocument(CSPDocFactory& pDocFactory,
		CSPViewFactroy& pViewFactory);
public:
	virtual BOOL initInstance();
	virtual int exitInstance();
	virtual int run();
protected:
	HINSTANCE m_hInstance;
	SPDocManager m_pDocMgr;
	SPFrame m_pFrame;
};
typedef std::tr1::shared_ptr<Application> SPApplication;

END_NAMESPACE_WFX
