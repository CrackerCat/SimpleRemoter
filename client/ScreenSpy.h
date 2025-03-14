// ScreenSpy.h: interface for the CScreenSpy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCREENSPY_H__5F74528D_9ABD_404E_84D2_06C96A0615F4__INCLUDED_)
#define AFX_SCREENSPY_H__5F74528D_9ABD_404E_84D2_06C96A0615F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define COPY_ALL 1	// ����ȫ����Ļ�����ֿ鿽����added by yuanyuanxiang 2019-1-7��
#include "CursorInfo.h"
#include "ScreenCapture.h"


class CScreenSpy  : public ScreenCapture
{
private:
	HWND             m_hDeskTopWnd;      //��ǰ�������Ĵ��ھ��
	HDC              m_hFullDC;          //Explorer.exe �Ĵ����豸DC

	HDC              m_hFullMemDC;
	HBITMAP	         m_BitmapHandle;
	PVOID            m_BitmapData_Full;

	HDC              m_hDiffMemDC;
	HBITMAP	         m_DiffBitmapHandle;
	PVOID            m_DiffBitmapData_Full;

	ULONG            m_RectBufferOffset; // ������λ��

public:
	CScreenSpy(ULONG ulbiBitCount, int gop = DEFAULT_GOP);

	virtual ~CScreenSpy();

	int GetWidth()const {
		return m_ulFullWidth;
	}

	int GetHeight() const {
		return m_ulFullHeight;
	}

	bool IsZoomed() const {
		return m_bZoomed;
	}

	double GetWZoom() const {
		return m_wZoom;
	}

	double GetHZoom() const {
		return m_hZoom;
	}

	const LPBITMAPINFO& GetBIData() const
	{
		return m_BitmapInfor_Full;
	}

	ULONG GetFirstScreenLength() const
	{
		return m_BitmapInfor_Full->bmiHeader.biSizeImage;
	}

	FORCEINLINE VOID WriteRectBuffer(LPBYTE szBuffer, ULONG ulLength)
	{
		memcpy(m_RectBuffer + m_RectBufferOffset, szBuffer, ulLength);
		m_RectBufferOffset += ulLength;
	}

	virtual LPBYTE GetFirstScreenData(ULONG* ulFirstScreenLength);

	virtual LPBYTE ScanNextScreen() {
		ScanScreen(m_hDiffMemDC, m_hFullDC, m_BitmapInfor_Full->bmiHeader.biWidth, m_BitmapInfor_Full->bmiHeader.biHeight);
		return (LPBYTE)m_DiffBitmapData_Full;
	}

	VOID ScanScreen(HDC hdcDest, HDC hdcSour, ULONG ulWidth, ULONG ulHeight);

};

#endif // !defined(AFX_SCREENSPY_H__5F74528D_9ABD_404E_84D2_06C96A0615F4__INCLUDED_)
