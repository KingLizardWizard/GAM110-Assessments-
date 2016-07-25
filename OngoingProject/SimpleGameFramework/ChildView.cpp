
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "CGameFrameworkApp.h"
#include "ChildView.h"
#include "GameInput.h"
#include "GameManager.h"

#include <WinUser.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Setup the timer for our core update method
	SetTimer(TimerID_UpdateTimer, UpdateTimer_Rate, NULL);

	// Setup the input and the game
	GameFrameworkInstance;
	GameInputInstance.BeginPlay();
	GameManagerInstance.BeginPlay();

	return 0;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this);

	CRect childViewRect;
	GetClientRect(&childViewRect);

	GameFrameworkInstance.Update();
	GameFrameworkInstance.Render(dc, childViewRect);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TimerID_UpdateTimer)
	{
		// Invalidate the view so it will redraw
		this->Invalidate(TRUE);
	}
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	GameInputInstance.OnKeyDown(nChar, nRepCnt);
}

void CChildView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	GameInputInstance.OnKeyUp(nChar, nRepCnt);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	GameInputInstance.OnLButtonDown(Vector2i(point.x, point.y));
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	GameInputInstance.OnLButtonUp(Vector2i(point.x, point.y));
}

void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	GameInputInstance.OnLButtonDblClk(Vector2i(point.x, point.y));
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	GameInputInstance.OnRButtonDown(Vector2i(point.x, point.y));
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
	GameInputInstance.OnRButtonUp(Vector2i(point.x, point.y));
}

void CChildView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	GameInputInstance.OnRButtonDblClk(Vector2i(point.x, point.y));
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	GameInputInstance.OnMouseMove(Vector2i(point.x, point.y));
}

void CChildView::OnDestroy()
{
	GameFrameworkInstance.EndPlay();
}