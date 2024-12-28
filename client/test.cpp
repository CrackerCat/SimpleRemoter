#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <corecrt_io.h>
#include "common/commands.h"

// �Զ�����ע����е�ֵ
#define REG_NAME "a_ghost"

typedef void (*StopRun)();

typedef bool (*IsStoped)();

typedef BOOL (*IsExit)();

// ֹͣ��������
StopRun stop = NULL;

// �Ƿ�ɹ�ֹͣ
IsStoped bStop = NULL;

// �Ƿ��˳����ض�
IsExit bExit = NULL;

BOOL status = 0;

CONNECT_ADDRESS g_ConnectAddress = { FLAG_FINDEN, "", 0, CLIENT_TYPE_DLL };

//����Ȩ��
void DebugPrivilege()
{
	HANDLE hToken = NULL;
	//�򿪵�ǰ���̵ķ�������
	int hRet = OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS,&hToken);

	if( hRet)
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		//ȡ������Ȩ�޵�LUID
		LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		//�����������Ƶ�Ȩ��
		AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),NULL,NULL);

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
BOOL SetSelfStart(const char *sPath, const char *sNmae)
{
	DebugPrivilege();

	// д���ע���·��
#define REGEDIT_PATH "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\"

	// ��ע�����д��������Ϣ
	HKEY hKey = NULL;
	LONG lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, REGEDIT_PATH, 0, KEY_ALL_ACCESS, &hKey);

	// �ж��Ƿ�ɹ�
	if(lRet != ERROR_SUCCESS)
		return FALSE;

	lRet = RegSetValueExA(hKey, sNmae, 0, REG_SZ, (const BYTE*)sPath, strlen(sPath) + 1);

	// �ر�ע���
	RegCloseKey(hKey);

	// �ж��Ƿ�ɹ�
	return lRet == ERROR_SUCCESS;
}

BOOL CALLBACK callback(DWORD CtrlType)
{
	if (CtrlType == CTRL_CLOSE_EVENT)
	{
		status = 1;
		if(stop) stop();
		while(1==status)
			Sleep(20);
	}
	return TRUE;
}

// ���г���.
BOOL Run(const char* argv1, int argv2);

// @brief ���ȶ�ȡsettings.ini�����ļ�����ȡIP�Ͷ˿�.
// [settings] 
// localIp=XXX
// ghost=6688
// ��������ļ������ھʹ��������л�ȡIP�Ͷ˿�.
int main(int argc, const char *argv[])
{
	if(!SetSelfStart(argv[0], REG_NAME))
	{
		std::cout<<"���ÿ���������ʧ�ܣ����ù���ԱȨ������.\n";
	}
	status = 0;
	SetConsoleCtrlHandler(&callback, TRUE);

	do {
		BOOL ret = Run(argc > 1 ? argv[1] : (strlen(g_ConnectAddress.ServerIP()) == 0 ? "127.0.0.1" : g_ConnectAddress.ServerIP()),
			argc > 2 ? atoi(argv[2]) : (g_ConnectAddress.ServerPort() == 0 ? 6543 : g_ConnectAddress.ServerPort()));
		if (ret == 1) {
			return -1;
		}
	} while (status == 0);

	status = 0;
	return -1;
}

// ���������в���: IP �� �˿�.
BOOL Run(const char* argv1, int argv2) {
	BOOL result = FALSE;
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
				std::cerr << "Error deleting file. Error code: " << GetLastError() << std::endl;
				ok = FALSE;
			}
		}
		if (ok && !MoveFileA(path, oldFile.c_str())) {
			std::cerr << "Error removing file. Error code: " << GetLastError() << std::endl;
			ok = FALSE;
		}else {
			// �����ļ�����Ϊ����
			if (SetFileAttributesA(oldFile.c_str(), FILE_ATTRIBUTE_HIDDEN))
			{
				std::cout << "File created and set to hidden: " << oldFile << std::endl;
			}
		}
		if (ok && !MoveFileA(newFile.c_str(), path)) {
			std::cerr << "Error removing file. Error code: " << GetLastError() << std::endl;
			MoveFileA(oldFile.c_str(), path);// recover
		}else if (ok){
			std::cout << "Using new file: " << newFile << std::endl;
		}
	}
	HMODULE hDll = LoadLibraryA(path);
	typedef void (*TestRun)(char* strHost, int nPort);
	TestRun run = hDll ? TestRun(GetProcAddress(hDll, "TestRun")) : NULL;
	stop = hDll ? StopRun(GetProcAddress(hDll, "StopRun")) : NULL;
	bStop = hDll ? IsStoped(GetProcAddress(hDll, "IsStoped")) : NULL;
	bExit = hDll ? IsExit(GetProcAddress(hDll, "IsExit")) : NULL;
	if (run)
	{
		char ip[_MAX_PATH];
		strcpy_s(ip, g_ConnectAddress.ServerIP());
		int port = g_ConnectAddress.ServerPort();
		strcpy(p + 1, "settings.ini");
		if (_access(path, 0) == -1) { // �ļ�������: ���ȴӲ�����ȡֵ������Ǵ�g_ConnectAddressȡֵ.
			strcpy(ip, argv1);
			port = argv2;
		}
		else {
			GetPrivateProfileStringA("settings", "localIp", g_ConnectAddress.ServerIP(), ip, _MAX_PATH, path);
			port = GetPrivateProfileIntA("settings", "ghost", g_ConnectAddress.ServerPort(), path);
		}
		printf("[server] %s:%d\n", ip, port);
		do
		{
			run(ip, port);
			while (bStop && !bStop() && 0 == status)
				Sleep(20);
		} while (bExit && !bExit() && 0 == status);

		while (bStop && !bStop() && 1 == status)
			Sleep(20);
		if (bExit) {
			result = bExit();
		}
		if (!FreeLibrary(hDll)) {
			printf("�ͷŶ�̬���ӿ�\"ServerDll.dll\"ʧ��. �������: %d\n", GetLastError());
		}
		else {
			printf("�ͷŶ�̬���ӿ�\"ServerDll.dll\"�ɹ�!\n");
		}
	}
	else {
		printf("���ض�̬���ӿ�\"ServerDll.dll\"ʧ��. �������: %d\n", GetLastError());
		Sleep(3000);
	}
	return result;
}