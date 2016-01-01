// Page6.cpp : 实现文件
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "Page6.h"


// CPage6 对话框

IMPLEMENT_DYNAMIC(CPage6, CDialog)

CPage6::CPage6(CWnd* pParent /*=NULL*/)
	: CDialog(CPage6::IDD, pParent)
{

}

CPage6::~CPage6()
{
}

void CPage6::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_FileTree);
	DDX_Control(pDX, IDC_LIST4, m_FileList);
}


BEGIN_MESSAGE_MAP(CPage6, CDialog)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE1, &CPage6::OnItemexpanding)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE1, &CPage6::OnSelchanging)
	ON_COMMAND(ID_MENU_32775, &CPage6::OnMenuitemForceKill)
	ON_NOTIFY(NM_RCLICK, IDC_LIST4, &CPage6::OnNMRclickList4)
	ON_COMMAND(ID_MENU_32776, &CPage6::OnMenuitemDelete)
END_MESSAGE_MAP()

BOOL CPage6::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CWinApp* pApp = AfxGetApp();
	CMySystem:: LoadDriver(L"Explorer");

	index=0;
    m_pImageList = new CImageList();
	m_pImage = new CImageList();
	m_FileList.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	ASSERT(m_pImage != NULL);    // serious allocation failure checking
	m_FileList.SetImageList(m_pImage, LVSIL_SMALL);	
	m_FileList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	ProcessList->InitSystemImageLists(m_FileList.m_hWnd);//!!!!!!!!!!!!!!!
	m_FileList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CRect rect;
	GetClientRect(&rect);
	m_FileList.InsertColumn(0,L"文件名称",LVCFMT_LEFT,rect.right/6,-1);
	m_FileList.InsertColumn(1,L"占用空间",LVCFMT_RIGHT,rect.right/8,-1);
	m_FileList.InsertColumn(2,L"创建时间",LVCFMT_RIGHT,rect.right/5,-1);
	m_FileList.InsertColumn(3,L"修改时间",LVCFMT_RIGHT,rect.right/5,-1);
	m_FileList.InsertColumn(4,L"进入时间",LVCFMT_RIGHT,rect.right/5,-1);
	m_FileList.InsertColumn(5,L"属性",LVCFMT_CENTER,rect.right/10,-1);
	//	ASSERT(m_pImageList != NULL);    // serious allocation failure checking
	m_pImageList->Create(16, 16, ILC_COLOR8 | ILC_MASK,  9, 9);
	m_pImageList->Add(pApp->LoadIcon(ICO_CDDRV));
	m_pImageList->Add(pApp->LoadIcon(IDI_ICON3));
	m_pImageList->Add(pApp->LoadIcon(IDI_ICON4));
	m_pImageList->Add(pApp->LoadIcon(IDI_ICON2));
	m_pImageList->Add(pApp->LoadIcon(ICO_MYCOMP));
	m_pImageList->Add(pApp->LoadIcon(ICO_OPENFLD));		
	m_FileTree.SetImageList(m_pImageList , TVSIL_NORMAL);
	m_FileTree.ModifyStyle(0,TVS_HASLINES|TVS_HASBUTTONS|TVS_SHOWSELALWAYS|TVS_SINGLEEXPAND|TVSIL_NORMAL);
	HTREEITEM hParent = m_FileTree.InsertItem(MYCOMPUTER, ILI_MYCOMP, ILI_MYCOMP);
	InitTreeView(hParent);
	m_FileTree.Expand(hParent, TVE_EXPAND); 
_Again:
	hDevice = CreateFile( L"\\\\.\\explorer",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	if ((HANDLE)-1==hDevice||(HANDLE)NULL==hDevice)
	{

		if(IDYES==::MessageBox(0, L"系统繁忙，加载文件管理驱动失败,需要重新加载吗？\n如果不加载，某些功能将无法使用！",L"警告",MB_YESNO))
		{
			CMySystem:: UnloadDriver(L"Explorer");
			CMySystem:: LoadDriver(L"Explorer");
			goto _Again;
		}

		return false;

	}
	return true;
}
HBRUSH CPage6::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	CBrush* brBkgnd = new CBrush();
	brBkgnd->CreateSolidBrush( RGB(250, 249, 250) );
	hbr = *brBkgnd;                // Control bkgnd


	return hbr;
}
void CPage6::InitTreeView(HTREEITEM hParent)
{
	int nPos = 0;
	UINT nCount = 0;
	CString strDrive = L"?:\\";

	DWORD dwDriveList = ::GetLogicalDrives ();

	CString cTmp;

	while (dwDriveList) {
		if (dwDriveList & 1) {
			cTmp = strDrive;
			strDrive.SetAt (0, 0x41 + nPos);
			if (AddDrives(strDrive , hParent))
				nCount++;
		}
		dwDriveList >>= 1;
		nPos++;
	}
	return;
}

BOOL CPage6::AddDrives(CString strDrive, HTREEITEM hParent)
{
	HTREEITEM hItem;
	UINT nType = ::GetDriveType ((LPCTSTR) strDrive);
	UINT nDrive = (UINT) strDrive[0] - 0x41;

	switch (nType) {

	case DRIVE_REMOVABLE:
		strDrive=strDrive;
		hItem = m_FileTree.InsertItem(strDrive, ILI_FLOPPYDRV, ILI_FLOPPYDRV, hParent);
		if (HasSubdirectory (strDrive))
		{
			AddDummyNode(hItem);
		}
		break;

	case DRIVE_FIXED:
		strDrive=strDrive;
		hItem = m_FileTree.InsertItem(strDrive,  ILI_DRIVE, ILI_DRIVE, hParent);
		if (HasSubdirectory (strDrive))
		{
			AddDummyNode(hItem);
		}
		break;

	case DRIVE_REMOTE:
		hItem = m_FileTree.InsertItem(strDrive, ILI_DRIVE, ILI_DRIVE, hParent);
		if (HasSubdirectory (strDrive))
		{
			AddDummyNode(hItem);
		}
		break;

	case DRIVE_CDROM:
		strDrive=strDrive;
		hItem = m_FileTree.InsertItem(strDrive, ILI_CDDRV, ILI_CDDRV, hParent);
		if (HasSubdirectory (strDrive))
		{
			AddDummyNode(hItem);
		}
		break;

	case DRIVE_RAMDISK:
		hItem = m_FileTree.InsertItem(strDrive, ILI_CDDRV, ILI_CDDRV, hParent);
		if (HasSubdirectory (strDrive))
		{
			AddDummyNode(hItem);
		}
		break;

	default:
		return FALSE;
	}

	return true;
}
BOOL CPage6::HasSubdirectory(CString &strPathName)
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	BOOL bResult = FALSE;
	UINT uDriveType = GetDriveType(strPathName);
	if (DRIVE_FIXED !=uDriveType)
	{
		return bResult;
	}
	if ("A:\\" == strPathName)
	{
		    return bResult;
	}

	CString strFileSpec = strPathName;
	if (strFileSpec.Right (1) != "\\")
		strFileSpec += "\\";
	strFileSpec += "*.*";

	if ((hFind = ::FindFirstFile ((LPCTSTR) strFileSpec, &fd)) !=
		INVALID_HANDLE_VALUE) {
			do {
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					CString strFileName = (LPCTSTR) &fd.cFileName;
					if ((strFileName != ".") && (strFileName != ".."))
						bResult = TRUE;
				}
			} while (::FindNextFile (hFind, &fd) && !bResult);
			::FindClose (hFind);
	}

	return bResult;
}
CString CPage6::GetPathFromItem(HTREEITEM hItem)
{


	CString strPathName;
	while (hItem != NULL) 
	{
		CString string = m_FileTree.GetItemText (hItem);
		if ((string.Right (1) != "\\") && !strPathName.IsEmpty ())
			string += "\\";
		strPathName = string + strPathName;
		hItem = m_FileTree.GetParentItem (hItem);
	}


	if(strPathName.Left(11) == MYCOMPUTER && strPathName.GetLength() > 11)
		strPathName = strPathName.Mid(12);

	return strPathName;

}

// CPage6 消息处理程序
BOOL CPage6::IsDriveNode(HTREEITEM hItem)
{

	return (m_FileTree.GetParentItem (hItem) == NULL) ? TRUE : FALSE;
}
BOOL CPage6::IsPathValid(CString &strPathName)
{

	if (strPathName.GetLength () == 3)
		return TRUE;

	HANDLE hFind;
	WIN32_FIND_DATA fd;
	BOOL bResult = FALSE;

	if ((hFind = ::FindFirstFile ((LPCTSTR) strPathName, &fd)) !=
		INVALID_HANDLE_VALUE) {
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				bResult = TRUE;
			::CloseHandle (hFind);
	}
	return bResult;
}
void CPage6::AddDummyNode(HTREEITEM hItem)
{
	m_FileTree.InsertItem (L"", 0, 0, hItem);
}
UINT CPage6::DeleteChildren(HTREEITEM hItem)
{

	UINT nCount = 0;
	HTREEITEM hChild = m_FileTree.GetChildItem (hItem);

	while (hChild != NULL) {
		HTREEITEM hNextItem = m_FileTree.GetNextSiblingItem (hChild);
		m_FileTree.DeleteItem (hChild);
		hChild = hNextItem;
		nCount++;

		//		dwTreeItemCount--;

	}
	return nCount;

}
HTREEITEM CPage6::GetDriveNode(HTREEITEM hItem)
{

	HTREEITEM hParent;
	do {
		hParent = m_FileTree.GetParentItem (hItem);
		if (hParent != NULL)
			hItem = hParent;
	} while (hParent != NULL);
	return hItem;

}
BOOL CPage6::IsMediaValid(CString &strPathName)
{

	UINT nDriveType = GetDriveType ((LPCTSTR) strPathName);
	if ((nDriveType != DRIVE_REMOVABLE) && (nDriveType != DRIVE_CDROM))
		return TRUE;
	else
		return FALSE;
}
void CPage6::OnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	CString strPathName = GetPathFromItem (hItem);

	if (!IsMediaValid (strPathName)) 
	{
		HTREEITEM hRoot = GetDriveNode (hItem);
		m_FileTree.Expand (hRoot, TVE_COLLAPSE);
		DeleteChildren (hRoot);
		AddDummyNode (hRoot);
		*pResult = TRUE;
		return;
	}


	if (!IsPathValid (strPathName)) 
	{
		if(strPathName != MYCOMPUTER && strPathName != "")
		{
			m_FileTree.DeleteItem (hItem);
			*pResult = TRUE;
			return;
		}
	}

	CWaitCursor wait;
	
	if (pNMTreeView->action == TVE_EXPAND) 
	{
		if(strPathName != MYCOMPUTER)
		{
			DeleteChildren (hItem);
			if (!AddDirectoryNodes (hItem, strPathName))
				*pResult = TRUE;
		}
	}
	else {
		if(strPathName != MYCOMPUTER)
		{
			DeleteChildren (hItem);
			if (IsDriveNode (hItem))
				AddDummyNode (hItem);
			else
				SetButtonState (hItem, strPathName);
		}
	}

	m_LocalPath = strPathName;


	*pResult = 0;
}
void CPage6::SetButtonState(HTREEITEM hItem, CString &strPathName)
{
	//if (HasSubdirectory (strPathName))
		AddDummyNode (hItem);
}

UINT CPage6::AddDirectoryNodes(HTREEITEM hItem, CString &strPathName)
{
	WCHAR wBuf[60];
	DWORD    bytesReturned=0;
	ULONG   num=0;
	PDIRECTORY_INFO   temp={0};
	DIRECTORY_INFO_EX  DIRECTORY_INFO_b;
	CString str,str1,strFileSpec = strPathName;
	if (strFileSpec.Right (1) != "\\")
		strFileSpec += "\\";
	char  a[MAX_PATH];
	
	str1=strFileSpec;
	CMySystem::WCHARToChar(a,strFileSpec.GetBuffer(strFileSpec.GetLength()));

	strFileSpec += "*.*";
	DeviceIoControl(hDevice,(DWORD)IOCTL_MT_GETDIRNUMINF,a,sizeof(a),&num,sizeof(ULONG),&bytesReturned,NULL);
	if(num==0)
	{
		AfxMessageBox(L"驱动未加载，列举出错！");
		return 0;
	}
	temp=(PDIRECTORY_INFO)calloc(num,sizeof(DIRECTORY_INFO));
	if(temp==NULL)
	{

		return 0;
	}
	DeviceIoControl(hDevice,(DWORD)IOCTL_MT_GETDIRINF,a,sizeof(a),temp,num*sizeof(DIRECTORY_INFO),&bytesReturned,NULL);	
	CWaitCursor wait;
	WCHAR wTemp[MAX_PATH]={'\0'};
	m_FileList.DeleteAllItems();
   	index=0;
	SetPath(str1,hDevice);
	for(ULONG i=0;i<num;i++)
	{
		TRACE("AddDirectoryNode:%d\n",i);
		CMySystem::CharToWCHAR(wTemp,temp[i].FileName);
		str.Format(L"%s",wTemp);
		str=str1+str;
		CString strFileName = (LPCTSTR) &temp[i].FileName;
		if(PathIsDirectory(str))
		{
			if(strcmp(temp[i].FileName,"."))
			{
				if(strcmp(temp[i].FileName,".."))
				{
					CMySystem::CharToWCHAR(wTemp,temp[i].FileName);
					HTREEITEM hChild =
						m_FileTree.InsertItem ((LPCTSTR) wTemp,//&fd.cFileName,
						ILI_CLSDFLD , ILI_OPENFLD , hItem , TVI_SORT);

					CString strNewPathName = strPathName;
					if (strNewPathName.Right (1) != "\\")
					{strNewPathName += "\\";}
					CMySystem::CharToWCHAR(wBuf,temp[i].FileName);

					strNewPathName += wBuf;//fd.cFileName;
					SetButtonState (hChild, strNewPathName);	
					
				}

			}
		}
		else
		{
			DIRECTORY_INFO_b.DirectoryInfo=temp[i];
			DIRECTORY_INFO_b.path=str1;
		
			AddToListView(&DIRECTORY_INFO_b);	
		}
	} 
	delete temp;
	return num;
}

void CPage6::AddToListView(PDIRECTORY_INFO_EX fd)
{
	CString  str,temp;

	WCHAR wTemp[MAX_PATH]={'\0'};
	CMySystem::CharToWCHAR(wTemp,fd->DirectoryInfo.FileName);
	str.Format(L"%s",wTemp);
	str=fd->path+str;
	CMySystem::CharToWCHAR(wTemp,fd->DirectoryInfo.FileName);
	m_FileList.InsertItem(index, wTemp);
	temp.Format(L"%d-%d-%d",fd->DirectoryInfo.CreationTime.Year,fd->DirectoryInfo.CreationTime.Month,fd->DirectoryInfo.CreationTime.Day);
	m_FileList.SetItemText(index,2,temp);
	if(fd->DirectoryInfo.AllocationSize.QuadPart>1024*1024*1024)
	{
		temp.Format(L"%0.2fG",(float)(fd->DirectoryInfo.AllocationSize.QuadPart)/(float)(1024*1024*1024));
	}
	else if(fd->DirectoryInfo.AllocationSize.QuadPart>1024*1024)
	{
		temp.Format(L"%0.2fM",(float)(fd->DirectoryInfo.AllocationSize.QuadPart)/(float)(1024*1024));
	}
	else if(fd->DirectoryInfo.AllocationSize.QuadPart>1024)
	{
		temp.Format(L"%0.2fK",(float)(fd->DirectoryInfo.AllocationSize.QuadPart)/(float)(1024));
	}
	else
	{
		temp.Format(L"%ldB",fd->DirectoryInfo.AllocationSize.QuadPart);
	}
	m_FileList.SetItemText(index,1,temp);
	temp.Format(L"%d-%d-%d",fd->DirectoryInfo.LastWriteTime.Year,fd->DirectoryInfo.LastWriteTime.Month,fd->DirectoryInfo.LastWriteTime.Day);
	m_FileList.SetItemText(index,3,temp);
	temp.Format(L"%d-%d-%d",fd->DirectoryInfo.LastAccessTime.Year,fd->DirectoryInfo.LastAccessTime.Month,fd->DirectoryInfo.LastAccessTime.Day);
	m_FileList.SetItemText(index,4,temp);
	switch(fd->DirectoryInfo.FileAttributes)
	{
	case 0x00000001:
		m_FileList.SetItemText(index,5,L"只读");
		break;
	case 0x00000002:
		m_FileList.SetItemText(index,5,L"隐藏");
		break;
	case 0x00000004:
		m_FileList.SetItemText(index,5,L"系统");
		break;
	case 0x00000020:
		m_FileList.SetItemText(index,5,L"存档");
		break;
	case 0x00000080:
		m_FileList.SetItemText(index,5,L"正常");
		break;
	case 0x00000100:
		m_FileList.SetItemText(index,5,L"临时");
		break;
	case 0x00000800:
		m_FileList.SetItemText(index,5,L"压缩");
		break;
	}
	CMySystem::CharToWCHAR(wTemp,fd->DirectoryInfo.FileName);
	m_FileList.SetItem(index, 0, LVIF_TEXT | LVIF_IMAGE,wTemp, 
	ProcessList->GetFileIcon(str), 0, 0, 0);
//		m_FileList.SetItem(index, 0, LVIF_TEXT | LVIF_IMAGE,wTemp, 
//		0, 0, 0, 0);
	//SetPath(str);
	index++;
}
void CPage6::OnSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
/*
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	HTREEITEM hItem = pNMTreeView->itemNew.hItem;

	CString strPathName = GetPathFromItem (hItem);
	*pResult = FALSE;

	if(strPathName == MYCOMPUTER)
		return;

	CWaitCursor wait;

	if (!AddDirectoryNodes (hItem, strPathName))
		*pResult = TRUE;


	m_LocalPath = strPathName;
	*pResult = 0;*/

}

void CPage6::OnMenuitemForceKill()
{

	DELETE_INFO delete_info;
	if(iSelected < 0 || iSelected >= m_FileList.GetItemCount())
		return;
	WCHAR    name[255];char szName[255];
	m_FileList.GetItemText(iSelected,0, name, 255);
	CString   str,temp;
	str.Format(L"%s",name);
	str=path+str;
	temp=L"确定删除"+str+L"吗?";
	if(MessageBox(temp,L"提示", MB_OKCANCEL) != IDOK)
		return;
	wcscpy_s(name,str.GetBuffer(str.GetLength()));

    CMySystem::WCHARToChar(szName,name);

	ULONG   bytesReturned;
	DeviceIoControl(hDevice,(DWORD)IOCTL_MT_KILLFILE,szName,sizeof(szName),&delete_info,sizeof(delete_info),&bytesReturned,NULL);//
	
	if(!delete_info.IsDelete)
	{
		AfxMessageBox(L"删除失败!");
	}
	else
	{
		m_FileList.DeleteItem(iSelected);
	}
}

void CPage6::OnNMRclickList4(NMHDR *pNMHDR, LRESULT *pResult)
{
	iSelected = m_FileList.GetNextItem(-1, LVNI_SELECTED);
	POINT point;
	::GetCursorPos(&point);
	CMenu menu;
	menu.LoadMenu(IDR_MENU3);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	*pResult = 0;
}

void CPage6::OnMenuitemDelete()
{
	if(iSelected < 0 || iSelected >= m_FileList.GetItemCount())
		return;
	WCHAR    name[255];
	m_FileList.GetItemText(iSelected,0, name, 255);
	CString   str,temp;
	str.Format(L"%s",name);
	str=path+str;
	temp=L"确定删除"+str+L"吗?";
	if(MessageBox(temp,L"提示", MB_OKCANCEL) != IDOK)
		return;
	//str.Format("%s",name);
	if(!DeleteFile(str))
	{
		AfxMessageBox(L"删除失败!");
	}
	else
	{
		m_FileList.DeleteItem(iSelected);
	}
}
void CPage6::SetPath(CString str,HANDLE handle)
{
	path=str;
	hDevice=handle;
}
