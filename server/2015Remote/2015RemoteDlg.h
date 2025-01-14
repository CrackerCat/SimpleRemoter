
// 2015RemoteDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "TrueColorToolBar.h"
#include "IOCPServer.h"

//////////////////////////////////////////////////////////////////////////
// ����Ϊ��������ʹ��

#define INDEPENDENT 1

//////////////////////////////////////////////////////////////////////////

#if INDEPENDENT
// �Ƿ����˳����ض�ʱҲ�˳��ͻ���
#define CLIENT_EXIT_WITH_SERVER 0
#else
#define CLIENT_EXIT_WITH_SERVER 1
#endif 

// CMy2015RemoteDlg �Ի���
class CMy2015RemoteDlg : public CDialogEx
{
	// ����
public:
	CMy2015RemoteDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CMy2015RemoteDlg();
	// �Ի�������
	enum { IDD = IDD_MY2015REMOTE_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void SortByColumn(int nColumn);
	afx_msg VOID OnHdnItemclickList(NMHDR* pNMHDR, LRESULT* pResult);
	static int CALLBACK CompareFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	VOID InitControl();             //��ʼ�ؼ�
	VOID TestOnline();              //���Ժ���
	VOID AddList(CString strIP, CString strAddr, CString strPCName, CString strOS, 
		CString strCPU, CString strVideo, CString strPing, CString ver, CString st, CString tp, CONTEXT_OBJECT* ContextObject);
	VOID ShowMessage(BOOL bOk, CString strMsg);
	VOID CreatStatusBar();
	VOID CreateToolBar();
	VOID CreateNotifyBar();
	VOID CreateSolidMenu();	
	BOOL ListenPort();
	BOOL Activate(int nPort,int nMaxConnection);

	static VOID CALLBACK NotifyProc(CONTEXT_OBJECT* ContextObject);
	static VOID CALLBACK OfflineProc(CONTEXT_OBJECT* ContextObject);
	static VOID MessageHandle(CONTEXT_OBJECT* ContextObject);
	VOID SendSelectedCommand(PBYTE  szBuffer, ULONG ulLength);
	// ��ʾ�û�������Ϣ
	CListCtrl  m_CList_Online;    
	CListCtrl  m_CList_Message;

	CStatusBar m_StatusBar;          //״̬��
	CTrueColorToolBar m_ToolBar;
#if INDEPENDENT
	NOTIFYICONDATA  m_Nid;
#endif
	CRITICAL_SECTION m_cs;
	BOOL       isClosed;

	CBitmap m_bmOnline[3];
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
			void Release();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMRClickOnline(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOnlineMessage();
	afx_msg void OnOnlineDelete();
	afx_msg void OnOnlineUpdate();
	afx_msg void OnAbout();
	afx_msg void OnIconNotify(WPARAM wParam,LPARAM lParam);
	afx_msg void OnNotifyShow();
	afx_msg void OnNotifyExit();
	afx_msg void OnMainSet();
	afx_msg void OnMainExit();
	afx_msg void OnOnlineCmdManager();
	afx_msg void OnOnlineProcessManager();
	afx_msg void OnOnlineWindowManager();
	afx_msg void OnOnlineDesktopManager();
	afx_msg void OnOnlineAudioManager();
	afx_msg void OnOnlineVideoManager();
	afx_msg void OnOnlineFileManager();
	afx_msg void OnOnlineServerManager();
	afx_msg void OnOnlineRegisterManager();
	afx_msg void OnOnlineBuildClient();
	afx_msg LRESULT OnUserToOnlineList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserOfflineMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenScreenSpyDialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenFileManagerDialog(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnOpenTalkDialog(WPARAM wPrarm,LPARAM lParam);
	afx_msg LRESULT OnOpenShellDialog(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnOpenSystemDialog(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnOpenAudioDialog(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnOpenRegisterDialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenServicesDialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenVideoDialog(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
