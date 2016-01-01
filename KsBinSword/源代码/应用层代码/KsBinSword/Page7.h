#pragma once
#include "hexedit.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "EditEx.h"
#include "MySystem.h"
// CPage7 对话框

class CPage7 : public CDialog
{
	DECLARE_DYNAMIC(CPage7)

public:
	CPage7(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage7();

// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CHexEdit m_Edit;



	afx_msg void OnBnClickedCheck1();

	afx_msg void OnBnClickedCheck2();

	afx_msg void OnBnClickedCheck3();

	afx_msg void OnBnClickedCheck4();

	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


	CSliderCtrl m_slider;

	CButton m_address;

	CButton m_hex;

	CButton m_ascii;

	CButton m_48;

	afx_msg void OnBnClickedReadSector();

	CEditEx m_Sector;

	CButton m_HexCheck;

	afx_msg void OnBnClickedWriteSector();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	CEditEx m_FilePath;

	afx_msg void OnEnUpdateEdit3();

	CEditEx m_PrintLCN;

	int WriteSectorByDrive(__int64 Sect,BYTE* InBuf);//写一个扇区
	int ReadSectorByDrive(__int64 Sect,BYTE* OutBuf);//读一个扇区
	signed int  WriteSIoctl_4030D0(int this_,  int Writebuf, int a3, int TotalSec_, unsigned __int8 a5);
	signed int  ReadSIoctl_402FF0(int this_, int OutBuf, int StartSec, int TotalSec_, int DiskID_);
	int  SIoctl_402E00(int this_, LPVOID lpInBuffer);
	afx_msg void OnBnClickedReadSectorByDrive();
	afx_msg void OnBnClickedWriteSectorByDrive();
};
