// ClrListCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "TestList.h"
#include "ClrListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClrListCtrl

CClrListCtrl::CClrListCtrl()
{
	m_ColNum=0;
}

CClrListCtrl::~CClrListCtrl()
{
}


BEGIN_MESSAGE_MAP(CClrListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CClrListCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClrListCtrl message handlers



void CClrListCtrl::MyDrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	if ((int)lpDIS->itemID < 0)
		return; 

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	COLORREF crText;
	CString sText;
	COLORREF crNorm = (COLORREF)lpDIS->itemData;		// Color information is in item data.
	COLORREF crHilite = RGB(255-GetRValue(crNorm), 255-GetGValue(crNorm), 255-GetBValue(crNorm));


	// If item has been selected, draw the highlight rectangle using the item's color.
	if ((lpDIS->itemState & ODS_SELECTED) &&
		 (lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		CBrush brush(crNorm);
		pDC->FillRect(&lpDIS->rcItem, &brush);
	}
	// If item has been deselected, draw the rectangle using the window color.
	else //if (!(lpDIS->itemState & ODS_SELECTED) &&(lpDIS->itemAction & ODA_SELECT))
	{
		//CBrush brush(::GetSysColor(COLOR_WINDOW));
		CBrush brush(GetBkColor());
  	    pDC->FillRect(&lpDIS->rcItem, &brush);
	}	 	
    
	
	// If item has focus, draw the focus rect.
	if ( (lpDIS->itemState & ODS_FOCUS))
	{//(lpDIS->itemAction & ODA_FOCUS) &&
		pDC->DrawFocusRect(&lpDIS->rcItem); 
	}
	/*****
	// If item does not have focus, redraw (erase) the focus rect.
	if ((lpDIS->itemAction & ODA_FOCUS) && !(lpDIS->itemState & ODS_FOCUS))
	{
		pDC->DrawFocusRect(&lpDIS->rcItem); 
	}
    ****/ //其实没有必要画焦点筐

	// Set the background mode to TRANSPARENT to draw the text.
	int nBkMode= pDC->SetBkMode(TRANSPARENT);

	// If the item's color information is set, use the highlight color
	// gray text color, or normal color for the text.
	if (lpDIS->itemData)		
	{
		if (lpDIS->itemState & ODS_SELECTED)
			crText = pDC->SetTextColor(crHilite);
		else if (lpDIS->itemState & ODS_DISABLED)
			crText = pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		else
			crText = pDC->SetTextColor(crNorm);
		
	}
	// Else the item's color information is not set, so use the
	// system colors for the text.
	else
	{
		if (lpDIS->itemState & ODS_SELECTED)
			crText = pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		else if (lpDIS->itemState & ODS_DISABLED)
			crText = pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		else
			crText = pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	}

    
	for(int i=0;i<m_ColNum;i++)
	{
		// Get and display item text.
		sText=this->GetItemText(lpDIS->itemID,i);
		//CRect rect = lpDIS->rcItem;
		CRect rect;
		this->GetSubItemRect(lpDIS->itemID,i,LVIR_BOUNDS,rect);
		rect.BottomRight().x-=1;
		rect.TopLeft().x+=5;//调整一下矩形的位置
		// Setup the text format.
		UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
		if (GetStyle() & LBS_USETABSTOPS)
			nFormat |= DT_EXPANDTABS;
		
		// Calculate the rectangle size before drawing the text.
		//没有必要计算，否则字母长了会超出矩形
	    //pDC->DrawText(sText, -1, &rect, nFormat | DT_CALCRECT);
		pDC->DrawText(sText, -1, &rect, nFormat);
	}
	
	pDC->SetTextColor(crText); 
	pDC->SetBkMode(nBkMode);

}

int CClrListCtrl::InsertColumn( int nCol, const LVCOLUMN* pColumn )
{

    int n=((CListCtrl*)this)->InsertColumn(nCol,pColumn);
	if(n>=0)
	    m_ColNum++;
	return n;
}
int CClrListCtrl::InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int nFormat , int nWidth , int nSubItem )
{
	int n=((CListCtrl*)this)->InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,nSubItem);
	if(n>=0)
	    m_ColNum++;
	return n;
}

BOOL CClrListCtrl::DeleteColumn( int nCol )
{
	 BOOL flag=((CListCtrl*)this)->DeleteColumn(nCol);
     if(flag)
	 {
		 m_ColNum--;
	 }
     return flag;
}

int CClrListCtrl::InsertItem(int nIndex, LPCTSTR lpszItem, COLORREF rgb)
{
	int nItem = ((CListCtrl*)this)->InsertItem(nIndex,lpszItem);
	if (nItem >= 0)
		SetItemData(nItem, rgb);
 	return nItem;
}
int CClrListCtrl::InsertItem(int nIndex, LPCTSTR lpszItem)
{
	int nItem = ((CListCtrl*)this)->InsertItem(nIndex,lpszItem);
	return nItem;
}

void CClrListCtrl::SetItemColor(int nIndex, COLORREF rgb)
{
	SetItemData(nIndex, rgb);
	RedrawWindow();
}

COLORREF CClrListCtrl::GetItemColor(int nIndex)
{
    return (COLORREF)GetItemData(nIndex);
}

void CClrListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	MyDrawItem(lpDrawItemStruct);
}


void CClrListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0L,LVS_OWNERDRAWFIXED); //自绘的必须有这个属性
	CListCtrl::PreSubclassWindow();
}

int CClrListCtrl::GetColumnCount()
{
    return m_ColNum;
}


BOOL CClrListCtrl::DeleteItem(int nItem)
{
	return CListCtrl::DeleteItem(nItem);
}

BOOL CClrListCtrl::DeleteAllItems()
{

    return CListCtrl::DeleteAllItems();

}
