#pragma once
#include "afxcmn.h"
#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")
#include "WINIOCTL.H"
#include "MySystem.h"
#include "MyList.h"
#include "EditEx.h"
#define MYCOMPUTER L"My Computer"
#define ILI_CDDRV     0
#define ILI_CLSDFLD   1
#define ILI_DRIVE     2 
#define ILI_FLOPPYDRV 3 
#define ILI_MYCOMP    4
#define ILI_OPENFLD   5 
#define ILI_TEXTFILE  6 

#define IOCTL_MT_GETDIRINF   \
	CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x201, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MT_GETDIRNUMINF   \
	CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x202, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MT_GETIOCOMPLETEREQUESTADDR   \
	CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x203, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MT_GETKERAISEIRQLADDR   \
	CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x204, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MT_REKERAISEIRQL   \
	CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x205, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MT_REIOCOMPLETEREQUEST   \
	CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x206, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MT_KILLFILE   \
	CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x207, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
// CPage6 对话框
typedef struct _DELETE_INFO{
	BOOLEAN  IsDelete;
}DELETE_INFO,*PDELETE_INFO;
class CPage6 : public CDialog
{
	DECLARE_DYNAMIC(CPage6)

public:
	CPage6(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage6();

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_FileTree;
	CListCtrl m_FileList;
	int       iSelected;
	CString path;
public:
	BOOL OnInitDialog();
	void DeleteAllItems();
	CString GetPathFromItem(HTREEITEM hItem);
	BOOL IsMediaValid(CString &strPathName);
	HTREEITEM GetDriveNode(HTREEITEM hItem);
	UINT DeleteChildren(HTREEITEM hItem);
	void AddDummyNode(HTREEITEM hItem);
	BOOL IsPathValid(CString &strPathName);
	afx_msg void OnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL IsDriveNode(HTREEITEM hItem);
	UINT AddDirectoryNodes(HTREEITEM hItem, CString &strPathName);
	void InitTreeView(HTREEITEM hParent);
	BOOL AddDrives(CString strDrive, HTREEITEM hParent);
	BOOL HasSubdirectory(CString &strPathName);
    void SetButtonState(HTREEITEM hItem, CString &strPathName);
	void AddToListView(PDIRECTORY_INFO_EX fd);
	void SetPath(CString str,HANDLE handle);
public:
	
	CImageList* m_pImageList;
	CImageList* m_pImage;
	HANDLE  hDevice;
	CString m_LocalPath;
	ULONG index;
	CMyList   *ProcessList;
public:
	afx_msg void OnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnMenuitemForceKill();
public:
	afx_msg void OnNMRclickList4(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnMenuitemDelete();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
