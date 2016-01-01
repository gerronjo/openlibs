// MyList.cpp : 实现文件
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "MyList.h"


// CMyList

IMPLEMENT_DYNAMIC(CMyList, CListCtrl)




CMyList::CMyList()
{

}

CMyList::~CMyList()
{
}

void CMyList::SetItemColor(DWORD iItem, COLORREF color)
{
	MapItemColor.SetAt(iItem, color);
	this->RedrawItems(iItem, iItem);
	UpdateWindow();	
}
bool CMyList::InitSystemImageLists(HWND hwndList)
{
	SHFILEINFO sfi;

	HIMAGELIST himlSmall = (HIMAGELIST)::SHGetFileInfo( L"C:\\", 0, &sfi, 
		sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON );

	HIMAGELIST himlLarge = (HIMAGELIST)::SHGetFileInfo( L"C:\\", 0, &sfi, 
		sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_LARGEICON );

	if( himlSmall && himlLarge ) { 
		ListView_SetImageList(hwndList, himlSmall, LVSIL_SMALL);
		ListView_SetImageList(hwndList, himlLarge, LVSIL_NORMAL);
		return TRUE;
	}

	return FALSE;
}
int CMyList::GetFileIcon(LPCTSTR lpFileName)
{
	SHFILEINFO sfi;
	SHGetFileInfo(lpFileName, 0, &sfi, sizeof(sfi), SHGFI_ICON|SHGFI_SMALLICON|SHGFI_LARGEICON);
	return sfi.iIcon;
}

BEGIN_MESSAGE_MAP(CMyList, CListCtrl)
END_MESSAGE_MAP()



// CMyList 消息处理程序


