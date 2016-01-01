#pragma once
#include "afxwin.h"

#include "MySystem.h"
#include<windows.h>
#include<stdio.h>
#include<ImageHlp.h>
#include<time.h>
#include"EditEx.h"
#pragma comment(lib,"ImageHlp")
// CPage9 对话框
typedef struct _MAP_FILE_STRUCT
{
	HANDLE hFile;
	HANDLE hMapping;
	LPVOID ImageBase;
} MAP_FILE_STRUCT,* PMAP_FILE_STRUCT;



class CPage9 : public CDialog
{
	DECLARE_DYNAMIC(CPage9)

public:
	CPage9(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage9();

// 对话框数据
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	CEditEx m_PePrint;
	MAP_FILE_STRUCT MapFile;
	CString PrintEdit;
	CString PrintEditTem;
	void printET();
	void printIAT();
	void PrintSectionInfo();
	void PrintOptionAlHeader(PIMAGE_OPTIONAL_HEADER pNt);
	void PrintFileHeader(PIMAGE_FILE_HEADER pFileHeader);
	LPVOID GetDirectoryEntryToData(LPVOID ImageBase,USHORT DirectoryEntry);
	PIMAGE_SECTION_HEADER GetFirstSectionHeader(LPVOID ImageBase);
	PIMAGE_OPTIONAL_HEADER GetOptionalHeader(LPVOID ImageBase);
	PIMAGE_FILE_HEADER   GetFileHeader(LPVOID ImageBase);
	PIMAGE_NT_HEADERS GetNtHeaders(LPVOID ImageBase);
	BOOL IsPEFile(LPVOID ImageBase);
	void UnLoadFile(PMAP_FILE_STRUCT pstMapFile);
	BOOL LoadFile(LPTSTR lpFilename,PMAP_FILE_STRUCT pstMapFile);
	LPVOID RvaToPtr(PIMAGE_NT_HEADERS pNtH,LPVOID ImageBase,DWORD dwRVA);
	void OnDropFiles(HDROP hDropInfo);
public:
	afx_msg void OnBnClickedOk();
public:
	CEditEx m_PEPath;
};
