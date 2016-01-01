#if !defined(AFX_CLRLISTCTRL_H__018648B9_03A7_41BA_B427_936525B2DB84__INCLUDED_)
#define AFX_CLRLISTCTRL_H__018648B9_03A7_41BA_B427_936525B2DB84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClrListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClrListCtrl window

class CClrListCtrl : public CListCtrl
{
// Construction
public:
	CClrListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClrListCtrl)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL DeleteAllItems( );
	BOOL DeleteItem( int nItem );
	int GetColumnCount();
	void SetItemColor(int nIndex, COLORREF rgb);
	COLORREF GetItemColor(int nIndex);
	int InsertColumn( int nCol, const LVCOLUMN* pColumn );
	int InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1 );
	BOOL DeleteColumn( int nCol );
	int InsertItem(int nIndex, LPCTSTR lpszItem, COLORREF rgb);
	int InsertItem(int nIndex, LPCTSTR lpszItem);
	void MyDrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual ~CClrListCtrl();

	// Generated message map functions
protected:
	int m_ColNum;
	//{{AFX_MSG(CClrListCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLRLISTCTRL_H__018648B9_03A7_41BA_B427_936525B2DB84__INCLUDED_)
