#pragma once

class CViewDlg: public SHostWnd
{
public:
	explicit CViewDlg(LPCTSTR pszResName, BOOL bIsMain = FALSE);
	~CViewDlg();

	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnSize(UINT nType, CSize size);
	void OnDropFiles(HDROP hDropInfo);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	// 遍历查找Class
	void FindChildByClass(SArray<SWindow*> &pList, SWindow* pWindow, LPCTSTR pszClass, int nDeep = -1);
protected:
	virtual void OnFinalMessage(HWND hWnd);
protected:
	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
	EVENT_MAP_END()

	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CViewDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
		MSG_WM_DROPFILES(OnDropFiles)
		MSG_WM_KEYDOWN(OnKeyDown)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
private:
	BOOL			m_bLayoutInited;
	BOOL			m_bIsMain;
};

