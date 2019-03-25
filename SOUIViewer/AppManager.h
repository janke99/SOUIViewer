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
	//
	BOOL Run();

	// 加载皮肤
	BOOL		LoadSkin(LPCTSTR lpcSkin);
	// 加载内置皮肤
	BOOL		LoadDefaultSkin();
	// 重新加载皮肤
	BOOL		ReloadSkin(LPCTSTR lpcSkin = NULL);
	// 设置m_ParentHwnd
	void		SetParentHwnd(HWND hWnd);
private:
	TString			m_strSkin;		// 皮肤完整路径
	TString			m_strSkinPath;	// 皮肤目录
	TString			m_AppPath;
	SApplication*	m_App;
	BOOL			m_bNewSkin;		// 新皮肤加载完成，通过窗口消息通知
	HWND			m_ParentHwnd;	// 调用者句柄
	CCmdLine*		m_pCmd;
	HINSTANCE		m_hInstance;	
};

