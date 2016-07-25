
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CGameFrameworkApp.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// Calculate our window dimensions
	RECT clientRect = { 0, 0, AppConfigInstance.GetConfig<LONG>("Width"), AppConfigInstance.GetConfig<LONG>("Height") };
	AdjustWindowRectEx(&clientRect, lpCreateStruct->style, true, lpCreateStruct->dwExStyle);

	// Retrieve the width and height configuration and update our window
	lpCreateStruct->cx = (clientRect.right - clientRect.left) + GetSystemMetrics(SM_CXFRAME);
	lpCreateStruct->cy = (clientRect.bottom - clientRect.top) + GetSystemMetrics(SM_CYFRAME);

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// Retrieve the available screen area
	CRect workArea(0, 0, 0, 0);
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);

	// Calculate our window dimensions
	RECT clientRect = { 0, 0, AppConfigInstance.GetConfig<LONG>("Width"), AppConfigInstance.GetConfig<LONG>("Height") };
	AdjustWindowRectEx(&clientRect, cs.style, true, cs.dwExStyle);

	// Centre our window in the screen
	cs.cx = (clientRect.right - clientRect.left) + GetSystemMetrics(SM_CXFRAME);
	cs.cy = (clientRect.bottom - clientRect.top) + GetSystemMetrics(SM_CYFRAME);
	cs.x = (workArea.Width() - cs.cx) / 2;
	cs.y = (workArea.Height() - cs.cy) / 2;

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_MINIMIZEBOX | WS_SYSMENU;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

