// SettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "2015Remote.h"
#include "SettingDlg.h"
#include "afxdialogex.h"
#include "client/CursorInfo.h"


// CSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CSettingDlg, CDialog)

CSettingDlg::CSettingDlg(CWnd* pParent)
	: CDialog(CSettingDlg::IDD, pParent)
	, m_nListenPort(0)
	, m_nMax_Connect(0)
	, m_sScreenCapture(_T("GDI"))
	, m_sScreenCompress(_T("��Ļ�����㷨"))
{
}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nListenPort);
	DDX_Text(pDX, IDC_EDIT_MAX, m_nMax_Connect);
	DDX_Control(pDX, IDC_BUTTON_SETTINGAPPLY, m_ApplyButton);
	DDX_Control(pDX, IDC_COMBO_SCREEN_CAPTURE, m_ComboScreenCapture);
	DDX_CBString(pDX, IDC_COMBO_SCREEN_CAPTURE, m_sScreenCapture);
	DDV_MaxChars(pDX, m_sScreenCapture, 32);
	DDX_Control(pDX, IDC_COMBO_SCREEN_COMPRESS, m_ComboScreenCompress);
	DDX_CBString(pDX, IDC_COMBO_SCREEN_COMPRESS, m_sScreenCompress);
}

BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETTINGAPPLY, &CSettingDlg::OnBnClickedButtonSettingapply)
	ON_EN_CHANGE(IDC_EDIT_PORT, &CSettingDlg::OnEnChangeEditPort)
	ON_EN_CHANGE(IDC_EDIT_MAX, &CSettingDlg::OnEnChangeEditMax)
END_MESSAGE_MAP()


// CSettingDlg ��Ϣ�������


BOOL CSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int nPort = ((CMy2015RemoteApp*)AfxGetApp())->m_iniFile.GetInt("settings", "ghost");         
	//��ȡini �ļ��еļ����˿�
	int nMaxConnection = ((CMy2015RemoteApp*)AfxGetApp())->m_iniFile.GetInt("settings", "MaxConnection");    

	int DXGI = ((CMy2015RemoteApp*)AfxGetApp())->m_iniFile.GetInt("settings", "DXGI");

	CString algo = ((CMy2015RemoteApp*)AfxGetApp())->m_iniFile.GetStr("settings", "ScreenCompress", "");

	m_nListenPort = (nPort<=0 || nPort>65535) ? 6543 : nPort;
	m_nMax_Connect  = nMaxConnection<=0 ? 10000 : nMaxConnection;

	int n = algo.IsEmpty() ? ALGORITHM_DIFF : atoi(algo.GetString());
	switch (n)
	{
	case ALGORITHM_GRAY:
		m_sScreenCompress = "�Ҷ�ͼ����";
		break;
	case ALGORITHM_DIFF:
		m_sScreenCompress = "��Ļ�����㷨";
		break;
	case ALGORITHM_H264:
		m_sScreenCompress = "H264ѹ���㷨";
		break;
	default:
		break;
	}
	m_ComboScreenCompress.InsertString(ALGORITHM_GRAY, "�Ҷ�ͼ����");
	m_ComboScreenCompress.InsertString(ALGORITHM_DIFF, "��Ļ�����㷨");
	m_ComboScreenCompress.InsertString(ALGORITHM_H264, "H264ѹ���㷨");

	m_ComboScreenCapture.InsertString(0, "GDI");
	m_ComboScreenCapture.InsertString(1, "DXGI");
	m_sScreenCapture = DXGI ? "DXGI" : "GDI";

	UpdateData(FALSE);

	return TRUE; 
}


void CSettingDlg::OnBnClickedButtonSettingapply()
{
	UpdateData(TRUE);
	((CMy2015RemoteApp *)AfxGetApp())->m_iniFile.SetInt("settings", "ghost", m_nListenPort);      
	//��ini�ļ���д��ֵ
	((CMy2015RemoteApp *)AfxGetApp())->m_iniFile.SetInt("settings", "MaxConnection", m_nMax_Connect);

	int n = m_ComboScreenCapture.GetCurSel();
	((CMy2015RemoteApp*)AfxGetApp())->m_iniFile.SetInt("settings", "DXGI", n);

	n = m_ComboScreenCompress.GetCurSel();
	((CMy2015RemoteApp*)AfxGetApp())->m_iniFile.SetInt("settings", "ScreenCompress", n);

	m_ApplyButton.EnableWindow(FALSE);
	m_ApplyButton.ShowWindow(SW_HIDE);
}


void CSettingDlg::OnEnChangeEditPort()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	// ��Button��ӱ���
	m_ApplyButton.ShowWindow(SW_NORMAL);
	m_ApplyButton.EnableWindow(TRUE);
}

void CSettingDlg::OnEnChangeEditMax()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	HWND hApplyButton = ::GetDlgItem(m_hWnd,IDC_BUTTON_SETTINGAPPLY);

	::ShowWindow(hApplyButton,SW_NORMAL);
	::EnableWindow(hApplyButton,TRUE);
}


void CSettingDlg::OnOK()
{
	OnBnClickedButtonSettingapply();

	CDialog::OnOK();
}
