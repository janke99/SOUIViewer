#include "stdafx.h"
#include "AppManager.h"
#include <ShellAPI.h>
#include "Dialog/ViewDlg.h"
#include "Common/Utils.h"


template<> CAppManager* CSingleton<CAppManager>::ms_Singleton = NULL;

#pragma warning(disable:4996)
CAppManager::CAppManager(HINSTANCE hInstance)
	:m_dlgMain(NULL)
{
	m_hInstance = hInstance;
	m_AppUI = SApplication::getSingletonPtr();
	assert(m_AppUI);

	TCHAR szCurrentDir[MAX_PATH + 1] = { 0 };
	GetCurrentDirectory(MAX_PATH, szCurrentDir);
	LPTSTR lpInsertPos = _tcsrchr(szCurrentDir, _T('\\'));
	_tcscpy(lpInsertPos, _T("\\"));
	m_AppPath = szCurrentDir;
}

CAppManager::~CAppManager()
{
	if (m_pCmd) delete m_pCmd;
}

BOOL CAppManager::Run()
{
	m_pCmd = new CCmdLine(::GetCommandLine());

	LoadDefaultSkin();
	LPCTSTR lpSkin = m_pCmd->GetParam(1);
	if (lpSkin) LoadSkin(lpSkin);

	m_AppUI->Run(m_dlgMain->m_hWnd);

	return TRUE;
}

BOOL CAppManager::LoadSkin(LPCTSTR lpcSkin)
{
	if (!lpcSkin || _tcslen(lpcSkin) == 0) return FALSE;
	TCHAR szBuf[MAX_PATH] = { 0 };
	::PathCombine(szBuf, m_AppPath.c_str(), lpcSkin);
	TString strSkin = szBuf;
	if (strSkin.empty()) return FALSE;

	size_t ps = strSkin.find_last_of(_T("\\"));
	if (ps == TString::npos) return FALSE;
	TString strPath = strSkin.substr(0, ps);

	if (strPath.empty())
		return FALSE;

	// 释放已加载窗口
	for (auto pdlg : m_Dlgs)
	{
		pdlg->DestroyWindow();
		delete pdlg;
	}
	m_Dlgs.clear();
	// 释放ResProvider
	if (m_pResProvider)
	{
		// TODO: 参考SOUIEditer自定义加载
		CAutoRefPtr<SStylePool>		mStylePool; /**<局部style pool*/
		GETSTYLEPOOLMGR->PopStylePool(mStylePool);
		if (mStylePool && mStylePool->GetCount()) mStylePool->RemoveAll();

		CAutoRefPtr<SSkinPool>		pSkinPool;  /**<局部skin pool*/
		GETSKINPOOLMGR->PopSkinPool(pSkinPool);
		if (pSkinPool && pSkinPool->GetCount()) pSkinPool->RemoveAll();

		m_AppUI->RemoveResProvider(m_pResProvider);
	}

	CAutoRefPtr<IResProvider>   pResProvider;
	BOOL	bLoaded = FALSE;
	CreateResProvider(RES_FILE, (IObjRef**)&pResProvider);
	bLoaded = pResProvider->Init((LPARAM)strPath.c_str(), 0);
	SASSERT(bLoaded);

	m_AppUI->AddResProvider(pResProvider);

	pugi::xml_document xmlDoc;
	TString strView = strPath + _T("\\view.xml");
	if (xmlDoc.load_file(strView.c_str(), pugi::parse_default, pugi::encoding_utf8))
	{
		// 加载view.xml定义LAYOUT
		pugi::xml_node xmlRoot = xmlDoc.child(L"root");
		if (!xmlRoot) return FALSE;
		pugi::xml_node xmlView = xmlRoot.first_child();
		while (xmlView)
		{
			const pugi::char_t* pszPath = xmlView.attribute(_T("path")).value();
			CViewDlg* pDlg = new CViewDlg(pszPath, FALSE);
			pDlg->Create(GetActiveWindow());
			pDlg->SendMessage(WM_INITDIALOG);
			pDlg->CenterWindow(pDlg->m_hWnd);
			pDlg->ShowWindow(SW_SHOWNORMAL);
			PushDlg(pDlg);

			xmlView = xmlView.next_sibling();
		}
	}
	else if (xmlDoc.load_file(strSkin.c_str(), pugi::parse_default, pugi::encoding_utf8))
	{
		// 加载第一个layout
		pugi::xml_node xmlNode = xmlDoc.child(_T("resource")).child(_T("LAYOUT")).child(_T("file"));
		if (!xmlNode) return FALSE;
		TString szLayer = Utils::Format(_T("LAYOUT:%s"), xmlNode.attribute(_T("name")).as_string());
		CViewDlg* pDlg = new CViewDlg(szLayer.c_str(), FALSE);
		pDlg->Create(GetActiveWindow());
		pDlg->SendMessage(WM_INITDIALOG);
		pDlg->CenterWindow(pDlg->m_hWnd);
		pDlg->ShowWindow(SW_SHOWNORMAL);
		PushDlg(pDlg);
	}
	else
		return FALSE;

	m_strSkin = strSkin;
	m_strSkinPath = strPath;
	m_pResProvider = pResProvider;

	return TRUE;
}

BOOL CAppManager::LoadDefaultSkin()
{
	CAutoRefPtr<IResProvider>   pResProvider;
	BOOL	bLoaded = FALSE;

#ifdef _DEBUG		
	{
		CreateResProvider(RES_FILE, (IObjRef**)&pResProvider);
		bLoaded = pResProvider->Init((LPARAM)_T("uires"), 0);
		SASSERT(bLoaded);
	}
#else
	{
		CreateResProvider(RES_PE, (IObjRef**)&pResProvider);
		bLoaded = pResProvider->Init((WPARAM)m_hInstance, 0);
		SASSERT(bLoaded);
	}
#endif

	m_AppUI->InitXmlNamedID(namedXmlID, ARRAYSIZE(namedXmlID), TRUE);
	m_AppUI->AddResProvider(pResProvider);

	m_dlgMain = new CViewDlg(_T("LAYOUT:XML_MAINWND"), TRUE);
	m_dlgMain->Create(GetActiveWindow());
	m_dlgMain->SendMessage(WM_INITDIALOG);
	m_dlgMain->CenterWindow(m_dlgMain->m_hWnd);
	m_dlgMain->ShowWindow(SW_SHOWNORMAL);

	return TRUE;
}

BOOL CAppManager::RefreshSkin()
{
	if (m_strSkin.empty())
		return FALSE;
	LoadSkin(m_strSkin.c_str());
	return TRUE;
}

BOOL CAppManager::PushDlg(CViewDlg* pDlg, BOOL bCheckMain)
{
	if (!pDlg) return FALSE;
	m_Dlgs.push_back(pDlg);
	if (bCheckMain && m_dlgMain && m_dlgMain->IsWindow())
		m_dlgMain->ShowWindow(SW_HIDE);
	return TRUE;
}

BOOL CAppManager::PopDlg(CViewDlg* pDlg, BOOL bCheckMain)
{
	BOOL bRet = FALSE;
	for (auto it = m_Dlgs.begin(); it != m_Dlgs.end();)
	{
		if (pDlg == *it)
		{
			m_Dlgs.erase(it);
			bRet = TRUE;
			break;
		}
		++it;
	}
	if (m_Dlgs.size() == 0)
	{
		if (bCheckMain && m_dlgMain && m_dlgMain->IsWindow())
			m_dlgMain->ShowWindow(SW_SHOW);
	}
	return bRet;
}

BOOL CAppManager::ClearDlg(BOOL bDelete /*= TRUE*/)
{
	if (bDelete)
	{
		for (auto pDlg : m_Dlgs)
		{
			pDlg->OnClose();
			delete pDlg;
		}
	}
	m_Dlgs.clear();
	return FALSE;
}

BOOL CAppManager::ShowDlgMain(int nShow)
{
	if (m_dlgMain)
		return m_dlgMain->ShowWindow(nShow);
	return FALSE;
}

