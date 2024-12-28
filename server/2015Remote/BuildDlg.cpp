// BuildDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "2015Remote.h"
#include "BuildDlg.h"
#include "afxdialogex.h"
#include <io.h>

// CBuildDlg �Ի���

IMPLEMENT_DYNAMIC(CBuildDlg, CDialog)

int MemoryFind(const char *szBuffer, const char *Key, int iBufferSize, int iKeySize);

CBuildDlg::CBuildDlg(CWnd* pParent)
	: CDialog(CBuildDlg::IDD, pParent)
	, m_strIP(_T(""))
	, m_strPort(_T(""))
{

}

CBuildDlg::~CBuildDlg()
{
}

void CBuildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP, m_strIP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_strPort);
	DDX_Control(pDX, IDC_COMBO_EXE, m_ComboExe);
}


BEGIN_MESSAGE_MAP(CBuildDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBuildDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBuildDlg ��Ϣ�������


void CBuildDlg::OnBnClickedOk()
{
	CFile File;
	char szTemp[MAX_PATH];
	ZeroMemory(szTemp,MAX_PATH);
	CString strCurrentPath;
	CString strFile;
	CString strSeverFile;
	BYTE *  szBuffer=NULL;
	DWORD dwFileSize;
	UpdateData(TRUE);
	int index = m_ComboExe.GetCurSel();
	CString file = index == 0 ? "TestRun.exe" : (index == 1 ? "ghost.exe" : "");
	if (file.IsEmpty())
	{
		MessageBox("��Ч�������, ���������ɷ���!");
		return CDialog::OnOK();
	}
	unsigned long flag = index == 0 ? FLAG_FINDEN : (index == 1 ? FLAG_GHOST : FLAG_FINDEN);
	//////////������Ϣ//////////////////////
	CONNECT_ADDRESS g_ConnectAddress = { flag,"",0 };
	strcpy(g_ConnectAddress.szServerIP,m_strIP);  //������IP

	g_ConnectAddress.iPort=atoi(m_strPort);   //�˿�
	if (strlen(m_strIP)==0 || g_ConnectAddress.iPort==0)
		return;
	try
	{
		//�˴��õ�δ����ǰ���ļ���
		char path[_MAX_PATH], *p = path;
		GetModuleFileNameA(NULL, path, sizeof(path));
		while (*p) ++p;
		while ('\\' != *p) --p;
		strcpy(p+1, file.GetString());

		strFile = path; //�õ���ǰδ�����ļ���
		if (_access(path, 0) == -1)
		{
			MessageBox(CString(path) + "\r\n����ģ��\"" + file + "\"������!");
			return CDialog::OnOK();
		}
		
		//���ļ�
		File.Open(strFile,CFile::modeRead|CFile::typeBinary);
		
		dwFileSize=File.GetLength();
		szBuffer=new BYTE[dwFileSize];
		ZeroMemory(szBuffer,dwFileSize);
		//��ȡ�ļ�����
		
		File.Read(szBuffer,dwFileSize);
		File.Close();
		//д������IP�Ͷ˿� ��Ҫ��Ѱ��0x1234567�����ʶȻ��д�����λ��
		int iOffset = MemoryFind((char*)szBuffer,(char*)&g_ConnectAddress.dwFlag,dwFileSize,sizeof(DWORD));
		if (iOffset==-1)
		{
			MessageBox(CString(path) + "\r\n����ģ��\"" + file + "\"��֧��!");
			return;
		}
		memcpy(szBuffer+iOffset,&g_ConnectAddress,sizeof(g_ConnectAddress));
		//���浽�ļ�
		strcpy(p+1, "ClientDemo.exe");
		strSeverFile = path;
		DeleteFileA(path);
		BOOL r=File.Open(strSeverFile,CFile::typeBinary|CFile::modeCreate|CFile::modeWrite);
		if (!r) {
			MessageBox(strSeverFile + "\r\n�������\"" + strSeverFile + "\"����ʧ��!");
			return CDialog::OnOK();
		}
		File.Write(szBuffer,dwFileSize);
		File.Close();
		delete[] szBuffer;
		MessageBox("���ɳɹ�!�ļ�λ��:\r\n"+ strSeverFile);
	}
	catch (CMemoryException* e)
	{
		MessageBox("�ڴ治��!");
	}
	catch (CFileException* e)
	{
		MessageBox("�ļ���������!");
	}
	catch (CException* e)
	{
		MessageBox("δ֪����!");
	}
	CDialog::OnOK();
}

int MemoryFind(const char *szBuffer, const char *Key, int iBufferSize, int iKeySize)   
{   
	int i,j;   
	if (iKeySize == 0||iBufferSize==0)
	{
		return -1;
	}
	for (i = 0; i < iBufferSize; ++i)   
	{   
		for (j = 0; j < iKeySize; j ++)   
			if (szBuffer[i+j] != Key[j])	break;
		if (j == iKeySize) return i;   
	}   
	return -1;   
}


BOOL CBuildDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ComboExe.InsertString(0, "TestRun.exe");
	m_ComboExe.InsertString(1, "ghost.exe");
	m_ComboExe.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
