#pragma once
#include "stdafx.h"
#include "Common/Singleton.h"
#include "Common/CmdLine.h"
#include "Dialog/ViewDlg.h"


class CAppManager: public CSingleton<CAppManager>
{
	friend CViewDlg;
public:
	explicit CAppManager(HINSTANCE hInstance);
	~CAppManager();
public:
	// 运行入口
	BOOL		Run();
	// 加载皮肤
	BOOL		LoadSkin(LPCTSTR lpcSkin);
	// 加载内置皮肤
	BOOL		LoadDefaultSkin();
	// 重新加载皮肤
	BOOL		RefreshSkin();
	// 新增窗口
	BOOL		PushDlg(CViewDlg* pDlg, BOOL bCheckMain = TRUE);
	// 删除窗口
	BOOL		PopDlg(CViewDlg* pDlg, BOOL bCheckMain = TRUE);
	// 清空窗口并释放
	BOOL		ClearDlg(BOOL bDelete = TRUE);
private:
	TString			m_strSkin;		// 皮肤完整路径
	TString			m_strSkinPath;	// 皮肤目录
	TString			m_AppPath;
	SApplication*	m_AppUI;
	CCmdLine*		m_pCmd;
	HINSTANCE		m_hInstance;	
	CAutoRefPtr<IResProvider> m_pResProvider;		//工程的UI提供者
	std::vector<CViewDlg*>	m_Dlgs;	// Dlg列表
	CViewDlg*		m_dlgMain;		// 主窗口
};

