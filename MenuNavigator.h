#pragma once

#include "UI.h"
#include "combase.h"
#include <wrl/client.h>
#include <mutex>

using namespace Microsoft::WRL;

class CMenuNavigator
	: public CComUnknown
	, public IMenuNavigator
{
public:
	CMenuNavigator();
	virtual ~CMenuNavigator();

public:
	//
	// IUnknown
	//
	DECLARE_IUNKNOWN
	virtual HRESULT NonDelegatingQueryInterface(REFIID riid, void** ppvObject);

	//
	// IMenuNavigator
	//
	STDMETHOD(Back)();
	STDMETHOD(Forward)();
	STDMETHOD(GoUp)();
	STDMETHOD(Refresh)();
	STDMETHOD(GetCurrentPage)(
		/* [out] */ IMenuPage** ppMenuPage);
	STDMETHOD(Navigate)(
		/* [in]  */ NAVIMENU_ID idMenu,
		/* [in]  */ MENUPAGE_COOKIE cookieMenuPage,
		/* [in[  */ IMenuPage* pUpperMenu);
	STDMETHOD(IsSupport)(
		/* [in]  */ NAVIMENU_ID idMenu);

protected:
	virtual HRESULT ActivateMenuPage(NAVIMENU_ID idMenu, MENUPAGE_COOKIE cookieMenuPage, IMenuPage** ppMenuPage) {
		UNREFERENCED_PARAMETER(idMenu);
		UNREFERENCED_PARAMETER(cookieMenuPage);
		UNREFERENCED_PARAMETER(ppMenuPage);
		return E_NOTIMPL;
	}

protected:
	std::list<ComPtr<IMenuPage>>	m_backPages;
	std::list<ComPtr<IMenuPage>>	m_forwardPages;
	ComPtr<IMenuPage>				m_currentPage;

	std::mutex						m_mutexList;
};

class CBaseMenuPage
	: public CComUnknown
	, public IMenuPage
{
public:
	CBaseMenuPage(NAVIMENU_ID idMenu, MENUPAGE_COOKIE cookieMenuPage, IMenuPage* pUpperMenu = nullptr);
	virtual ~CBaseMenuPage();

	//
	// IUnknown Interface
	//
	DECLARE_IUNKNOWN
	virtual HRESULT NonDelegatingQueryInterface(REFIID riid, void** ppvObject);

	//
	// IMenuPage Interface
	//
	STDMETHOD(Process)(
		/* [in] */ const char* szInput);
	STDMETHOD(OnNotify)(
		/* [in] */ unsigned int message,
		/* [in[ */ NOTIFY_PARAM param1,
		/* [in] */ NOTIFY_PARAM param2);
	STDMETHOD(Show)();
	STDMETHOD(GetUpperMenuPage)(
		/* [in] */ IMenuPage** ppMenuPage);
	STDMETHOD_(NAVIMENU_ID, GetMenuID)();
	STDMETHOD_(MENUPAGE_COOKIE, GetCookie)();
	STDMETHOD_(BOOL, HotkeyInput)();

protected:
	std::mutex						m_mutex;

	NAVIMENU_ID						m_idMenu = GUID_NULL;
	ComPtr<IMenuPage>				m_spUpperMenuPage;
	MENUPAGE_COOKIE					m_cookieMenuPage = 0;
};
