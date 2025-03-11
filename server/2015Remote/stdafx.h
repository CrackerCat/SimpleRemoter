
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#include "common/commands.h"

// ʹ��ѹ���㷨���㷨��Ҫ��clien��stdafx.hƥ��
#define USING_COMPRESS 1

// �Ƿ�ʹ��ZLIB
#define USING_ZLIB 0

#if !USING_ZLIB
// �Ƿ�ʹ��LZ4
#define USING_LZ4 0
#if !USING_LZ4
#define USING_ZSTD 1
#define USING_CTX 0
#endif
#endif

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#ifdef _DEBUG
#define Mprintf(format, ...) printf(format, ##__VA_ARGS__)
#else
#define Mprintf(format, ...) 
#endif

// �Ƴ��Ի�����MFC�ؼ���֧�֣���С��̬�������Ĵ�С
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS

// �رնԻ���ʱɾ���Ի���
#define CLOSE_DELETE_DLG 0

// ����ڴ�й©���谲װVLD��������ע�ʹ���
// �����װ��VLD, �뽫��װ·����ӵ���������: ����Ϊ"VLDPATH", ·��Ϊ"D:\Program Files (x86)\Visual Leak Detector"
// �����ʵ�ʰ�װĿ¼��дVLDPATH. �����ֶ��༭ÿ����Ŀ�ļ���ͷ�ļ�Ŀ¼�Ϳ�Ŀ¼. �й�����VLD�����Ϣ��ο���������.
// VS2017��ǰ�汾��VLD: https://kinddragon.github.io/vld
// VS2019ʹ�õ�VLD��֧��������VS�汾, �Ƽ���: https://github.com/oneiric/vld/releases/tag/v2.7.0
// ���Ҫ���ܿض˳���ŵ�����������������, ��ʹ��Releaseģʽ���ɵĳ���, �Խ����VLD������; ��������Ҫ��VLD����ļ�һͬ����.
// ����VLD��ʾ����й¶������׷�ٲ��˺������ö�ջ�������������ʹ�÷��ŷ������Զ����ء������������Գ����ʱ�������
// ȷ����ĵ��Թ��ߣ��� Visual Studio �� WinDbg�������˷��ŷ�������
// ���ŷ��������Զ�����ȱʧ�ķ����ļ������� dbghelp.pdb�������仺�浽���ط���·����
// ���÷��ŷ��������� Visual Studio Ϊ�������� Visual Studio �У��� ���� > ѡ�� > ���š�
// ��ѡ Microsoft Symbol Servers. ָ�����Ż���Ŀ¼������ "C:\Symbols"��
// ����ʱ��ȱʧ�ķ��ţ��� dbghelp.pdb�����Զ����ص�����Ŀ¼��
#include "vld.h"

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#include <afxdisp.h>        // MFC �Զ�����

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


#define WM_USERTOONLINELIST    WM_USER + 3000
#define WM_OPENSCREENSPYDIALOG WM_USER + 3001
#define WM_OPENFILEMANAGERDIALOG      WM_USER + 3002
#define WM_OPENTALKDIALOG              WM_USER+3003
#define WM_OPENSHELLDIALOG             WM_USER+3004
#define WM_OPENSYSTEMDIALOG            WM_USER+3005
#define WM_OPENAUDIODIALOG             WM_USER+3006
#define WM_OPENSERVICESDIALOG          WM_USER+3007
#define WM_OPENREGISTERDIALOG          WM_USER+3008
#define WM_OPENWEBCAMDIALOG            WM_USER+3009

#define WM_USEROFFLINEMSG				WM_USER+3010
#define WM_HANDLEMESSAGE				WM_USER+3011
#define WM_OPENKEYBOARDDIALOG			WM_USER+3012

enum
{
	FILEMANAGER_DLG = 1,
	SCREENSPY_DLG,
	VIDEO_DLG,
	AUDIO_DLG,
	KEYBOARD_DLG,
	SYSTEM_DLG,
	SHELL_DLG,
	SERVICES_DLG,
	REGISTER_DLG,
	TALK_DLG,
	MONITOR_DLG
};


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#include <assert.h>
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")

// �߾��ȵ�˯�ߺ���
#define Sleep_m(ms) { timeBeginPeriod(1); Sleep(ms); timeEndPeriod(1); }

// �Բ���n����������C�µȴ�T��(n�ǲ���������������1000)
#define WAIT_n(C, T, n) {assert(!(1000%(n)));int s=(1000*(T))/(n);do{Sleep(n);}while((C)&&(--s));}

// ������C����ʱ�ȴ�T��(����10ms)
#define WAIT(C, T) { timeBeginPeriod(1); WAIT_n(C, T, 10); timeEndPeriod(1); }

// ������C����ʱ�ȴ�T��(����1ms)
#define WAIT_1(C, T) { timeBeginPeriod(1); WAIT_n(C, T, 1); timeEndPeriod(1); }

// ���ܼ�ʱ�������㺯���ĺ�ʱ
class auto_tick
{
private:
	const char *func;
	int span;
	clock_t tick;
	__inline clock_t now() const { return clock(); }
	__inline int time() const { return now() - tick; }

public:
	auto_tick(const char *func_name, int th = 5) : func(func_name), span(th), tick(now()) { }
	~auto_tick() { stop(); }

	__inline void stop() {
		if (span != 0) { int s(this->time()); if (s > span)TRACE("[%s]ִ��ʱ��: [%d]ms.\n", func, s); span = 0; }
	}
};

#ifdef _DEBUG
// ���ܼ��㵱ǰ�����ĺ�ʱ����ʱ���ӡ
#define AUTO_TICK(thresh) auto_tick TICK(__FUNCTION__, thresh)
#define STOP_TICK TICK.stop()
#else
#define AUTO_TICK(thresh) 
#define STOP_TICK 
#endif
