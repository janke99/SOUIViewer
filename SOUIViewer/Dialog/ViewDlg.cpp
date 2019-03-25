#include "stdafx.h"
#include "ViewDlg.h"
#include "../AppManager.h"


CViewDlg::CViewDlg(LPCTSTR pszResName)
	:SHostWnd(pszResName)
{
	m_bLayoutInited = FALSE;
}

CViewDlg::~CViewDlg()
{
}

int CViewDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL CViewDlg::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	LONG lExStyle = GetWindowLong(GWL_EXSTYLE);
	lExStyle |= WS_EX_ACCEPTFILES;
	SetWindowLong(GWL_EXSTYLE, lExStyle);

	m_bLayoutInited = TRUE;
	return 0;
}

void CViewDlg::OnClose()
{
	CSimpleWnd::DestroyWindow();
}

void CViewDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}

void CViewDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}

void CViewDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CViewDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	if (!m_bLayoutInited) return;

	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if (!pBtnMax || !pBtnRestore) return;

	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

void CViewDlg::OnDropFiles(HDROP hDropInfo)
{
	SetMsgHandled(FALSE);
	TCHAR szPath[_MAX_PATH];
	UINT uCount = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (uCount > 0)
	{
		// 只取第一个
		::DragQueryFile(hDropInfo, 0, szPath, _MAX_PATH);
		CAppManager::getSingleton().ReloadSkin(szPath);
	}
	::DragFinish(hDropInfo);
}

void CViewDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SetMsgHandled(FALSE);
	if (nChar == VK_DELETE)
	{
		PostQuitMessage(0);
	}
	else if (nChar == VK_F5)
	{
		CAppManager::getSingleton().ReloadSkin();
	}
}

LRESULT CViewDlg::OnLoadSkin(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	CAppManager::getSingleton().m_bNewSkin = TRUE;
	return 0;
}
