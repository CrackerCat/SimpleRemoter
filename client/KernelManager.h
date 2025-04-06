// KernelManager.h: interface for the CKernelManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KERNELMANAGER_H__B1186DC0_E4D7_4D1A_A8B8_08A01B87B89E__INCLUDED_)
#define AFX_KERNELMANAGER_H__B1186DC0_E4D7_4D1A_A8B8_08A01B87B89E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Manager.h"
#include <vector>

#define MAX_THREADNUM 0x1000>>2

// �߳���Ϣ�ṹ��, ����3����Ա: ����״̬(run)�����(h)��ͨѶ�ͻ���(p).
struct ThreadInfo
{
	BOOL run;
	HANDLE h;
	IOCPClient *p;
	void* user;
	CONNECT_ADDRESS* conn;
	ThreadInfo() : run(TRUE), h(NULL), p(NULL), user(nullptr), conn(nullptr){ }
};

class CKernelManager : public CManager  
{
public:
	CONNECT_ADDRESS* m_conn;
	HINSTANCE m_hInstance;
	CKernelManager(CONNECT_ADDRESS* conn, IOCPClient* ClientObject, HINSTANCE hInstance);
	virtual ~CKernelManager();
	VOID OnReceive(PBYTE szBuffer, ULONG ulLength);

	ThreadInfo  m_hThread[MAX_THREADNUM];
	// ��ֵ��ԭ�����������ڼ�¼�߳����������߳�������������ʱm_hThread��Խ������³����쳣
	// ����ҽ���ֵ�ĺ����޸�Ϊ"�����߳��±�"����������m_hThread����ָλ�ÿ��ã��������µ��̷߳����ڸ�λ��
	ULONG   m_ulThreadCount;
	UINT	GetAvailableIndex();
};

#endif // !defined(AFX_KERNELMANAGER_H__B1186DC0_E4D7_4D1A_A8B8_08A01B87B89E__INCLUDED_)
