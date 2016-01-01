#if !defined(AFX_MYLIST_H__1F22266C_98BE_4EA2_BCE1_64967D0CC301__INCLUDED_)
#define AFX_MYLIST_H__1F22266C_98BE_4EA2_BCE1_64967D0CC301__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyList window
#include  <Afxtempl.h>
#include  "MyList.cpp"
class CMyList : public CListCtrl
{
// Construction
public:
	CMyList();
    CMap<DWORD , DWORD& , COLORREF , COLORREF&> MapItemColor;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyList();
    afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	void SetItemColor(DWORD  iItem, COLORREF color);
	bool InitSystemImageLists(HWND hwndList);
	int  GetFileIcon(LPCTSTR lpFileName);
	//void SetFolder(CString sFile);
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyList)
	//afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLIST_H__1F22266C_98BE_4EA2_BCE1_64967D0CC301__INCLUDED_)
