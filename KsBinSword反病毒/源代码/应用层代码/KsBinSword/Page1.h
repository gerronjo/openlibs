#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "EditEx.h"

// CPage1 对话框

class CPage1 : public CDialog
{
	DECLARE_DYNAMIC(CPage1)

public:
	CPage1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage1();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ProListCtrl;
public:
	CImageList m_ProImageList;
public:
	afx_msg void OnBnClickedOk();
	BOOL  OnInitDialog();
public:
	afx_msg void OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnMenuCopyProcessName();
public:
	CListCtrl m_DllListCtrl;
public:
	afx_msg void OnLvnItemActivateList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnLvnItemActivateList1(NMHDR *pNMHDR, LRESULT *pResult);
/*
	typedef struct _UNICODE_STRING {
		USHORT  Length;
		USHORT  MaximumLength;
		PWSTR  Buffer;
	} UNICODE_STRING, *PUNICODE_STRING;

	typedef struct
	{
		UNICODE_STRING SectionFileName;
		WCHAR       NameBuffer[ANYSIZE_ARRAY];
	} MEMORY_SECTION_NAME, *PMEMORY_SECTION_NAME;
	typedef  DWORD (WINAPI *ZwQueryVirtualMemory_)( DWORD a, DWORD b, DWORD c, DWORD d, DWORD f, DWORD e);*/

public:
	int dwProcessId;
public:
	afx_msg void OnNMRclickList2(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnMenuUnmapViewOfModule();
public:
	void ListProcessInfo(void);
public:
	afx_msg void OnMenuKillPro();
public:
	afx_msg void OnMenuFreeModuleByPid();
public:
	void ListProDllByQueryVirMem(void);
	void TrimPath(char szModPath[]);
public:
    HANDLE hMyHandle;
public:
	CListCtrl m_ThreaList;
public:
	void ListThread(void);
	void ListProcessByDrive();
public:
	afx_msg void OnNMRclickList3(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnMenuForceKillThread();
public:
	afx_msg void OnMenuForceKillProcess();
public:
	CStatic m_ProState;
public:
	afx_msg void OnMenuKillProcessByTer();
public:
	afx_msg void OnBnClickedListProcess();
public:
	afx_msg void OnMenuListProcessInfo();
public:
	afx_msg void OnMenuKillThreadByTerminateThread();
};
