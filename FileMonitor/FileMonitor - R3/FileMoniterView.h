// FileMoniterView.h : interface of the CFileMoniterView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMONITERVIEW_H__707406DA_C70E_4740_870B_D74798A74611__INCLUDED_)
#define AFX_FILEMONITERVIEW_H__707406DA_C70E_4740_870B_D74798A74611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFileMoniterView : public CListView
{
protected: // create from serialization only
	CFileMoniterView();
	DECLARE_DYNCREATE(CFileMoniterView)

// Attributes
public:
	CFileMoniterDoc* GetDocument();

// Operations
public:
	BOOL LoadNTDriver(CHAR* lpszDriverName, CHAR* lpszDriverPath);
	BOOL Startservice(CHAR* lpszDriverName, CHAR*  lpszDriverPath);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileMoniterView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	CListCtrl *m_pList;
	virtual ~CFileMoniterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFileMoniterView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FileMoniterView.cpp
inline CFileMoniterDoc* CFileMoniterView::GetDocument()
   { return (CFileMoniterDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEMONITERVIEW_H__707406DA_C70E_4740_870B_D74798A74611__INCLUDED_)
