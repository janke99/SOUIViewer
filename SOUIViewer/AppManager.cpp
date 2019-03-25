#include "stdafx.h"
#include "AppManager.h"
#include <ShellAPI.h>
#include "Dialog/ViewDlg.h"
#include "Common/Utils.h"


template<> CAppManager* CSingleton<CAppManager>::ms_Singleton = NULL;

#pragma warning(disable:4996)
CAppManager::CAppManager(HINSTANCE hInstance)
	:m_ParentHwnd(0)
{
	m_hInstance = hInstance;
	m_App = SApplication::getSingletonPtr();
	assert(m_App);

	TCHAR szCurrentDir[MAX_PATH + 1] = { 0 };
	GetCurrentDirectory(MAX_PATH, szCurrentDir);
	LPTSTR lpInsertPos = _tcsrchr(szCurrentDir, _T('\\'));
	_tcscpy(lpInsertPos, _T("\\"));
	m_AppPath = szCurrentDir;
}

CAppManager::~CAppManager()
{
}

BOOL CAppManager::Run()
{
	m_pCmd = new CCmdLine(::GetCommandLine());
	int nCount = m_pCmd->GetParamCount();
	if (nCount == 1)
	{
		LoadDefaultSkin();
	}
	else if (nCount >= 2)
	{
		if (nCount >= 3)
		{
			LPCTSTR lpParam = m_pCmd->GetParam(2);
			long hParent = 0;
			_stscanf_s(lpParam, _T("%ld"), &hParent);
			m_ParentHwnd = (HWND)hParent;
		}
		LoadSkin(m_pCmd->GetParam(1));
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CAppManager::LoadSkin(LPCTSTR lpcSkin)
{
	if (!lpcSkin || _tcslen(lpcSkin) == 0) return FALSE;
	TCHAR szBuf[MAX_PATH] = { 0 };
	::PathCombine(szBuf, m_AppPath.c_str(), lpcSkin);
	m_strSkin = szBuf;
	if (m_strSkin.empty()) return FALSE;

	size_t ps = m_strSkin.find_last_of(_T("\\"));
	if (ps == TString::npos) return FALSE;

	TString strPath = m_strSkin.substr(0, ps);

	if (strPath.empty())
		return FALSE;
	m_strSkinPath = strPath;

	CAutoRefPtr<IResProvider>   pResProvider;
	BOOL	bLoaded = FALSE;	
	CreateResProvider(RES_FILE, (IObjRef**)&pResProvider);
	bLoaded = pResProvider->Init((LPARAM)m_strSkinPath.c_str(), 0);
	SASSERT(bLoaded);

	m_App->InitXmlNamedID(namedXmlID, ARRAYSIZE(namedXmlID), TRUE);
	m_App->AddResProvider(pResProvider);

	pugi::xml_document xmlDoc;
	TString strView = m_strSkinPath + _T("\\view.xml");
	if (!xmlDoc.load_file(strView.c_str(), pugi::parse_default, pugi::encoding_utf8)) return FALSE;

	pugi::xml_node xmlRoot = xmlDoc.child(L"root");
	if (!xmlRoot) return FALSE;
	pugi::xml_node xmlView = xmlRoot.first_child();
	CViewDlg* pDlgMain = NULL;
	std::vector<CViewDlg*> dlgs;
	while (xmlView)
	{
		const pugi::char_t* pszPath = xmlView.attribute(_T("path")).value();
		CViewDlg* pDlg = new CViewDlg(pszPath);
		if (!pDlgMain) pDlgMain = pDlg;
		pDlg->Create(GetActiveWindow());
		pDlg->SendMessage(WM_INITDIALOG);
		pDlg->CenterWindow(pDlg->m_hWnd);
		pDlg->ShowWindow(SW_SHOWNORMAL);
		dlgs.push_back(pDlg);

		xmlView = xmlView.next_sibling();
	}
	if (!pDlgMain) return FALSE;

	// 通知调用者
	if (::IsWindow(m_ParentHwnd))
		::PostMessage(m_ParentHwnd, MSG_LOADSKIN, 0, 0);
	
	int nRet = m_App->Run(pDlgMain->m_hWnd);
	for (auto pp: dlgs)
	{
		delete pp;
	}
	return TRUE;
}

BOOL CAppManager::LoadDefaultSkin()
{
	CAutoRefPtr<IResProvider>   pResProvider;
	BOOL	bLoaded = FALSE;
#ifdef _DEBUG		
	//选择了仅在Release版本打包资源则在DEBUG下始终使用文件加载
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
	m_App->InitXmlNamedID(namedXmlID, ARRAYSIZE(namedXmlID), TRUE);
	m_App->AddResProvider(pResProvider);

	CViewDlg dlg(_T("LAYOUT:XML_MAINWND"));
	dlg.Create(GetActiveWindow());
	dlg.SendMessage(WM_INITDIALOG);
	dlg.CenterWindow(dlg.m_hWnd);
	dlg.ShowWindow(SW_SHOWNORMAL);
	int nRet = m_App->Run(dlg.m_hWnd);
	return TRUE;
}

BOOL CAppManager::ReloadSkin(LPCTSTR lpcSkin)
{
	if (!lpcSkin && m_strSkin.empty())
		return FALSE;
	LPCTSTR pszNewSkin = lpcSkin ? lpcSkin : m_strSkin.c_str();
	HWND hMain = m_App->GetMainWnd();
	TString strParam = Utils::Format(_T("%s %ld"), pszNewSkin, hMain);
	m_bNewSkin = FALSE;
	::ShowWindow(hMain, SW_HIDE);

	SHELLEXECUTEINFO ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	TCHAR buf[MAX_PATH] = { 0 };
	::GetModuleFileName(0, buf, MAX_PATH);
	ShExecInfo.lpFile = buf;
	ShExecInfo.lpParameters = strParam.c_str();

	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	BOOL bShell = ::ShellExecuteEx(&ShExecInfo);
	if (bShell)
	{
		//WaitForSingleObject(ShExecInfo.hProcess, 2000);
		::CloseHandle(ShExecInfo.hProcess);
	}

	/*if (m_bNewSkin)
		::PostQuitMessage(0);
	else
		::ShowWindow(hMain, SW_SHOW);*/
	::PostQuitMessage(0);

	return TRUE;
}

void CAppManager::SetParentHwnd(HWND hWnd)
{
	m_ParentHwnd = hWnd;
}
