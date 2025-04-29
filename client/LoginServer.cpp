#include "StdAfx.h"
#include "LoginServer.h"
#include "Common.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <NTSecAPI.h>
#include "common/skCrypter.h"

// by ChatGPT
bool IsWindows11() {
	typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
	RTL_OSVERSIONINFOW rovi = { 0 };
	rovi.dwOSVersionInfoSize = sizeof(rovi);

	HMODULE hMod = GetModuleHandleW(L"ntdll.dll");
	if (hMod) {
		RtlGetVersionPtr rtlGetVersion = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
		if (rtlGetVersion) {
			rtlGetVersion(&rovi);
			return (rovi.dwMajorVersion == 10 && rovi.dwMinorVersion == 0 && rovi.dwBuildNumber >= 22000);
		}
	}
	return false;
}

/************************************************************************
--------------------- 
���ߣ�IT1995 
��Դ��CSDN 
ԭ�ģ�https://blog.csdn.net/qq78442761/article/details/64440535 
��Ȩ����������Ϊ����ԭ�����£�ת���븽�ϲ������ӣ�
�޸�˵����2019.3.29��Ԭ�����޸�
************************************************************************/
std::string getSystemName()
{
	std::string vname("δ֪����ϵͳ");
	//���ж��Ƿ�Ϊwin8.1��win10
	typedef void(__stdcall*NTPROC)(DWORD*, DWORD*, DWORD*);
	HINSTANCE hinst = LoadLibrary("ntdll.dll");
	if (hinst == NULL)
	{
		return vname;
	}
	if (IsWindows11()) {
		vname = "Windows 11";
		Mprintf("�˵��Եİ汾Ϊ:%s\n", vname.c_str());
		return vname;
	}
	DWORD dwMajor, dwMinor, dwBuildNumber;
	NTPROC proc = (NTPROC)GetProcAddress(hinst, "RtlGetNtVersionNumbers"); 
	if (proc==NULL)
	{
		return vname;
	}
	proc(&dwMajor, &dwMinor, &dwBuildNumber); 
	if (dwMajor == 6 && dwMinor == 3)	//win 8.1
	{
		vname = "Windows 8.1";
		Mprintf("�˵��Եİ汾Ϊ:%s\n", vname.c_str());
		return vname;
	}
	if (dwMajor == 10 && dwMinor == 0)	//win 10
	{
		vname = "Windows 10";
		Mprintf("�˵��Եİ汾Ϊ:%s\n", vname.c_str());
		return vname;
	}
	//���治���ж�Win Server����Ϊ���˻�δ������ϵͳ�Ļ��ӣ���ʱ������

	//�ж�win8.1���µİ汾
	SYSTEM_INFO info;                //��SYSTEM_INFO�ṹ�ж�64λAMD������
	GetSystemInfo(&info);            //����GetSystemInfo�������ṹ
	OSVERSIONINFOEX os;
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if (GetVersionEx((OSVERSIONINFO *)&os))
	{
		//������ݰ汾��Ϣ�жϲ���ϵͳ����
		switch (os.dwMajorVersion)
		{                    //�ж����汾��
		case 4:
			switch (os.dwMinorVersion)
			{                //�жϴΰ汾��
			case 0:
				if (os.dwPlatformId == VER_PLATFORM_WIN32_NT)
					vname ="Windows NT 4.0";  //1996��7�·���
				else if (os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
					vname = "Windows 95";
				break;
			case 10:
				vname ="Windows 98";
				break;
			case 90:
				vname = "Windows Me";
				break;
			}
			break;
		case 5:
			switch (os.dwMinorVersion)
			{               //�ٱȽ�dwMinorVersion��ֵ
			case 0:
				vname = "Windows 2000";    //1999��12�·���
				break;
			case 1:
				vname = "Windows XP";      //2001��8�·���
				break;
			case 2:
				if (os.wProductType == VER_NT_WORKSTATION &&
					info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
					vname = "Windows XP Professional x64 Edition";
				else if (GetSystemMetrics(SM_SERVERR2) == 0)
					vname = "Windows Server 2003";   //2003��3�·���
				else if (GetSystemMetrics(SM_SERVERR2) != 0)
					vname = "Windows Server 2003 R2";
				break;
			}
			break;
		case 6:
			switch (os.dwMinorVersion)
			{
			case 0:
				if (os.wProductType == VER_NT_WORKSTATION)
					vname = "Windows Vista";
				else
					vname = "Windows Server 2008";   //�������汾
				break;
			case 1:
				if (os.wProductType == VER_NT_WORKSTATION)
					vname = "Windows 7";
				else
					vname = "Windows Server 2008 R2";
				break;
			case 2:
				if (os.wProductType == VER_NT_WORKSTATION)
					vname = "Windows 8";
				else
					vname = "Windows Server 2012";
				break;
			}
			break;
		default:
			vname = "δ֪����ϵͳ";
		}
		Mprintf("�˵��Եİ汾Ϊ:%s\n", vname.c_str());
	}
	else
		Mprintf("�汾��ȡʧ��\n");
	return vname;
}

std::string formatTime(const FILETIME& fileTime) {
	// ת��Ϊ 64 λʱ��
	ULARGE_INTEGER ull;
	ull.LowPart = fileTime.dwLowDateTime;
	ull.HighPart = fileTime.dwHighDateTime;

	// ת��Ϊ�뼶ʱ���
	std::time_t startTime = static_cast<std::time_t>((ull.QuadPart / 10000000ULL) - 11644473600ULL);

	// ��ʽ�����
	std::tm* localTime = std::localtime(&startTime);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
	return std::string(buffer);
}

std::string getProcessTime() {
	FILETIME creationTime, exitTime, kernelTime, userTime;

	// ��ȡ��ǰ���̵�ʱ����Ϣ
	if (GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime, &kernelTime, &userTime)) {
		return formatTime(creationTime);
	}
	std::time_t now = std::time(nullptr);
	std::tm* t = std::localtime(&now);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);
	return buffer;
}

LOGIN_INFOR GetLoginInfo(DWORD dwSpeed, const CONNECT_ADDRESS& conn)
{
	LOGIN_INFOR  LoginInfor;
	LoginInfor.bToken = TOKEN_LOGIN; // ����Ϊ��¼
	//��ò���ϵͳ��Ϣ
	strcpy_s(LoginInfor.OsVerInfoEx, getSystemName().c_str());

	//���PCName
	char szPCName[MAX_PATH] = {0};
	gethostname(szPCName, MAX_PATH);  

	DWORD	dwCPUMHz;
	dwCPUMHz = CPUClockMHz();

	BOOL bWebCamIsExist = WebCamIsExist();

	memcpy(LoginInfor.szPCName,szPCName,sizeof(LoginInfor.szPCName));
	LoginInfor.dwSpeed  = dwSpeed;
	LoginInfor.dwCPUMHz = dwCPUMHz;
	LoginInfor.bWebCamIsExist = bWebCamIsExist;
	strcpy_s(LoginInfor.szStartTime, getProcessTime().c_str());
	sprintf_s(LoginInfor.szReserved, "%s", GetClientType(conn.ClientType()));
	bool isDefault = strlen(conn.szFlag) == 0 || strcmp(conn.szFlag, skCrypt(FLAG_GHOST)) == 0 ||
		strcmp(conn.szFlag, skCrypt("Happy New Year!")) == 0;
	std::string masterHash(skCrypt(MASTER_HASH));
	const char* id = isDefault ? masterHash.c_str() : conn.szFlag;
	memcpy(LoginInfor.szMasterID, id, min(strlen(id), 16));

	return LoginInfor;
}


DWORD CPUClockMHz()
{
	HKEY	hKey;
	DWORD	dwCPUMHz;
	DWORD	dwReturn = sizeof(DWORD);
	DWORD	dwType = REG_DWORD;
	RegOpenKey(HKEY_LOCAL_MACHINE, 
		"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", &hKey);
	RegQueryValueEx(hKey, "~MHz", NULL, &dwType, (PBYTE)&dwCPUMHz, &dwReturn);
	RegCloseKey(hKey);
	return	dwCPUMHz;
}

BOOL WebCamIsExist()
{
	BOOL	bOk = FALSE;

	char	szDeviceName[100], szVer[50];
	for (int i = 0; i < 10 && !bOk; ++i)
	{
		bOk = capGetDriverDescription(i, szDeviceName, sizeof(szDeviceName),     
			//ϵͳ��API����
			szVer, sizeof(szVer));
	}
	return bOk;
}
