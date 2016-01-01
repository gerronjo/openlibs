// MyList.cpp : implementation file
//

//#include "stdafx.h"
//#include "MTrickster.h"
#include "MyList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyList

CMyList::CMyList()
{
}

CMyList::~CMyList()
{
}


BEGIN_MESSAGE_MAP(CMyList, CListCtrl)
	//{{AFX_MSG_MAP(CMyList)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	//ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyList message handlers
void CMyList::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
// TODO: Add your control notification handler code here
	*pResult = CDRF_DODEFAULT;
	NMLVCUSTOMDRAW * lplvdr=(NMLVCUSTOMDRAW*)pNMHDR;
	NMCUSTOMDRAW &nmcd = lplvdr->nmcd;
	
	switch(lplvdr->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:		
			*pResult = CDRF_NOTIFYITEMDRAW;
			break;		
	case CDDS_ITEMPREPAINT:
		{
			COLORREF ItemColor;
			if( MapItemColor.Lookup(nmcd.dwItemSpec, ItemColor))
			{
				lplvdr->clrText = ItemColor;
				*pResult = CDRF_DODEFAULT;
			}
		}
		break;
	}
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

    HIMAGELIST himlSmall = (HIMAGELIST)::SHGetFileInfo( "C:\\", 0, &sfi, 
		sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON );

    HIMAGELIST himlLarge = (HIMAGELIST)::SHGetFileInfo( "C:\\", 0, &sfi, 
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
/*void CMyList::OnColumnClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	const int iColumn = pNMListView->iSubItem;
    MessageBox("ok");
	// if it's a second click on the same column then reverse the sort order,
	// otherwise sort the new column in ascending order.
	//Sort( iColumn, iColumn == m_iSortColumn ? !m_bSortAscending : TRUE );

	*pResult = 0;
}*/