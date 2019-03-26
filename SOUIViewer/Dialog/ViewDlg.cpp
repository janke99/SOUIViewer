#include "stdafx.h"
#include "ViewDlg.h"
#include "../AppManager.h"

CViewDlg::CViewDlg(LPCTSTR pszResName, BOOL bIsMain)
	:SHostWnd(pszResName)
{
	m_bLayoutInited = FALSE;
	m_bIsMain = bIsMain;
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
	ModifyStyleEx(0, WS_EX_ACCEPTFILES, 0);
	if (!m_bIsMain)
		m_hostAttr.SetAttribute(_T("wndType"), _T("normal"), FALSE);

	m_bLayoutInited = TRUE;
	return 0;
}

void CViewDlg::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
}

void CViewDlg::OnClose()
{
	SetMsgHandled(FALSE);
	CAppManager::getSingleton().PopDlg(this, TRUE);
	if (IsWindow()) CSimpleWnd::DestroyWindow();
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
		SRUNONUI(
			CAppManager::getSingleton().LoadSkin(szPath);
		);
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
		SetMsgHandled(TRUE);
		SRUNONUI(
			CAppManager::getSingleton().RefreshSkin();
		);
	}
}



